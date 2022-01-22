/*
 * segment.h
 * Head file of segment.cpp
 *
 * Author:Weixuan Yang
 * Date: Oct 08, 2021
 */

#ifndef SEGMENT_H
#define SEGMENT_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

class Segment {
public:
	Segment(int x, int y, int w);
	void label(int label);
	void set_available(bool is_available);
	void set_source(bool is_source, int sx, int sy, int spin);
	void set_sink(bool is_sink, int sx, int sy, int spin);
	void reset_segment();
	int get_x();
	int get_y();
	int get_w();
	int get_label();
	bool get_source();
	int get_source_x();
	int get_source_y();
	int get_source_pin();
	bool get_sink();
	int get_sink_x();
	int get_sink_y();
	int get_sink_pin();
	bool get_available();

private:
	int x_;
	int y_;
	int w_;
	int sx_;
	int sy_;
	int spin_;
	int label_;
	bool is_source_;
	bool is_sink_;
	bool is_available_;
};

class Net {
public:
	Net(int source_x, int source_y, int source_pin, int sink_x, int sink_y, int sink_pin);
	int net_get_source_x();
	int net_get_source_y();
	int net_get_source_pin();
	int net_get_sink_x();
	int net_get_sink_y();
	int net_get_sink_pin();
	vector<Segment*>* to_route_list();

private:
	int source_x_;
	int source_y_;
	int source_pin_;
	int sink_x_;
	int sink_y_;
	int sink_pin_;
	vector<Segment*> route_list;
};

#endif