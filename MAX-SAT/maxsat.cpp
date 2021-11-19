/*
 * maxsat.cpp
 * Logic of MAX-SAT solver
 *
 * Author:Weixuan Yang
 * Date: Nov 17, 2021
 */

#include "maxsat.h"

Maxsat* problem = NULL;
random_device randn;
mt19937 rd(randn());
uniform_int_distribution<int> gen(1, 100);

char msg[100] = { 0 };
const float resolution = 1000.0;

Maxsat::Maxsat() {
	nv = -1;
	nc = -1;
	lb = 0;
	best = 0;
	visited = 0;
	best_d = 0;
	best_x = 0.0;
	start = (clock_t)0.0;
	end = (clock_t)0.0;
	weighted = false;
}

Maxsat::~Maxsat() {
	for (unsigned int i = 0; i < x_list.size(); i++) {
		if (x_list[i]) delete x_list[i];
		x_list[i] = NULL;
	}
	for (unsigned int i = 0; i < clause_list.size(); i++) {
		if (clause_list[i]) delete clause_list[i];
		clause_list[i] = NULL;
	}
	x_map.clear();
}

/* MAX-SAT main process */
bool Maxsat::maxsat(string filename) {
	ifstream filein;
	filein.open(filename, ios::in);
	if (filein.is_open()) cout << "\n\n" << filename << " is opened successfully." << endl;
	else {
		filein.open(filename + ".cnf", ios::in);
		if (filein.is_open()) cout << "\n\n" << filename << ".cnf is opened successfully." << endl;
		else return false;
	}
	filein >> nv;
	filein >> nc;
	if (nv == -1 || nc == -1) return false;
	for (int i = 1; i <= nv; i++) {
		Variable* x = new Variable(i);
		x_map.insert(pair<int, Variable*>(i, x));
	}
	plot_init();
	event_loop(act_on_button_press, NULL, NULL, plot_message);
	enable_or_disable_button(8, false);
	enable_or_disable_button(9, false);
	for (int i = 0; i < nc; i++) {
		Clause* clause = new Clause;
		int j = 0;
		int weight = 1;
		while (filein >> j) {
			if (j == 0) break;
			Variable* xx = NULL;
			if (j < 0) {
				xx = find_x(-j);
				clause->to_disj().push_back(pair<Variable*, int>(xx, -1));
			}
			else {
				xx = find_x(j);
				clause->to_disj().push_back(pair<Variable*, int>(xx, 1));
			}
			if (xx == NULL) return false;
			xx->set_supply(); // the number of related clauses of this variable
		}
		filein >> weight;
		if (weighted) clause->set_weight(weight); // standard (unweighted) / weighted MAX-SAT
		clause_list.push_back(clause);
	}
	filein.close();

	if (!init()) cout << "* Fail to obtain the initial solution." << endl;
	if (!optm()) cout << "* Fail to optimize the solution." << endl;
	event_loop(act_on_button_press, NULL, NULL, plot_blank);
	return true;
}

/* Initial solution */
bool Maxsat::init() {
	start = clock();
	sort(clause_list.begin(), clause_list.end(), Clause::clause_sort()); // sort the clauses by their ascending numbers of variables then their weights 
	int d = 0;
	for (unsigned int i = 0; i < clause_list.size(); i++) {
		sort(clause_list[i]->to_disj().begin(), clause_list[i]->to_disj().end(), Clause::x_sort()); // sort the variables of the current clause by ascending supplies
		bool satisfiable = false;
		for (unsigned int j = 0; j < clause_list[i]->to_disj().size(); j++) { // assign the first undefined variables to make the clause satis?able
			Variable* xx = clause_list[i]->to_disj()[j].first;
			if (xx == NULL) return false;
			if (xx->get_boolean() * clause_list[i]->to_disj()[j].second == 1) {
				satisfiable = true;
				break; // if the clause is already satisfied
			}
			else continue;
		}
		if(!satisfiable){
			for (unsigned int j = 0; j < clause_list[i]->to_disj().size(); j++) {
				Variable* xx = clause_list[i]->to_disj()[j].first;
				if (xx->get_boolean() == 0) {
					d++;
					if (clause_list[i]->to_disj()[j].second == 1) xx->set_boolean(1);
					else xx->set_boolean(-1); // set the first found undefined variable to be true
					xx->set_depth(d); // the depth of the variable in the decision tree
					x_list.push_back(xx); // record the order of assigning variables
					break;
				}		
			}	
		}
	}
	for (unsigned int i = 0; i < x_list.size(); i++) {
		x_list[i]->set_best_boolean(x_list[i]->get_boolean()); // record boolean assignment for best solution
	}
	if (x_list.size() < (unsigned int)nv) { // make sure not to miss any still undefined variable
		for (int i = 1; i <= nv; i++) {
			bool found = false;
			for (unsigned int j = 0; j < x_list.size(); j++) {
				if (i == x_list[j]->get_index()) found = true;
			}
			if (!found) {
				Variable* xx = find_x(i);
				d++;
				xx->set_depth(d);
				x_list.push_back(xx);
			}
		}
		if (x_list.size() != (unsigned int)nv) return false;
	}
	lb = updt_lb(0);
	best = updt_best();
	cout << "\n---------------------------- MAX-SAT optimizing... --------------------------";
	return true;
}

