#include <stdio.h>
#include "router.h"
#include "graphics.h"

extern int segments = 0;
extern bool mode = false;
extern bool debug = false; // start with debug mode

const float resolution = 100;
int width_ = -1;
int grid_size_ = -1;
int grid_type_ = -1;
Router* circuit_;

void plot(Router* circuit, int width, int grid_size, int grid_type);
void plot_circuit(void);
void act_on_button_press(float x, float y);

void plot(Router* circuit, int width, int grid_size, int grid_type) {
	width_ = width;
	grid_size_ = grid_size;
	grid_type_ = grid_type;
	circuit_ = circuit;
	float window_size_x = resolution * (2 * grid_size_ + 1);
	float window_size_y = resolution * (2 * grid_size_ + 1);
	init_world(0., 0., window_size_x, window_size_y);
	init_graphics("Maze Router", WHITE);
	update_message("A Maze Router - by Weixuan Yang @University of Toronto");
	clearscreen();
	//create_button("Window", "All Net", act_on_new_button_func);
	event_loop(act_on_button_press, NULL, NULL, plot_circuit);
	close_graphics();
}

void plot_circuit(void) {
	set_draw_mode(DRAW_NORMAL);
	clearscreen();
	setlinewidth(1);
	setcolor(BLACK);
	setfontsize(8);
	setlinestyle(SOLID);
	float linegap1 = 0.5 / (width_ - 1);
	float linegap2 = 0.2 / (width_ - 1);
	int source_x = -1;
	int source_y = -1;
	int source_pin = -1;
	int sink_x = -1;
	int sink_y = -1;
	int sink_pin = -1;
	int color_index = 4;

	for (int x = 0; x < grid_size_; x++) { // draw logic blocks
		for (int y = 0; y < grid_size_; y++) {
			setcolor(BLACK);
			drawline((resolution * (2 * x + 1)), (resolution * (2 * y + 1)), (resolution * (2 * x + 1)), (resolution * (2 * y + 2)));
			drawline((resolution * (2 * x + 1)), (resolution * (2 * y + 1)), (resolution * (2 * x + 2)), (resolution * (2 * y + 1)));
			drawline((resolution * (2 * x + 1)), (resolution * (2 * y + 2)), (resolution * (2 * x + 2)), (resolution * (2 * y + 2)));
			drawline((resolution * (2 * x + 2)), (resolution * (2 * y + 1)), (resolution * (2 * x + 2)), (resolution * (2 * y + 2)));
			drawline((resolution * (2 * x + 1.6)), (resolution * (2 * y + 2)), (resolution * (2 * x + 1.6)), (resolution * (2 * y + 2.75))); // pin 1
			drawline((resolution * (2 * x + 1)), (resolution * (2 * y + 1.4)), (resolution * (2 * x + 0.25)), (resolution * (2 * y + 1.4))); // pin 2
			drawline((resolution * (2 * x + 1.4)), (resolution * (2 * y + 1)), (resolution * (2 * x + 1.4)), (resolution * (2 * y + 0.25))); // pin 3
			drawline((resolution * (2 * x + 2)), (resolution * (2 * y + 1.6)), (resolution * (2 * x + 2.75)), (resolution * (2 * y + 1.6))); // pin 4
			char loc[10];
			sprintf(loc, "%d %d", x, (grid_size_ - y - 1));
			drawtext((resolution * (2 * x + 1.5)), (resolution * (2 * y + 2.1)), loc, 400.);
			setcolor(LIGHTGREY);
			fillrect((resolution * (2 * x + 1)), (resolution * (2 * y + 1)), (resolution * (2 * x + 2)), (resolution * (2 * y + 2)));
		}
	}
	setcolor(BLACK);
	for (int x = 0; x < grid_size_ + 1; x++) { // draw switch blocks
		for (int y = 0; y < grid_size_ + 1; y++) {	
			switch (grid_type_) {
			case 1:
				drawline(resolution * (2 * x + 0.25), (resolution * (2 * y + 0.25)), (resolution * (2 * x + 0.25)), (resolution * (2 * y + 0.75)));
				drawline(resolution * (2 * x + 0.25), (resolution * (2 * y + 0.25)), (resolution * (2 * x + 0.75)), (resolution * (2 * y + 0.25)));
				drawline(resolution * (2 * x + 0.25), (resolution * (2 * y + 0.75)), (resolution * (2 * x + 0.75)), (resolution * (2 * y + 0.75)));
				drawline(resolution * (2 * x + 0.75), (resolution * (2 * y + 0.25)), (resolution * (2 * x + 0.75)), (resolution * (2 * y + 0.75)));
				break;
			case 2:
				drawline(resolution * (2 * x + 0.4), (resolution * (2 * y + 0.2)), (resolution * (2 * x + 0.2)), (resolution * (2 * y + 0.4)));
				drawline(resolution * (2 * x + 0.2), (resolution * (2 * y + 0.4)), (resolution * (2 * x + 0.2)), (resolution * (2 * y + 0.6)));
				drawline(resolution * (2 * x + 0.2), (resolution * (2 * y + 0.6)), (resolution * (2 * x + 0.4)), (resolution * (2 * y + 0.8)));
				drawline(resolution * (2 * x + 0.4), (resolution * (2 * y + 0.8)), (resolution * (2 * x + 0.6)), (resolution * (2 * y + 0.8)));
				drawline(resolution * (2 * x + 0.6), (resolution * (2 * y + 0.8)), (resolution * (2 * x + 0.8)), (resolution * (2 * y + 0.6)));
				drawline(resolution * (2 * x + 0.8), (resolution * (2 * y + 0.6)), (resolution * (2 * x + 0.8)), (resolution * (2 * y + 0.4)));
				drawline(resolution * (2 * x + 0.8), (resolution * (2 * y + 0.4)), (resolution * (2 * x + 0.6)), (resolution * (2 * y + 0.2)));
				drawline(resolution * (2 * x + 0.6), (resolution * (2 * y + 0.2)), (resolution * (2 * x + 0.4)), (resolution * (2 * y + 0.2)));
				break;
			}
			
		}
	}
	setcolor(LIGHTGREY);
	setlinewidth(0.5);
	for (int x = 0; x < grid_size_; x++) { // draw horizontal segments
		for (int y = 0; y < grid_size_ + 1; y++) {
			for(int w = 0; w < width_; w++){
				switch (grid_type_) {
				case 1:
					drawline(resolution * (2 * x + 0.75), (resolution * (2 * y + 0.75 - w * linegap1)), (resolution * (2 * x + 2.25)), (resolution * (2 * y + 0.75 - w * linegap1)));
					break;
				case 2:
					drawline(resolution * (2 * x + 0.8), (resolution * (2 * y + 0.6 - w * linegap2)), (resolution * (2 * x + 2.2)), (resolution * (2 * y + 0.6 - w * linegap2)));
					break;
				}
			}
		}
	}
	for (int x = 0; x < grid_size_ + 1; x++) { // draw vertical segments
		for (int y = 0; y < grid_size_; y++) {
			for (int w = 0; w < width_; w++) {
				switch (grid_type_) {
				case 1:
					drawline(resolution * (2 * x + 0.25 + w * linegap1), (resolution * (2 * y + 0.75)), (resolution * (2 * x + 0.25 + w * linegap1)), (resolution * (2 * y + 2.25)));
					break;
				case 2:
					drawline(resolution * (2 * x + 0.4 + w * linegap2), (resolution * (2 * y + 0.8)), (resolution * (2 * x + 0.4 + w * linegap2)), (resolution * (2 * y + 2.2)));
					break;
				}
			}
		}
	}
	for (int x = 0; x < grid_size_; x++) { // draw diagonal segments
		for (int y = 0; y < grid_size_; y++) {
			for (int w = 0; w < width_; w++) {
				switch (grid_type_) {
				case 1:
					break;
				case 2:
					drawline(resolution * (2 * x + 0.8 - w * linegap2), (resolution * (2 * y + 0.6 + w * linegap2)), (resolution * (2 * x + 2.4 - w * linegap2)), (resolution * (2 * y + 2.2 + w * linegap2)));
					drawline(resolution * (2 * x + 2.2 + w * linegap2), (resolution * (2 * y + 0.6 + w * linegap2)), (resolution * (2 * x + 0.6 + w * linegap2)), (resolution * (2 * y + 2.2 + w * linegap2)));
					break;
				}
			}
		}
	}

	for (unsigned int i = 0; i < circuit_->to_net_list()->size(); i++) { // draw each net in the net list
		setcolor(color_index);
		color_index++;
		if (color_index > 10) color_index = 4; // randomly choose a color for each net, ignore grey, black, and white
		source_x = circuit_->to_net_list()->at(i)->net_get_source_x();
		source_y = grid_size_ - 1 - circuit_->to_net_list()->at(i)->net_get_source_y();
		source_pin = circuit_->to_net_list()->at(i)->net_get_source_pin();
		sink_x = circuit_->to_net_list()->at(i)->net_get_sink_x();
		sink_y = grid_size_ - 1 - circuit_->to_net_list()->at(i)->net_get_sink_y();
		sink_pin = circuit_->to_net_list()->at(i)->net_get_sink_pin();
		fillrect((resolution * (2 * source_x + 1)), (resolution * (2 * source_y + 1)), (resolution * (2 * source_x + 2)), (resolution * (2 * source_y + 2))); // paint the source block
		setlinewidth(1);
		switch (source_pin) {
		case 1:
			drawline((resolution * (2 * source_x + 1.6)), (resolution * (2 * source_y + 2)), (resolution * (2 * source_x + 1.6)), (resolution * (2 * source_y + 2.75)));
			break;
		case 2:
			drawline((resolution * (2 * source_x + 1)), (resolution * (2 * source_y + 1.4)), (resolution * (2 * source_x + 0.25)), (resolution * (2 * source_y + 1.4)));
			break;
		case 3:
			drawline((resolution * (2 * source_x + 1.4)), (resolution * (2 * source_y + 1)), (resolution * (2 * source_x + 1.4)), (resolution * (2 * source_y + 0.25)));
			break;
		case 4:
			drawline((resolution * (2 * source_x + 2)), (resolution * (2 * source_y + 1.6)), (resolution * (2 * source_x + 2.75)), (resolution * (2 * source_y + 1.6)));
			break;
		}
		fillrect((resolution * (2 * sink_x + 1)), (resolution * (2 * sink_y + 1)), (resolution * (2 * sink_x + 2)), (resolution * (2 * sink_y + 2))); // paint the source block
		switch (sink_pin) {
		case 1:
			drawline((resolution * (2 * sink_x + 1.6)), (resolution * (2 * sink_y + 2)), (resolution * (2 * sink_x + 1.6)), (resolution * (2 * sink_y + 2.75)));
			break;
		case 2:
			drawline((resolution * (2 * sink_x + 1)), (resolution * (2 * sink_y + 1.4)), (resolution * (2 * sink_x + 0.25)), (resolution * (2 * sink_y + 1.4)));
			break;
		case 3:
			drawline((resolution * (2 * sink_x + 1.4)), (resolution * (2 * sink_y + 1)), (resolution * (2 * sink_x + 1.4)), (resolution * (2 * sink_y + 0.25)));
			break;
		case 4:
			drawline((resolution * (2 * sink_x + 2)), (resolution * (2 * sink_y + 1.6)), (resolution * (2 * sink_x + 2.75)), (resolution * (2 * sink_y + 1.6)));
			break;
		}
		setlinewidth(0.5);
		for (unsigned int n = 0; n < circuit_->to_net_list()->at(i)->to_route_list()->size(); n++) { // draw route	
			unsigned int x = circuit_->to_net_list()->at(i)->to_route_list()->at(n)->get_x();
			unsigned int y = circuit_->to_net_list()->at(i)->to_route_list()->at(n)->get_y();
			unsigned int w = circuit_->to_net_list()->at(i)->to_route_list()->at(n)->get_w();
			if (debug) { cout << n << " x= " << x << " y= " << y << " w= " << w << " i= " << i << endl; };
			switch (grid_type_) {
			case 1:
				if (x % 2 == 0) { // vertical
					drawline((resolution * (x + 0.25 + w * linegap1)), (resolution* ((2 * (grid_size_ - 1 - y)) + 0.75)), (resolution* (x + 0.25 + w * linegap1)), (resolution* ((2 * (grid_size_ - 1 - y)) + 2.25)));
				}
				else { // horizontal
					drawline((resolution * (x - 0.25)), (resolution * (2 * (grid_size_ - y) + 0.25 + w * linegap1)), (resolution * (x + 1.25)), (resolution * (2 * (grid_size_ - y) + 0.25 + w * linegap1)));
				}
				break;
			case 2:
				if (x % 2 == 0) { // vertical
					drawline((resolution * (x + 0.4 + w * linegap2)), (resolution * ((2 * (grid_size_ - 1 - y)) + 0.8)), (resolution * (x + 0.4 + w * linegap2)), (resolution * ((2 * (grid_size_ - 1 - y)) + 2.2)));
				}
				else if (y <= grid_size_){ // horizontal
					drawline((resolution * (x - 0.2)), (resolution * (2 * (grid_size_ - y) + 0.4 + w * linegap2)), (resolution * (x + 1.2)), (resolution * (2 * (grid_size_ - y) + 0.4 + w * linegap2)));
				}
				else { // diagonal
					if (y % 2 == 0) { // upper right to bottom left
						drawline((resolution* (x - 0.2 - w * linegap2)), (resolution* (2 * (grid_size_ - 1 - (y - grid_size_) / 2) + 0.6 + w * linegap2)), (resolution* (x + 1.4 - w * linegap2)), (resolution* (2 * (grid_size_ - 1 - (y - grid_size_) / 2) + 2.2 + w * linegap2)));
					}
					else { // bottom right to upper left
						drawline((resolution* (x - 0.2 - w * linegap2)), (resolution* (2 * (grid_size_ - 1 - (y - grid_size_) / 2) + 2.4 - w * linegap2)), (resolution* (x + 1.4 - w * linegap2)), (resolution* (2 * (grid_size_ - 1 - (y - grid_size_) / 2) + 0.8 - w * linegap2)));
					}
				}

			}
				
		}	
	}
}

void act_on_button_press(float x, float y) {
	clearscreen();
	plot_circuit();
	update_message("Routing plot refreshed.");
}
