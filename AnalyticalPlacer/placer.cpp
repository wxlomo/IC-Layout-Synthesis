/*
 * placer.cpp
 * Logic of cell displacement and spreading
 * 
 * Author:Weixuan Yang
 * Date: Oct 27, 2021
 */

#include "placer.h"

/* Constants */
const int it_spr = 200;            // Max Iteration of spreading
const int omega = 1;               // Max block number in a bin
const float resolution = 1000.0;   // Resolution of the plot
//bool debug = true;               // Generate log for the process
const int it_max = 1000;           // Max Outer Iteration @ MGMRES
const double tolerance = 0.000001; // Absolute and relative tolerances @ MGMRES

Placer* circuit = NULL;
char msg[100] = { 0 };

Placer::Placer() {
	psi = 1.0;
	WL = 0.0;
	disp = 0.0;
	it = 0;
	mode = -1;
	blocki = -1;
	neti = -1;
	blockx = -1;
	blocky = -1;
	gridsize = -1;
	result = false;
}

Placer::~Placer() {
	for (unsigned int i = 0; i < block_list.size(); i++) {
		if (block_list[i]) delete block_list[i];
		block_list[i] = NULL;
	}
	for (unsigned int i = 0; i < net_list.size(); i++) {
		if (net_list[i]) delete net_list[i];
		net_list[i] = NULL;
	}
	block_map.clear();
	block_movable.clear();
	block_not_movable.clear();
	net_map.clear();
}

/* Implementation */
void Placer::placing() {
	string filename;
	ifstream filein;
	//if (debug) cout << "\n---------------------------- Run Under Debug Mode --------------------------" << endl;
	for (;;) {
		cout << "\nPlease enter the name of the input file: (e.g. cct1.txt)\n";
		getline(cin, filename);
		filein.open(filename, ios::in);
		if (!filein.is_open()) {
			cerr << "\nThe file you entered does not exist, please try again." << endl;
		}
		else {
			break;
		}
	}
	for (;;) {
		filein >> blocki;
		if (blocki == -1)break;
		Block* block = new Block(blocki);
		block_list.push_back(block);
		block_map.insert(pair<int, Block*>(blocki, block));
		for (;;) {
			filein >> neti;
			if (neti == -1)break;
			block->to_block_net_list().push_back(neti);
			Net* net_exist = find_net(neti);
			if (net_exist == NULL) {
				Net* net = new Net(neti);
				net_list.push_back(net);
				net_map.insert(pair<int, Net*>(neti, net));
			}
		}
	}
	for (;;) {
		filein >> blocki;
		if (blocki == -1)break;
		filein >> blockx >> blocky;
		Block* block = find_block(blocki);
		if (block != NULL) {
			block->set_fixed(true);
			block->set_xy(blockx, blocky);
		}
		else {
			Block* block = new Block(blocki);
			block->set_fixed(true);
			block->set_xy(blockx, blocky);
			block_list.push_back(block);
		}
		if (blockx + 1 > gridsize) gridsize = blockx + 1;
		if (blocky + 1 > gridsize) gridsize = blocky + 1;
	}
	for (unsigned int i = 0; i < block_list.size(); i++) {
		if (!block_list[i]->block_get_fixed()) {
			block_movable.push_back(block_list[i]->block_get_index());
		}
		else {
			block_not_movable.push_back(block_list[i]->block_get_index());
		}
	}
	for (unsigned int i = 0; i < net_list.size(); i++) {
		for (unsigned int j = 0; j < block_list.size(); j++) {
			for (unsigned int k = 0; k < block_list[j]->to_block_net_list().size(); k++) {
				if (block_list[j]->to_block_net_list().at(k) == net_list[i]->net_get_index()) {
					net_list[i]->to_net_block_list().push_back(block_list[j]->block_get_index());
				}
			}
		}
	}
	cout << "\nPlease select the operating mode.\n[1] Spreading with default parameters \n    - Quadratic PSI \n    - Bins sorted by ascending supplies \n    - Normal anchor weights" << endl;
	cout << "[2] Solve the initial placement only \n[3] Spreading with Linearly increased PSI \n[4] Spreading with reversed order of bins \n[5] Spreading with strong anchor weights" << endl;
	cout << "[6] Spreading with weak anchor weights \n[7] Spreading with strong anchor weights and reversed order of bins \n[8] Spreading with weak anchor weights and reversed order of bins"<< endl;
	cin >> mode;
	if (!cin.good() || mode < 1 || mode > 8) {
		cerr << "\nInvalid input. The program will run with mode [1]." << endl;
		mode = 1;
	}
	filein.close();
	if (!init_block()) mode = -1;
	event_loop(act_on_button_press, NULL, NULL, plot_block);
	while (mode != 2 && mode != -1) {
		result = spread_block();
		if (result || it == it_spr)break; // break after completly or max times spreaded
		#if __linux__
		clearscreen();
		plot_block();
		#endif
		event_loop(act_on_button_press, NULL, NULL, plot_block);
		it++;
	}
	if (mode != 2) {
		if (result) {
			sprintf(msg, "Iteration %d: Spreading Complete. Total HPWL: %.2f. Total cell displacement: %.2f", it, WL, disp);
			update_message(msg);
			cout << "\n------------- Converged at iteration " << it << ". Spreading Complete ----------------- " << endl;
		}
		else {
			sprintf(msg, "Iteration %d: Spreading Failed. Total HPWL: %.2f. Total cell displacement: %.2f", it, WL, disp);
			update_message(msg);
			cout << "\n----------------------------- Spreading Failed -----------------------------" << endl;
		}
		#if __linux__
		event_loop(act_on_button_press, NULL, NULL, close_graphics);
		#else
		event_loop(act_on_button_press, NULL, NULL, plot_block);
		#endif
	}
}

