#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <deque>
using namespace std;

class Segment {
public:
	Segment(int x, int y, int w) {
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
	void label(int label) { label_ = label; }
	void set_available(bool is_available) { is_available_ = is_available; }
	void set_source(bool is_source, int sx, int sy, int spin) {
		is_source_ = is_source;
		sx_ = sx;
		sy_ = sy;
		spin_ = spin;
	}
	void set_sink(bool is_sink, int sx, int sy, int spin) {
		is_sink_ = is_sink;
		sx_ = sx;
		sy_ = sy;
		spin_ = spin;
	}
	void reset_segment() {
		sx_ = -1;
		sy_ = -1;
		spin_ = -1;
		label_ = -1;
		is_source_ = false;
		is_sink_ = false;
		is_available_ = true;
	}
	int get_x() { return x_; }
	int get_y() { return y_; }
	int get_w() { return w_; }
	int get_label(){ return label_; }
	bool get_source() { return is_source_; }
	int get_source_x() { return sx_; }
	int get_source_y() { return sy_; }
	int get_source_pin() { return spin_; }
	bool get_sink() { return is_sink_; }
	int get_sink_x() { return sx_; }
	int get_sink_y() { return sy_; }
	int get_sink_pin() { return spin_; }
	bool get_available() { return is_available_; }

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
	Net(int source_x, int source_y, int source_pin, int sink_x, int sink_y, int sink_pin) {
		source_x_ = source_x;
		source_y_ = source_y;
		source_pin_ = source_pin;
		sink_x_ = sink_x;
		sink_y_ = sink_y;
		sink_pin_ = sink_pin;
	}
	int net_get_source_x() { return source_x_; }
	int net_get_source_y() { return source_y_; }
	int net_get_source_pin() { return source_pin_; }
	int net_get_sink_x() { return sink_x_; }
	int net_get_sink_y() { return sink_y_; }
	int net_get_sink_pin() { return sink_pin_; }
	vector<Segment*>* to_route_list() { return &route_list; }

private:
	int source_x_;
	int source_y_;
	int source_pin_;
	int sink_x_;
	int sink_y_;
	int sink_pin_;
	vector<Segment*> route_list;
};
