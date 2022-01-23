/*
 * segment.h
 * Head file of segment.cpp
 *
 * Author:Weixuan Yang
 * Date: Oct 27, 2021
 */

#ifndef SEGMENT_H
#define SEGMENT_H

#include <math.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <stack>
#include <tuple>
using namespace std;

/* Constants */
const int it_spr = 200;            // Max Iteration of spreading
const int kpsi = 1;                // Factor k for updating Psi
const int omega = 1;               // Max block number in a bin
const float resolution = 1000.0;   // Resolution of the plot
//bool debug = true;               // Generate log for the process

class Net {
public:
	Net(int index);
	void net_set_weight(double weight);
	int net_get_index();
	double net_get_weight();
	vector<int>& to_net_block_list();

private:
	int index_;
	double weight_;
	vector<int> net_block_list;
};

class Block {
public:
	Block(int index);
	void set_fixed(bool isfixed);
	void set_xy(double blockx, double blocky);
	void set_pseudo_xy(double blockx, double blocky);
	void set_distance(double distance);
	int block_get_index();
	double block_get_x();
	double block_get_y();
	double block_get_pseudo_x();
	double block_get_pseudo_y();
	double block_get_distance();
	bool block_get_fixed();
	vector<int>& to_block_net_list();

private:
	int index_;
	double blockx_;
	double blocky_;
	double block_pseudox_;
	double block_pseudoy_;
	double distance_;
	bool isfixed_;
	vector<int> block_net_list;
};

class Bin {
public:
	Bin(int x, int y);
	void set_overlap(bool isoverlap);
	void set_visited(bool isvisited);
	int bin_get_x();
	int bin_get_y();
	int bin_calc_supply() const;
	bool bin_get_overlap();
	bool bin_get_visited();
	vector<int>& to_bin_block_list();
	struct bin_supply {
		bool operator()(const Bin* front, const Bin* back) {
			return front->bin_calc_supply() < back->bin_calc_supply();
		}
	};

	struct bin_supply_reversed {
		bool operator()(const Bin* front, const Bin* back) {
			return front->bin_calc_supply() > back->bin_calc_supply();
		}
	};

private:
	int x_;
	int y_;
	bool isoverlap_;
	bool isvisited_;
	vector<int> bin_block_list;
};

class Path {
public:
	Path(vector<Bin*>path, double cost);
	double path_get_cost() const;
	vector<Bin*>& to_path_bins();
	struct path_cost {
		bool operator()(const Path* front, const Path* back) {
			return front->path_get_cost() < back->path_get_cost();
		}
	};

private:
	double cost_;
	vector<Bin*>path_bins;
};

#endif
