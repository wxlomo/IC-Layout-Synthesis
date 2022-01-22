/*
 * router.h
 * Head file of router.cpp
 *
 * Author:Weixuan Yang
 * Date: Oct 08, 2021
 */

#ifndef ROUTER_H
#define ROUTER_H

#include "segment.h"
#include "graphics.h"

static char buffer[1024] = { 0 };
extern int segments;

class Router {
public:
	Router(int grid_size, int width, int grid_type);
	~Router();
	vector<Segment*> find_adjacent(Segment* segment);
	vector<Net*>* to_net_list();
	bool maze_routing();

private:
	int grid_size_;
	int width_;
	int grid_type_;
	int source_x_;
	int source_y_;
	int source_pin_;
	int sink_x_;
	int sink_y_;
	int sink_pin_;
	int xx;
	int yy;
	int ww;
	vector<Net*>net_list;
	queue<Segment*>expansion_list;
	vector<vector<vector<Segment*>>>grid_map;
};

void plot(Router* circuit, int width, int grid_size, int grid_type);
void plot_circuit(void);
void act_on_button_press(float x, float y);

#endif