/* Calculate the initial weight of the blocks */
bool Placer::init_block() {
	for (unsigned int i = 0; i < net_list.size(); i++) {
		double p = net_list[i]->to_net_block_list().size();
		net_list[i]->net_set_weight(2.0 / p);
	}
	if (!calc_weight()) return false;
	if (!solv_weight(AA, bx, by)) return false;
	calc_HPWL();
	plot_init();
	cout << "\nInitial HPWL: " << WL << endl;
	sprintf(msg, "Press 'Proceed' to continue. Initial HPWL: %.2f", WL);
	update_message(msg);
	return true;
}
/* Algorithm 1 @ MFSCAP */
bool Placer::spread_block() {
	if (mode == 3) psi = 2 * (it + 1.0) * (double)omega; // Linear PSI function
	else psi = (it + 1.0) * (it + 1.0) * (double)omega * (double)omega; // Quadratic PSI function
	vector<tuple<Block*, double, double> > update_loc; // Store the potential displacement for update
	for (unsigned int i = 0; i < bin_list.size(); i++) {
		if (bin_list[i]) delete bin_list[i];
		bin_list[i] = NULL;
	}
	bin_list.clear();
	for (unsigned int i = 0; i < block_list.size(); i++) { // Rounding the coordinates of the blocks and pair with bins
		int bin_x = (int)round(block_list.at(i)->block_get_x());
		int bin_y = (int)round(block_list.at(i)->block_get_y());
		block_list.at(i)->set_distance(0.0);
		if (!find_overlap_bin(bin_x, bin_y, block_list.at(i)->block_get_index())) { // Find all overlapped bins when paring, if not, create a new bin into the bin list
			Bin* bin = new Bin(bin_x, bin_y);
			bin->to_bin_block_list().push_back(block_list.at(i)->block_get_index());
			bin_list.push_back(bin);
		}
	}
	vector<Bin*>B;
	for (unsigned int i = 0; i < bin_list.size(); i++) {
		Bin* bin = bin_list[i];
		if (bin->bin_get_overlap()) B.push_back(bin);
	}
	if (B.empty()) {
		return true; // No overlapped bin
	}

	if (mode == 4 || mode == 7 || mode == 8) sort(B.begin(), B.end(), Bin::bin_supply_reversed()); // Sort the overlap bin list with reversed order
	else sort(B.begin(), B.end(), Bin::bin_supply());

	//if (debug) {
	//	cout << "\nBins";
	//	for (unsigned int i = 0; i < bins.size(); i++) {
	//		cout << "\n" << bins[i]->bin_get_x() << "," << bins[i]->bin_get_y() << " ";
	//		if (bins[i]->bin_get_overlap()) cout << "is overlap ";
	//		for (unsigned int j = 0; j < bins[i]->to_bin_block_list().size(); j++) {
	//			cout << bins[i]->to_bin_block_list().at(j) << " ";
	//		}
	//	}
	//	cout << "\nSorted Overlap Bins" << endl;
	//	for (unsigned int i = 0; i < B.size(); i++) {
	//		cout << B[i]->bin_get_x() << "," << B[i]->bin_get_y() << " ";
	//	}
	//	cout << endl;
	//}

	for (unsigned int i = 0; i < B.size(); i++) {
		vector<Path*>paths = find_path(B[i], B); // Find all available paths
		sort(paths.begin(), paths.end(), Path::path_cost()); 
		for (unsigned int j = 0; j < paths.size(); j++) {
			if (calc_supply(B[i]->to_bin_block_list().size()) > 0) {
				move_path(paths[j], update_loc); // Attempt to move the blocks on the path
			}
		}
		for (unsigned int j = 0; j < paths.size(); j++) {
			if (paths[j]) delete paths[j];
			paths[j] = NULL;
		}
	}
	for (unsigned int i = 0; i < block_list.size(); i++) {
		block_list[i]->set_old_xy(block_list[i]->block_get_x(), block_list[i]->block_get_y());
	}

	for (unsigned int i = 0; i < update_loc.size(); i++) { // Update coordinates for pseudo nets
		Block* block = get<0>(update_loc[i]);
		double xx = get<1>(update_loc[i]);
		double yy = get<2>(update_loc[i]);
		block->set_xy(block->block_get_x() + xx, block->block_get_y() + yy);
	}

	vector<vector<double>>AA_pseudo;
	vector<double>bx_pseudo;
	vector<double>by_pseudo;
	AA_pseudo.resize(block_movable.size());
	bx_pseudo.resize(block_movable.size());
	by_pseudo.resize(block_movable.size());
	for (unsigned int i = 0; i < block_movable.size(); i++) { // Create copy of the A and b matrices for implementing the pseudo block weights
		AA_pseudo[i].resize(block_movable.size(), 0.0);
		bx_pseudo[i] = bx[i];
		by_pseudo[i] = by[i];
		for (unsigned int j = 0; j < block_movable.size(); j++) {
			AA_pseudo[i][j] = AA[i][j];
		}
	}
	for (unsigned int i = 0; i < block_movable.size(); i++) { // Apply anchor weights for spreading and update the matrices
		double kanchor = 0;
		if (mode == 5 || mode ==7) kanchor = 20 * AA[0][0]; // Strong anchor weight
		else if (mode == 6 || mode == 8) kanchor = 2 * AA[0][0]; // Weak anchor weight
		else kanchor = 8 * AA[0][0]; // normal anchor weight
		Block* block_pseudo = find_block(block_movable[i]);
		if (block_pseudo != NULL) { 
			double pseudo_x = block_pseudo->block_get_x();
			double pseudo_y = block_pseudo->block_get_y();
			double anchor = kanchor * block_movable.size();
			AA_pseudo[i][i] += anchor;
			bx_pseudo[i] += anchor * pseudo_x;
			by_pseudo[i] += anchor * pseudo_y;
		} // Only update the weights regarding the anchors in the matrices
		else mode = -1;
	}
	if (!solv_weight(AA_pseudo, bx_pseudo, by_pseudo)) mode = -1;
	calc_HPWL();
	calc_disp();
	cout << "Iteration: " << it + 1 << " HPWL: " << WL << " DISP: " << disp << endl;
	sprintf(msg, "Iteration %d: Press 'Proceed' to continue. Current HPWL: %.2f", it + 1, WL);
	update_message(msg);
	return false;
}

