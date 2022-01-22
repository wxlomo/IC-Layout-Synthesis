/*
 * segment.cpp
 * Logic of data structures
 *
 * Author:Weixuan Yang
 * Date: Oct 08, 2021
 */

#include "segment.h"

Segment::Segment(int x, int y, int w) {
	x_ = x;
	y_ = y;
	w_ = w;
	sx_ = -1;
	sy_ = -1;
	spin_ = -1;
	label_ = -1; // T = 0, L = 1
	is_source_ = false;
	is_sink_ = false;
	is_available_ = true;
}

void Segment::label(int label) { 
	label_ = label; 
}

void Segment::set_available(bool is_available) { 
	is_available_ = is_available; 
}

void Segment::set_source(bool is_source, int sx, int sy, int spin) {
	is_source_ = is_source;
	sx_ = sx;
	sy_ = sy;
	spin_ = spin;
}

void Segment::set_sink(bool is_sink, int sx, int sy, int spin) {
	is_sink_ = is_sink;
	sx_ = sx;
	sy_ = sy;
	spin_ = spin;
}

void Segment::reset_segment() {
	sx_ = -1;
	sy_ = -1;
	spin_ = -1;
	label_ = -1;
	is_source_ = false;
	is_sink_ = false;
	is_available_ = true;
}

int Segment::get_x() { 
	return x_; 
}

int Segment::get_y() { 
	return y_; 
}

int Segment::get_w() { 
	return w_; 
}

int Segment::get_label() { 
	return label_; 
}

bool Segment::get_source() { 
	return is_source_;
}

int Segment::get_source_x() { 
	return sx_;
}

int Segment::get_source_y() {
	return sy_; 
}

int Segment::get_source_pin() { 
	return spin_; 
}

bool Segment::get_sink() { 
	return is_sink_; 
}

int Segment::get_sink_x() { 
	return sx_; 
}

int Segment::get_sink_y() { 
	return sy_;
}

int Segment::get_sink_pin() { 
	return spin_; 
}

bool Segment::get_available() { 
	return is_available_; 
}

Net::Net(int source_x, int source_y, int source_pin, int sink_x, int sink_y, int sink_pin) {
	source_x_ = source_x;
	source_y_ = source_y;
	source_pin_ = source_pin;
	sink_x_ = sink_x;
	sink_y_ = sink_y;
	sink_pin_ = sink_pin;
}
int Net::net_get_source_x() { 
	return source_x_;
}

int Net::net_get_source_y() { 
	return source_y_; 
}

int Net::net_get_source_pin() { 
	return source_pin_; 
}

int Net::net_get_sink_x() { 
	return sink_x_; 
}

int Net::net_get_sink_y() { 
	return sink_y_; 
}

int Net::net_get_sink_pin() { 
	return sink_pin_; 
}

vector<Segment*>* Net::to_route_list() { 
	return &route_list;
}
