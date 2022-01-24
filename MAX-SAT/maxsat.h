/*
 * maxsat.h
 * Head file of maxsat.cpp
 *
 * Author:Weixuan Yang
 * Date: Nov 17, 2021
 */

#ifndef MAXSAT_H
#define MAXSAT_H

#include "graphics.h"
#include "segment.h"

class Maxsat {
public:
	Maxsat(bool wit);
	~Maxsat();
	bool maxsat(string filename);
	bool init();
	bool optm();
	void find_best(Node* current, float parent_x);
	int updt_lb(int dd);
	int updt_best();
	Variable* find_x(int index);
	void set_weighted(bool mode);
	int get_nv();
	bool get_weighted();

private:
	int nv;
	int nc;
	int lb;
	int best;
	int visited;
	int best_d;
	float best_x;
	clock_t start;
	clock_t end;
	bool weighted;
	vector<Variable*> x_list;
	map<int, Variable*> x_map;
	vector<Clause*> clause_list;
};

void plot_init(void);
void plot_message(void);
void plot_blank(void);
void act_on_button_press(float x, float y);

#endif