/* Build A and b matrices */
bool Placer::calc_weight() {
	bx.clear();
	by.clear();
	AA.clear();
	AA.resize(block_movable.size());
	for (unsigned int i = 0; i < AA.size(); i++) {
		AA[i].resize(block_movable.size(), 0.0);
	}
	for (unsigned int i = 0; i < block_movable.size(); i++) {  // Movable blocks
		for (unsigned int j = 0; j < block_movable.size(); j++) {
			double weight = 0.0;
			Block* blocki = find_block(block_movable[i]);
			Block* blockj = find_block(block_movable[j]);
			if (blocki == NULL || blockj == NULL) return false;
			if (i != j) {
				for (unsigned int a = 0; a < blocki->to_block_net_list().size(); a++) {
					for (unsigned int b = 0; b < blockj->to_block_net_list().size(); b++) {
						if (blocki->to_block_net_list().at(a) == blockj->to_block_net_list().at(b)) {
							Net* net = find_net(blocki->to_block_net_list().at(a));
							if (net != NULL) weight -= net->net_get_weight();
							else return false;
						}
					}
				}
			}
			else {
				for (unsigned int a = 0; a < blocki->to_block_net_list().size(); a++) {
					Net* net = find_net(blocki->to_block_net_list().at(a));
					if (net != NULL) weight += (net->net_get_weight()) * (net->to_net_block_list().size() - 1);
					else return false;
				}
			}
			AA[i][j] = weight;
		}
	}

	for (unsigned int i = 0; i < block_movable.size(); i++) { // Fixed blocks
		double weightx = 0.0;
		double weighty = 0.0;
		for (unsigned int j = 0; j < block_not_movable.size(); j++) {
			Block* blocki = find_block(block_movable[i]);
			Block* blockj = find_block(block_not_movable[j]);
			if (blocki == NULL || blockj == NULL) return false;
			for (unsigned int a = 0; a < blocki->to_block_net_list().size(); a++) {
				for (unsigned int b = 0; b < blockj->to_block_net_list().size(); b++) {
					if (blocki->to_block_net_list().at(a) == blockj->to_block_net_list().at(b)) {
						Net* net = find_net(blocki->to_block_net_list().at(a));
						if (net != NULL) {
							weightx += (net->net_get_weight()) * (blockj->block_get_x());
							weighty += (net->net_get_weight()) * (blockj->block_get_y());
						}
						else return false;
					}
				}
			}
		}
		bx.push_back(weightx);
		by.push_back(weighty);
	}
	return true;
}

