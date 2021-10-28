/*
 * segment.cpp
 * Logic of data structures
 *
 * Author:Weixuan Yang
 * Date: Oct 27, 2021
 */

#include "segment.h"

Net::Net(int index) {
	index_ = index;
	weight_ = 0.0;
}

void Net::net_set_weight(double weight) {
	weight_ = weight;
}

int Net::net_get_index() {
	return index_;
}

double Net::net_get_weight() {
	return weight_; 
}

vector<int>& Net::to_net_block_list() {
	return net_block_list; 
}

Block::Block(int index) {
	index_ = index;
	blockx_ = 0.0;
	blocky_ = 0.0;
	block_oldx_ = 0.0;
	block_oldy_ = 0.0;
	distance_ = 0.0;
	isfixed_ = false;
}

void Block::set_fixed(bool isfixed) {
	isfixed_ = isfixed;
}

void Block::set_xy(double blockx, double blocky) {
	blockx_ = blockx;
	blocky_ = blocky;
}

void Block::set_old_xy(double block_oldx, double block_oldy) {
	block_oldx_ = block_oldx;
	block_oldy_ = block_oldy;
}

void Block::set_distance(double distance) {
	distance_ = distance;
}

int Block::block_get_index() { 
	return index_; 
}

double Block::block_get_x() {
	return blockx_; 
}

double Block::block_get_y() {
	return blocky_;
}

double Block::block_get_old_x() {
	return block_oldx_;
}

double Block::block_get_old_y() {
	return block_oldy_;
}

double Block::block_get_distance() {
	return distance_;
}

bool Block::block_get_fixed() {
	return isfixed_; 
}

vector<int>& Block::to_block_net_list() {
	return block_net_list; 
}

Bin::Bin(int x, int y) {
	x_ = x;
	y_ = y;
	isoverlap_ = false;
	isvisited_ = false;
}

void Bin::set_overlap(bool isoverlap) {
	isoverlap_ = isoverlap;
}
void Bin::set_visited(bool isvisited) {
	isvisited_ = isvisited;
}

int Bin::bin_get_x() {
	return x_;
}

int Bin::bin_get_y() {
	return y_;
}

bool Bin::bin_get_overlap() {
	return isoverlap_;
}

bool Bin::bin_get_visited() {
	return isvisited_; 
}

vector<int>& Bin::to_bin_block_list() {
	return bin_block_list;
}

Path::Path(vector<Bin*>path, double cost) {
	cost_ = cost;
	path_bins = path;
}

double Path::path_get_cost() const { 
	return cost_;
}

vector<Bin*>& Path::to_path_bins() {
	return path_bins;
}
