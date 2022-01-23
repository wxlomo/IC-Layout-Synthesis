/*
 * placer.cpp
 * Logic of cell displacement and spreading
 * 
 * Author:Weixuan Yang
 * Date: Oct 27, 2021
 */

#include "placer.h"
#include "umfpack/umfpack.h"

Placer* circuit = NULL;
char msg[100] = { 0 };

Placer::Placer() {
	psi = 1.0;
	WL = 0.0;
	disp = 0.0;
	it = 0;
	blocki = -1;
	neti = -1;
	blockx = -1;
	blocky = -1;
	gridsize = -1;
	result = false;
	flag = false;
	linear_psi = false;
	reversed_bins = false;
	strong_anchor = false;
	weak_anchor = false;
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
bool Placer::placing(string filename) {
	ifstream filein;
	filein.open(filename, ios::in);
	if (filein.is_open()) cout << "\n\n" << filename << " is opened successfully." << endl;
	else {
		filein.open(filename + ".txt", ios::in);
		if (filein.is_open()) cout << "\n\n" << filename << ".txt is opened successfully." << endl;
		else return false;
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
	filein.close();
	if (!init_block()) return false;
	while (!flag) {
		#if __linux__
		clearscreen();
		block_plot();
		#endif
		event_loop(act_on_button_press, NULL, NULL, block_plot);
		result = spread_block();
		if (result || it == it_spr)break; // break after completly or max times spreaded
		it++; //Outer loop iteration
	}
	if (result) {
		sprintf(msg, "[Complete] Converged at iteration %d. Total HPWL: %.2f. Total cell displacement: %.0f", it, WL, disp);
		update_message(msg);
		if (it == 1) cout << "\n\nConverged at iteration " << it << ".\n";
		else cout << "\n\n";
		cout << "Total HPWL : " << WL << "\nTotal cell displacement : " << disp << endl;
		cout << "\n\n---------------------------------- Complete --------------------------------\n" << endl;
	}
	else {
		sprintf(msg, "[Failed] Iteration limit reached. Total HPWL: %.2f. Total cell displacement: %.0f", WL, disp);
		update_message(msg);
		cout << "\n\n* Iteration limit reached." << "\nTotal HPWL: " << WL << "\nTotal cell displacement: " << disp << endl;
		cout << "\n\n----------------------------- Spreading Failed -----------------------------\n" << endl;
	}

	#if __linux__
	event_loop(act_on_button_press, NULL, NULL, close_graphics);
	#else
	event_loop(act_on_button_press, NULL, NULL, block_plot);
	#endif
	return true;
}

/* Calculate the initial weight of the blocks */
bool Placer::init_block() {
	for (unsigned int i = 0; i < net_list.size(); i++) { // Initialize the net weights
		double p = net_list[i]->to_net_block_list().size();
		net_list[i]->net_set_weight(2.0 / p);
	}

	if (!set_weight()) return false;
	if (!update_block(Qij, ci, di)) return false;
	calc_HPWL();
	init_plot();
	disp = 0.0;
	cout << "\nInitial HPWL: " << WL << endl;
	sprintf(msg, "[Initial] Press 'Proceed' to continue. HPWL: %.2f. [Created by: Weixuan Yang @ University of Toronto]", WL);
	update_message(msg);
	return true;
}
/* Algorithm 1 @ MFSCAP */
bool Placer::spread_block() {
	int iter = 0; // Inner loop iteration (iter @ MFSCAP)
	bool flag = true; // True if complete spreading
	vector<tuple<Block*, double, double> > update_loc; // Store the potential displacement for update
	for (unsigned int i = 0; i < bin_list.size(); i++) {
		// bin_list[i]->set_overlap(false);
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
		else flag = false;
	}
	if (flag) return true; // Spread complete if no overlapped bins
	while (iter != it_spr) {
		if (linear_psi) psi = kpsi * (iter + 1.0) * (double)omega; // Linear PSI function
		else psi = kpsi * (iter + 1.0) * (iter + 1.0) * (double)omega * (double)omega; // Quadratic PSI function
		vector<Bin*>B;
		for (unsigned int i = 0; i < bin_list.size(); i++) {
			Bin* bin = bin_list[i];
			if (bin->bin_get_overlap()) B.push_back(bin);
		}
		if (B.empty()) {
			break; // No overlapped bin
		}

		if (reversed_bins) sort(B.begin(), B.end(), Bin::bin_supply_reversed()); // Sort the overlap bin list with reversed order
		else sort(B.begin(), B.end(), Bin::bin_supply());

		for (unsigned int i = 0; i < B.size(); i++) {
			vector<Path*>paths = find_path(B[i], B); // Find all available paths
			sort(paths.begin(), paths.end(), Path::path_cost());
			for (unsigned int j = 0; j < paths.size(); j++) {
				if (B[i]->bin_calc_supply() > 0) {
					move_path(paths[j], update_loc); // Attempt to move the blocks on the path
				}
			}
			for (unsigned int j = 0; j < paths.size(); j++) {
				if (paths[j]) delete paths[j];
				paths[j] = NULL;
			}
		}
		
		for (unsigned int i = 0; i < bin_list.size(); i++) { // Update bins' status for next iteration
			if (bin_list[i]->to_bin_block_list().size() > omega) {
				bin_list[i]->set_overlap(true);
			}
			else bin_list[i]->set_overlap(false);
		}
		iter++;
	}

	for (unsigned int i = 0; i < block_movable.size(); i++) { // Initialize the pseudo block coordinates
		Block* pseudo_block = find_block(block_movable[i]);
		if (pseudo_block != NULL) pseudo_block->set_pseudo_xy(pseudo_block->block_get_x(), pseudo_block->block_get_y());
		else return false;
	}

	for (unsigned int i = 0; i < update_loc.size(); i++) { // Update coordinates for pseudo blocks
		Block* block = get<0>(update_loc[i]);
		double xx = get<1>(update_loc[i]);
		double yy = get<2>(update_loc[i]);
		block->set_pseudo_xy(block->block_get_pseudo_x() + xx, block->block_get_pseudo_y() + yy);
	}

	if (!strong_anchor && !weak_anchor) { // No anchor
		for (unsigned int i = 0; i < block_movable.size(); i++) { // Update coordinates
			Block* block = find_block(block_movable[i]);
			if (block == NULL) return false;
			double xx = block->block_get_pseudo_x();
			double yy = block->block_get_pseudo_y();
			calc_disp(block, xx, yy); // Calculate the total cell displacement
			block->set_xy(xx, yy);
		}
		calc_HPWL();
		clearscreen();
		block_plot();
		it++;
		return true;
	}

	vector<vector<double>>Qij_pseudo;
	vector<double>ci_pseudo;
	vector<double>di_pseudo;
	Qij_pseudo.resize(block_movable.size());
	ci_pseudo.resize(block_movable.size());
	di_pseudo.resize(block_movable.size());
	for (unsigned int i = 0; i < block_movable.size(); i++) { // Create copy of the A and b matrices for implementing the pseudo block weights
		Qij_pseudo[i].resize(block_movable.size(), 0.0);
		ci_pseudo[i] = ci[i];
		di_pseudo[i] = di[i];
		for (unsigned int j = 0; j < block_movable.size(); j++) {
			Qij_pseudo[i][j] = Qij[i][j];
		}
	}
	for (unsigned int i = 0; i < block_movable.size(); i++) { // Apply anchor weights for spreading and update the matrices
		double kanchor = 0;
		if (strong_anchor) kanchor = 1000.0; // Strong anchor weight factor
		else if (weak_anchor) kanchor = 1.0; // Weak anchor weight factor
		Block* block_pseudo = find_block(block_movable[i]);
		if (block_pseudo != NULL) { 
			double pseudo_x = block_pseudo->block_get_pseudo_x();
			double pseudo_y = block_pseudo->block_get_pseudo_y();
			Qij_pseudo[i][i] += kanchor * Qij[i][i];
			ci_pseudo[i] += kanchor * Qij[i][i] * pseudo_x;
			di_pseudo[i] += kanchor * Qij[i][i] * pseudo_y;
		} // Only update the weights regarding the anchors in the matrices
		else flag = true;
	}
	if (!update_block(Qij_pseudo, ci_pseudo, di_pseudo)) flag = true;
	calc_HPWL();
	cout << "[" << it + 1 << "] Inner loop converged at iter " << iter << "." << " HPWL: " << WL << " DISP : " << disp << "." << endl;
	sprintf(msg, "[Iteration %d] Press 'Proceed' to continue. Current HPWL: %.2f. Current cell displacement: %.0f", it + 1, WL, disp);
	update_message(msg);
	return false;
}

/* Build Q, c, and d matrices */
bool Placer::set_weight() {
	ci.clear();
	di.clear();
	Qij.clear();
	Qij.resize(block_movable.size());
	for (unsigned int i = 0; i < Qij.size(); i++) {
		Qij[i].resize(block_movable.size(), 0.0);
	}
	for (unsigned int i = 0; i < block_movable.size(); i++) {  // Movable blocks, create Q matrix
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
			Qij[i][j] = weight;
		}
	}

	for (unsigned int i = 0; i < block_movable.size(); i++) { // Fixed blocks, create c, d matrices
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
		ci.push_back(weightx);
		di.push_back(weighty);
	}
	return true;
}

/* Solve the x and y matrices */
bool Placer::update_block(vector<vector<double>>& Q, vector<double>& c, vector<double>& d) {
	vector<int>ia;
	vector<int>ja;
	vector<double>x;
	vector<double>y;
	vector<double>Qs;
	ja.push_back(0);
	for (unsigned int j = 0; j < Q[0].size(); j++) { // Sparse matrix Conversion
		for (unsigned int i = 0; i < Q.size(); i++) {
			if (Q[i][j] != 0) {
				Qs.push_back(Q[i][j]);
				ia.push_back(i);;
			}
		}
		ja.push_back(ia.size());
	}
	int n = block_movable.size();
	x.resize(n, 0.0);
	y.resize(n, 0.0);
	umfsolver(x, n, ja, ia, Qs, c);
	umfsolver(y, n, ja, ia, Qs, d);
	
	if (x.empty() || y.empty()) return false;
	for (unsigned int i = 0; i < x.size(); i++) { // Update coordinates
		Block* block = find_block(block_movable[i]);
		if (block == NULL) return false;
		calc_disp(block, x[i], y[i]); // Calculate the total cell displacement
		block->set_xy(x[i], y[i]);	
	}
	return true;
}

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
	while (!Q.empty() && (double)bin->bin_calc_supply() > demand) {
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

/* UMF Solver @ SuiteSparse */
void Placer::umfsolver(vector<double>& x, int n, vector<int>& ja, vector<int>& ia, vector<double>& a, vector<double>& b) {
	void* Symbolic, * Numeric;
	(void)umfpack_di_symbolic(n, n, &ja[0], &ia[0], &a[0], &Symbolic, NULL, NULL);
	(void)umfpack_di_numeric(&ja[0], &ia[0], &a[0], Symbolic, &Numeric, NULL, NULL);
	(void)umfpack_di_solve(UMFPACK_A, &ja[0], &ia[0], &a[0], &x[0], &b[0], Numeric, NULL, NULL);
	umfpack_di_free_symbolic(&Symbolic);
	umfpack_di_free_numeric(&Numeric);
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

/* Calculate cell displacement */
void Placer::calc_disp(Block* block, double x, double y) {
	disp += fabs(x - block->block_get_x()) + fabs(y - block->block_get_y());
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

void init_plot (void) {
	float gridsize = (float)circuit->get_grid_size();
	float window_size_x = resolution * (gridsize + 3);
	float window_size_y = resolution * (gridsize + 1);
	init_graphics("Analytical Placer", WHITE);
	init_world(0., -200., window_size_x, window_size_y);
	circuit->linear_psi = false;
	circuit->reversed_bins = false;
	circuit->strong_anchor = false;
	circuit->weak_anchor = false;
	create_button("Window", "Default", set_default_plot);
	create_button("Default", "Linear Psi", set_linear_psi_plot);
	create_button("Linear Psi", "Reversed Bin", set_reversed_bins_plot);
	create_button("Reversed Bin", "Strong Anchor", set_strong_anchor_plot);
	create_button("Strong Anchor", "Weak Anchor", set_weak_anchor_plot);
	create_button("Proceed", "Index", show_block_index);
	enable_or_disable_button(8, false);
}

void block_plot(void) {
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

void act_on_button_press(float x, float y) {
}

void set_default_plot(void (*drawscreen_ptr) (void)) {
	sprintf(msg, "Set to default mode. Press 'Proceed' to continue.");
	update_message(msg);
	cout << "Set to default mode" << endl;
	circuit->linear_psi = false;
	circuit->reversed_bins = false;
	circuit->strong_anchor = false;
	circuit->weak_anchor = false;
	enable_or_disable_button(8, false);
	enable_or_disable_button(9, true);
	enable_or_disable_button(10, true);
	enable_or_disable_button(11, true);
	enable_or_disable_button(12, true);
}

void set_linear_psi_plot(void (*drawscreen_ptr) (void)) {
	sprintf(msg, "Set to linear psi mode. Press 'Proceed' to continue.");
	update_message(msg);
	cout << "Set to linear psi mode." << endl;
	circuit->linear_psi = true;
	enable_or_disable_button(8, true);
	enable_or_disable_button(9, false);
}

void set_reversed_bins_plot(void (*drawscreen_ptr) (void)) {
	sprintf(msg, "Set to reversed bin list mode. Press 'Proceed' to continue.");
	update_message(msg);
	cout << "Set to reversed bin list mode." << endl;
	circuit->reversed_bins = true;
	enable_or_disable_button(8, true);
	enable_or_disable_button(10, false);
}

void set_strong_anchor_plot(void (*drawscreen_ptr) (void)) {
	sprintf(msg, "Set to strong anchor weight mode. Press 'Proceed' to continue.");
	update_message(msg);
	cout << "Set to strong anchor weight mode." << endl;
	circuit->strong_anchor = true;
	circuit->weak_anchor = false;
	enable_or_disable_button(8, true);
	enable_or_disable_button(12, true);
	enable_or_disable_button(11, false);
}

void set_weak_anchor_plot(void (*drawscreen_ptr) (void)) {
	sprintf(msg, "Set to weak anchor weight mode. Press 'Proceed' to continue.");
	update_message(msg);
	cout << "Set to weak anchor weight mode." << endl;
	circuit->strong_anchor = false;
	circuit->weak_anchor = true;
	enable_or_disable_button(8, true);
	enable_or_disable_button(11, true);
	enable_or_disable_button(12, false);
}

void show_block_index(void (*drawscreen_ptr) (void)) {
	setcolor(1);
	for (unsigned int i = 0; i < circuit->to_block_list().size(); i++) {
		sprintf(msg, "(%d)", circuit->to_block_list()[i]->block_get_index());
		drawtext((resolution * (float)(circuit->to_block_list()[i]->block_get_x())), (resolution * (float)(circuit->get_grid_size() - circuit->to_block_list()[i]->block_get_y() - 0.6)), msg, resolution * 20);
	}
}