/* Solve the x and y matrices @ MGMRES */
bool Placer::solv_weight(vector<vector<double>>& A, vector<double>& b_x, vector<double>& b_y) {
	vector<int>ia;
	vector<int>ja;
	vector<double>x;
	vector<double>y;
	vector<double>a;
	for (unsigned int i = 0; i < A.size(); i++) {
		for (unsigned int j = 0; j < A[i].size(); j++) {
			if (A[i][j] != 0) {
				a.push_back(A[i][j]);
				ia.push_back(i);
				ja.push_back(j);
			}
		}
	}
	int n = block_movable.size();
	int nz_num = a.size();
	x.resize(n, 0.0);
	y.resize(n, 0.0);
	mgmres_st(n, nz_num, &ia[0], &ja[0], &a[0], &x[0], &b_x[0], it_max, n - 1, tolerance, tolerance);
	mgmres_st(n, nz_num, &ia[0], &ja[0], &a[0], &y[0], &b_y[0], it_max, n - 1, tolerance, tolerance);
	
	if (x.empty() || y.empty()) return false;
	for (unsigned int i = 0; i < x.size(); i++) { // Update coordinates
		Block* block = find_block(block_movable[i]);
		if (block != NULL) block->set_xy(x[i], y[i]);
		else return false;
	}

	//if (debug) {
	//	cout << "\nSolved coordinates" << endl;
	//	for (unsigned int i = 0; i < x.size(); i++) {
	//		cout << x[i] << "," << y[i] << "  ";
	//	}
	//}

	return true;
}

/* UMF Solver @ SuiteSparse */
//void umfsolver(vector<double>& x, int n, vector<int>& ja, vector<int>& ia, vector<double>& a, vector<double>& b) {
//	void* Symbolic, * Numeric;
//	(void)umfpack_di_symbolic(n, n, &ja[0], &ia[0], &a[0], &Symbolic, NULL, NULL);
//	(void)umfpack_di_numeric(&ja[0], &ia[0], &a[0], Symbolic, &Numeric, NULL, NULL);
//	(void)umfpack_di_solve(UMFPACK_A, &ja[0], &ia[0], &a[0], &x[0], &b_x[0], Numeric, NULL, NULL);
//	umfpack_di_free_symbolic(&Symbolic);
//	umfpack_di_free_numeric(&Numeric);
//}
//umfsolver(x, n, ja, ia, a, b_x);
//umfsolver(y, n, ja, ia, a, b_y);
/* Not used. Needs "umfpack.h" */