/* Optimize the solution by using decision tree */
bool Maxsat::optm() {
	clearscreen();
	set_draw_mode(DRAW_NORMAL);
	setlinewidth(1);
	setfontsize(12);
	setlinestyle(DASHED);
	for (unsigned int i = 0; i < x_list.size(); i++) {
		x_list[i]->set_boolean(0);
		setcolor(DARKGREY);
		sprintf(msg, "%d", x_list[i]->get_index());
		drawtext(0.5 * resolution, (i + (float)0.5) * resolution, msg, resolution * (nv + 1));
		setcolor(LIGHTGREY);
		drawline(resolution, (i + (float)0.5) * resolution, resolution * (nv + 1), (i + (float)0.5) * resolution);
	}
	setlinestyle(SOLID);
	Node* root = new Node(new Variable(0), 0, 1); // create root node and implement it into the decision tree
	find_best(root, 0); 
	best = updt_best();
	end = clock(); // calculate the run time after finishing
	float runtime = (float)(end - start) / (float)CLOCKS_PER_SEC;
	setcolor(GREEN);
	if (best_d != 0) fillrect((best_x - (float)0.004 * nv) * resolution, (best_d + (float)0.5 - (float)0.004 * nv) * resolution, (best_x + (float)0.004 * nv) * resolution, (best_d + (float)0.5 + (float)0.004 * nv) * resolution);
	if (weighted) {
		cout << "\n\n[Weighted MAX-SAT]\nTotal weight: " << best << " \nVisited tree nodes: " << visited << " \nRun time: " << runtime << " seconds" << endl;
		sprintf(msg, "[Complete] Total weight: %d. Visited tree nodes: %d, Run time: %.2f seconds", best, visited, runtime);
	}
	else {
		cout << "\n\n[Unweighted MAX-SAT]\n\nSatisfied clauses: " << best << " \nVisited tree nodes: " << visited << " \nRun time: " << runtime << " seconds" << endl;
		sprintf(msg, "[Complete] Satisfied clauses: %d. Visited tree nodes: %d, Run time: %.2f seconds", best, visited, runtime);
	}
	update_message(msg);
	cout << "True/False assignment: ";
	for (unsigned int i = 1; i <= x_list.size(); i++) {
		Variable* xx = find_x(i);
		if (xx == NULL) return false;
		if (xx->get_best_boolean() == 0) return false;
		if ((i - 1) % 5 == 0) cout << "\n";
		cout << "x" << i;
		if (i < 10) cout << " ";
		if (xx->get_best_boolean() == 1) cout << ": True  ";
		else cout << ": False ";
	}
	cout << "\n\n---------------------------------- Complete --------------------------------\n" << endl;
	return true;
}

/* Implement the decision tree by recursively running the sequence */
void Maxsat::find_best(Node* current, float parent_x) {
	visited++;
	Variable* xn = current->get_x();
	int depth = xn->get_depth();
	int this_lb = updt_lb(depth);
	float current_x = current->get_loc() * nv / (float)pow(2, depth + 1) + 1;
	if (depth == 6) {
		int progress = int(current->get_loc() * 100 / (float)pow(2, depth + 1));
		sprintf(msg, "[%d%%] MAX-SAT optimizing...", progress);
		update_message(msg);
	}
	setcolor(DARKGREY);
	if (parent_x != 0.0) drawline(parent_x * resolution, (depth - (float)0.5) * resolution, current_x * resolution, (depth + (float)0.5) * resolution);
	setcolor(BLACK);
	if (this_lb >= lb) setcolor(RED);
	fillrect((current_x - (float)0.002 * nv) * resolution, (depth + (float)0.5 - (float)0.002 * nv) * resolution, (current_x + (float)0.002 * nv) * resolution, (depth + (float)0.5 + (float)0.002 * nv) * resolution);
	if (this_lb >= lb) { // pruned the node
		delete current;
		xn->set_boolean(0);
		return;
	}
	if (depth == x_list.size()) { // reach a leaf
		lb = this_lb;
		best_x = current_x;
		best_d = depth;
		for (unsigned int i = 0; i < x_list.size(); i++) { // update the boolean expression for best solution
			x_list[i]->set_best_boolean(x_list[i]->get_boolean());
		}
		delete current;
		xn->set_boolean(0);
		return;
	}
	Node* left_child = new Node(x_list[depth], -1, current->get_loc() * 2 - 1); 
	find_best(left_child, current_x); // continue recursion on the left child
	Node* right_child = new Node(x_list[depth], 1, current->get_loc() * 2 + 1);
	find_best(right_child, current_x); // continue recursion on the right child
	delete current;
	xn->set_boolean(0);
	return;
}

