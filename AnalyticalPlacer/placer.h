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
	bool set_weight();
	bool update_block(vector<vector<double>>& A, vector<double>& b_x, vector<double>& b_y);
	vector<Path*> find_path(Bin* bin, vector<Bin*>of_bins);
	void move_path(Path* path, vector<tuple<Block*, double, double> >& update_loc);
	void umfsolver(vector<double>& x, int n, vector<int>& ja, vector<int>& ia, vector<double>& a, vector<double>& b);
	double calc_cost(Bin* vsrc, Bin* vi);
	void calc_HPWL();
	void calc_disp(Block* block, double x, double y);
	int find_block_by_distance(vector<int> block_list);
	bool find_overlap_bin(int x, int y, int block);
	Block* find_block(int index);
	Net* find_net(int index);
	Bin* find_bin(int x, int y);
	int get_grid_size();
	vector<Block*>& to_block_list();
	vector<Net*>& to_net_list();

	bool linear_psi;
	bool reversed_bins;
	bool strong_anchor;
	bool weak_anchor;

private:;
	double psi;
	double WL;
	double disp;
	int it;
	int blocki;
	int neti;
	int blockx;
	int blocky;
	int gridsize;
	bool result;
	bool error;
	vector<Block*> block_list;
	map<int, Block*> block_map;
	vector<Net*> net_list;
	map<int, Net*> net_map;
	vector<Bin*> bin_list;
	vector<int> block_movable;
	vector<int> block_not_movable;
	vector<vector<double>>Qij;
	vector<double>ci;
	vector<double>di;
};

void init_plot(void);
void block_plot(void);
void act_on_button_press(float x, float y);
void set_default_plot(void (*drawscreen_ptr) (void));
void set_linear_psi_plot(void (*drawscreen_ptr) (void));
void set_reversed_bins_plot(void (*drawscreen_ptr) (void));
void set_strong_anchor_plot(void (*drawscreen_ptr) (void));
void set_weak_anchor_plot(void (*drawscreen_ptr) (void));
void show_block_index(void (*drawscreen_ptr) (void));

#endif