/* Algorithm 2 @ MFSCAP */
vector<Path*> Placer::find_path(Bin* bin, vector<Bin*>of_bins) {
	vector<Bin*>path;
	vector<Path*>P;
	queue<vector<Bin*> >Q;
	queue<double>Qcost;
	double demand = 0.0;
	for (unsigned int i = 0; i < bin_list.size(); i++) {
		bin_list[i]->set_visited(false);
	}
	bin->set_visited(true);
	path.push_back(bin);
	Q.push(path);
	Qcost.push(0.0);
	while (!Q.empty() && (double)calc_supply(bin->to_bin_block_list().size()) > demand) {
		path = Q.front();
		double costpath = Qcost.front();
		Q.pop();
		Qcost.pop();
		Bin* vsrc = path.back();
		Bin* vi = NULL;
		for (unsigned int i = 0; i < 4; i++) {
			int x = -1;
			int y = -1;
			switch (i) {
			case 0: // left
				x = vsrc->bin_get_x() - 1;
				y = vsrc->bin_get_y();
				break;
			case 1: // top
				x = vsrc->bin_get_x();
				y = vsrc->bin_get_y() + 1;
				break;
			case 2: // right
				x = vsrc->bin_get_x() + 1;
				y = vsrc->bin_get_y();
				break;
			case 3: // bottom
				x = vsrc->bin_get_x();
				y = vsrc->bin_get_y() - 1;
				break;
			}
			bool found = false;
			if (x >= 0 && x < gridsize && y >= 0 && y < gridsize) { // Exceed the range of the board
				for (unsigned int j = 0; j < bin_list.size(); j++) {
					if (x == bin_list[j]->bin_get_x() && y == bin_list[j]->bin_get_y()) {
						vi = bin_list[j];
						found = true;
						break;
					}
				}
				if (!found) {
					vi = new Bin(x, y);
					bin_list.push_back(vi);
				}
			}
			else continue;
			if (!vi->bin_get_visited()) {
				double cost = calc_cost(vsrc, vi);
				if (cost != -1.0) { // -1.0 -> infinity
					vector<Bin*>pathi = path;
					double costi = costpath + cost;
					pathi.push_back(vi);
					vi->set_visited(true);
					if (vi->to_bin_block_list().empty()) {
						P.push_back(new Path(pathi, costi));
						demand = demand + 1;
					}
					else {
						Q.push(pathi);
						Qcost.push(costi);
					}
				}
			}
		}
	}
	//if (debug) {
	//	cout << "bin:" << bin->bin_get_x() << "," << bin->bin_get_y() << endl;
	//	for (unsigned int i = 0; i < P.size(); i++) {
	//		cout << "Path: ";
	//		for (unsigned int j = 0; j < P[i]->to_path_bins().size(); j++) {
	//			cout << P[i]->to_path_bins().at(j)->bin_get_x() << "," << P[i]->to_path_bins().at(j)->bin_get_y() << " ";
	//		}
	//	}
	//	cout << endl;
	//}
	return P;
}

/* Algorithm 3 @ MFSCAP */
void Placer::move_path(Path* path, vector<tuple<Block*, double, double> >& update_loc) {
	vector<Bin*>P = path->to_path_bins();
	Bin* vsrc = P[0];
	stack<Bin*>S;
	S.push(vsrc);
	for (unsigned i = 1; i < P.size(); i++) {
		Bin* vsink = P[i];
		double cost = calc_cost(vsrc, vsink);
		if (cost == -1.0) return;
		vsrc = vsink;
		S.push(vsink);
	}
	Bin* vsinki = S.top();
	S.pop();
	while (!S.empty()) {
		Bin* vsrci = S.top();
		S.pop();
		int block_des = find_block_by_distance(vsrci->to_bin_block_list());
		Block* blockd = find_block(block_des);
		for (unsigned int i = 0; i < vsrci->to_bin_block_list().size(); i++) { // Remove the block from the spurce bin
			if (block_des == vsrci->to_bin_block_list().at(i)) {
				for (unsigned int j = i; j < vsrci->to_bin_block_list().size() - 1; j++) {
					vsrci->to_bin_block_list().at(j) = vsrci->to_bin_block_list().at(j + 1);
				}
				vsrci->to_bin_block_list().pop_back();
				break;
			}
		}
		vsinki->to_bin_block_list().push_back(block_des); // Add the block to the sink bin
		blockd->set_distance(blockd->block_get_distance() + 1.0);
		double xx = (double)vsinki->bin_get_x() - (double)vsrci->bin_get_x();
		double yy = (double)vsinki->bin_get_y() - (double)vsrci->bin_get_y();
		update_loc.push_back(make_tuple(blockd, xx, yy)); // Store the designated displacements
		vsinki = vsrci;
	}
}

