/*
 * placer.h
 * Head file of placer.cpp
 *
 * Author:Weixuan Yang
 * Date: Oct 27, 2021
 */

#ifndef PLACER_H
#define PLACER_H

#include "graphics.h"
#include "segment.h"

class Placer {
public:
	Placer();
	~Placer();
	void placing();
	bool init_block();
	bool spread_block();	
	bool calc_weight();
	bool solv_weight(vector<vector<double>>& A, vector<double>& b_x, vector<double>& b_y);
	vector<Path*> find_path(Bin* bin, vector<Bin*>of_bins);
	void move_path(Path* path, vector<tuple<Block*, double, double> >& update_loc);
	double calc_cost(Bin* vsrc, Bin* vi);
	int calc_supply(int list_size);
	void calc_HPWL();
	void calc_disp();
	int find_block_by_distance(vector<int> block_list);
	bool find_overlap_bin(int x, int y, int block);
	Block* find_block(int index);
	Net* find_net(int index);
	Bin* find_bin(int x, int y);
	int get_grid_size();
	vector<Block*>& to_block_list();
	vector<Net*>& to_net_list();

private:;
	double psi;
	double WL;
	double disp;
	int it;
	int mode;
	int blocki;
	int neti;
	int blockx;
	int blocky;
	int gridsize;
	bool result;
	vector<Block*> block_list;
	map<int, Block*> block_map;
	vector<Net*> net_list;
	map<int, Net*> net_map;
	vector<Bin*> bin_list;
	vector<int> block_movable;
	vector<int> block_not_movable;
	vector<vector<double>>AA;
	vector<double>bx;
	vector<double>by;
};

void act_on_button_press(float x, float y);
void plot_init(void);
void plot_block(void);

#endif