/* Update the lower bound */
int Maxsat::updt_lb(int dd) {
	int this_lb = 0;
	for (unsigned int i = 0; i < clause_list.size(); i++) {
		int unsatisfied = 1;
		for (unsigned int j = 0; j < clause_list[i]->to_disj().size(); j++) {
			if (clause_list[i]->to_disj()[j].first->get_boolean() * clause_list[i]->to_disj()[j].second == 1) {
				unsatisfied = 0;
				break;
			}
			else if (clause_list[i]->to_disj()[j].first->get_boolean() == 0) {             // bounding function:
				if (clause_list[i]->to_disj().size() > 2 || gen(rd) < 90 || dd > nv - 7) { // If the clause is related to less than 2 variables, and the
					unsatisfied = 0;                                                       // distance between current node and the leaf is equal or larger
					break;                                                                 // than seven, the undefined variable has 10% chance to be 
				}                                                                          // treated as false, otherwise it is treated as true.
			}
		}
		this_lb += unsatisfied * clause_list[i]->get_weight();
	}
	return this_lb;
}

/* Update the best */
int Maxsat::updt_best() {
	int this_best = 0;
	for (unsigned int i = 0; i < clause_list.size(); i++) {
		int satisfied = 0;
		for (unsigned int j = 0; j < clause_list[i]->to_disj().size(); j++) {
			if (clause_list[i]->to_disj()[j].first->get_best_boolean() * clause_list[i]->to_disj()[j].second == 1) {
				satisfied = 1;
				break;
			}
		}
		this_best += satisfied * clause_list[i]->get_weight();
	}
	return this_best;
}

Variable* Maxsat::find_x(int index) {
	auto i = x_map.find(index);
	if (i != x_map.end()) return i->second;
	else return NULL;
}

void Maxsat::set_weighted(bool mode) {
	weighted = mode;
}

int Maxsat::get_nv() {
	return nv;
}

bool Maxsat::get_weighted() {
	return weighted;
}

void plot_init(void) {
	float depth = (float)problem->get_nv();
	float window_size_x = resolution * (depth + 1);
	float window_size_y = resolution * (depth + 1);
	init_graphics("Branch-and-Bound Solver for MAX-SAT", WHITE);
	init_world(0., 0., window_size_x, window_size_y);
	create_button("Window", "Vanilla", plot_set_vanilla);
	create_button("Vanilla", "Weighted", plot_set_weighted);
	sprintf(msg, "Created by: Weixuan Yang @ University of Toronto");
	update_message(msg);
	set_draw_mode(DRAW_NORMAL);
	enable_or_disable_button(8, false);
}

void plot_message(void) {
	float depth = (float)problem->get_nv();
	clearscreen();
	setlinewidth(1);
	setfontsize(14);
	setlinestyle(SOLID);
	setcolor(LIGHTGREY);
	fillrect(resolution * (depth + 1) * (float)0.10, resolution * (depth + 1) * (float)0.4, resolution * (depth + 1) * (float)0.90, resolution * (depth + 1) * (float)0.6);
	setcolor(BLACK);
	if (!problem->get_weighted()) sprintf(msg, "[Unweighted MAX-SAT]");
	else sprintf(msg, "[Weighted MAX-SAT]");
	drawtext(resolution * (depth + 1) / (float)2.0, resolution * (depth + 1) * (float)0.45, msg, 2 * resolution * (depth + 1));
	sprintf(msg, "Press 'Proceed' to start optimizing.");
	drawtext(resolution * (depth + 1) / (float)2.0, resolution * ((depth + 1) / (float)2.0), msg, 2 * resolution * (depth + 1));
	if (!problem->get_weighted()) sprintf(msg, "Press 'Weighted' to switch to weighted MAX-SAT mode.");
	else sprintf(msg, "Press 'Vanilla' to switch back to unweighted MAX-SAT mode.");
	drawtext(resolution * (depth + 1) / (float)2.0, resolution * (depth + 1) * (float)0.55, msg, 2 * resolution * (depth + 1));
}

void plot_blank(void) {
}

void act_on_button_press(float x, float y) {
}

void plot_set_vanilla(void (*drawscreen_ptr) (void)) {
	problem->set_weighted(false);
	enable_or_disable_button(8, false);
	enable_or_disable_button(9, true);
	plot_message();
}

void plot_set_weighted(void (*drawscreen_ptr) (void)) {
	problem->set_weighted(true);
	enable_or_disable_button(8, true);
	enable_or_disable_button(9, false);
	plot_message();
}