/* Calculate the cost @ w = 1 as all the blocks are equally weighted */
double Placer::calc_cost(Bin* vsrc, Bin* vi) {
	double weight = 1.0;
	double phi = 1.0;
	int gamma = find_block_by_distance(vsrc->to_bin_block_list());
	Block* gamma_star = find_block(gamma);
	if (gamma_star == NULL) return -1.0; // Infinity
	phi = (vi->bin_get_x() - (double)round(gamma_star->block_get_x())) * (vi->bin_get_x() - (double)round(gamma_star->block_get_x())) +
		(vi->bin_get_y() - (double)round(gamma_star->block_get_y())) * (vi->bin_get_y() - (double)round(gamma_star->block_get_y()));
	if (phi > psi) return -1.0;
	double cost = weight * phi;
	return cost;
}

/* Calculate the supply @ omega = 1 as only one block can be placed in each bin finally */
int Placer::calc_supply(int list_size) { 
	return list_size - omega;
}

/* Calculate the HPWL */
void Placer::calc_HPWL() {
	WL = 0.0;
	for (unsigned int i = 0; i < net_list.size(); i++) {
		double x_bottom = gridsize + 1.0;
		double x_top = 0.0;
		double y_left = gridsize + 1.0;
		double y_right = 0.0;
		for (unsigned int j = 0; j < net_list[i]->to_net_block_list().size(); j++) {
			Block* block = find_block(net_list[i]->to_net_block_list().at(j));
			x_bottom = min(x_bottom, block->block_get_x());
			x_top = max(x_top, block->block_get_x());
			y_left = min(y_left, block->block_get_y());
			y_right = max(y_right, block->block_get_y());
		}
		WL += (x_top - x_bottom) + (y_right - y_left);
	}
}

void Placer::calc_disp() {
	for (unsigned int i = 0; i < block_list.size(); i++) { // Calculate cell displacement
		disp += fabs(block_list[i]->block_get_old_x() - block_list[i]->block_get_x()) + fabs(block_list[i]->block_get_old_y() - block_list[i]->block_get_y());
	}
}

/* Find the block in the bin with minimum cost */
int Placer::find_block_by_distance(vector<int> block_list) {
	double minima = gridsize * 2.0;
	int index = -1;
	for (unsigned int i = 0; i < block_list.size(); i++) {
		Block* block = find_block(block_list[i]);
		if (block == NULL) return index;
		if (block->block_get_distance() < minima && !block->block_get_fixed()) {
			minima = block->block_get_distance();
			index = block->block_get_index();
		}
	}
	return index;
}

/* Find the exist bins and pair them with blocks */
bool Placer::find_overlap_bin(int x, int y, int block) {
	if (bin_list.empty()) return false;
	else {
		for (unsigned int i = 0; i < bin_list.size(); i++) {
			if (x == bin_list[i]->bin_get_x() && y == bin_list[i]->bin_get_y()) {
				bin_list[i]->set_overlap(true);
				bin_list[i]->to_bin_block_list().push_back(block);
				return true;
			}
		}
	}
	return false;
}

Block* Placer::find_block(int index) {
	auto i = block_map.find(index);
	if (i != block_map.end()) return i->second;
	else return NULL;
}

Net* Placer::find_net(int index) {
	auto i = net_map.find(index);
	if (i != net_map.end()) return i->second;
	else return NULL;
}

Bin* Placer::find_bin(int x, int y) {
	if (bin_list.empty()) return NULL;
	else {
		for (unsigned int i = 0; i < bin_list.size(); i++) {
			if (x == bin_list[i]->bin_get_x() && y == bin_list[i]->bin_get_y()) {
				return bin_list[i];
			}
		}
	}
	return NULL;
}

int Placer::get_grid_size() { 
	return gridsize; 
}

vector<Block*>& Placer::to_block_list() { 
	return block_list; 
}

vector<Net*>& Placer::to_net_list() { 
	return net_list;
}

void act_on_button_press(float x, float y) {
}

void plot_init(void) {
	float gridsize = (float)circuit->get_grid_size();
	float window_size_x = resolution * (gridsize + 3);
	float window_size_y = resolution * (gridsize + 1);
	init_graphics("Analytical Placer - by Weixuan Yang @ University of Toronto", WHITE);
	init_world(0., 0., window_size_x, window_size_y);
}

void plot_block(void) {
	set_draw_mode(DRAW_NORMAL);
	clearscreen();
	setlinewidth(1);
	setcolor(BLACK);
	setfontsize(12);
	setlinestyle(DASHED);
	float gridsize = (float)circuit->get_grid_size();
	for (unsigned int i = 0; i < gridsize + 1; i++) { // Plot the grid
		drawline(0, (resolution * i), (resolution * gridsize), (resolution * i));
		drawline((resolution * i), 0, (resolution * i), (resolution * gridsize));
	}
	setlinestyle(SOLID);
	setcolor(5); // Plot the Legend
	fillrect((resolution * (float)(gridsize + 0.8)), (resolution * (float)(gridsize - 2)), (resolution * (float)(gridsize + 1.2)), (resolution * (float)(gridsize - 1.6)));
	setcolor(9);
	fillrect((resolution * (float)(gridsize + 0.8)), (resolution * (float)(gridsize - 1)), (resolution * (float)(gridsize + 1.2)), (resolution * (float)(gridsize - 0.6)));
	setcolor(BLACK);
	sprintf(msg, "Movable Blocks");
	drawtext((resolution * (float)(gridsize + 1.9)), (resolution * (float)(gridsize - 1.8)), msg, resolution * gridsize);
	sprintf(msg, "Fixed Blocks");
	drawtext((resolution * (float)(gridsize + 1.9)), (resolution * (float)(gridsize - 0.8)), msg, resolution * gridsize);
	setcolor(3);
	for (unsigned int i = 0; i < circuit->to_net_list().size(); i++) { // Plot the nets
		for (unsigned int j = 0; j < circuit->to_net_list().at(i)->to_net_block_list().size(); j++) {
			Block* block = circuit->find_block(circuit->to_net_list().at(i)->to_net_block_list().at(j));
			float x_src = (float)(block->block_get_x() + 0.5);
			float y_src = (float)(gridsize - block->block_get_y() - 0.5);
			for (unsigned int k = 0; k < circuit->to_net_list().at(i)->to_net_block_list().size(); k++) {
				Block* blockd = circuit->find_block(circuit->to_net_list().at(i)->to_net_block_list().at(k));
				float x_sink = (float)(blockd->block_get_x() + 0.5);
				float y_sink = (float)(gridsize - blockd->block_get_y() - 0.5);
				drawline((resolution * (float)x_src), (resolution * (float)y_src), (resolution * (float)x_sink), (resolution * (float)y_sink));
			}
		}
	}
	for (unsigned int i = 0; i < circuit->to_net_list().size(); i++) { //Plot the blocks
		for (unsigned int j = 0; j < circuit->to_net_list().at(i)->to_net_block_list().size(); j++) {
			Block* block = circuit->find_block(circuit->to_net_list().at(i)->to_net_block_list().at(j));
			float x_src = (float)(block->block_get_x() + 0.5);
			float y_src = (float)(gridsize - block->block_get_y() - 0.5);
			if (block->block_get_fixed()) setcolor(9);
			else setcolor(5);
			fillrect((resolution * (float)(x_src - 0.1)), (resolution * (float)(y_src - 0.1)), (resolution * (float)(x_src + 0.1)), (resolution * (float)(y_src + 0.1)));
		}
	}
}