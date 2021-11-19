/*
 * segment.cpp
 * Logic of data structures
 *
 * Author:Weixuan Yang
 * Date: Nov 17, 2021
 */

#include "segment.h"

Variable::Variable(int i) {
	index = i;
	boolean = 0;
	best_boolean = 0;
	depth = 0;
	supply = 0;
}

void Variable::set_boolean(int b) {
	boolean = b;
}

void Variable::set_best_boolean(int bb) {
	best_boolean = bb;
}

void Variable::set_depth(int d) {
	depth = d;
}

void Variable::set_supply() {
	supply += 1;
}

int Variable::get_index() {
	return index;
}

int Variable::get_boolean() {
	return boolean;
}

int Variable::get_best_boolean() {
	return best_boolean;
}

int Variable::get_depth() {
	return depth;
}

int Variable::get_supply() {
	return supply;
}

Clause::Clause() {
	weight = 1;
}

void Clause::set_weight(int w) {
	weight = w;
}

int Clause::get_weight() const {
	return weight;
}

vector<pair<Variable*, int>>& Clause::to_disj() {
	return disj;
}

Node::Node(Variable* xn, int b, float location) {
	x = xn;
	x->set_boolean(b);
	loc = location;
	left_child = NULL;
	right_child = NULL;
}

void Node::set_left_child(Node* left) {
	left_child = left;
}
void Node::set_right_child(Node* right) {
	right_child = right;
}
float Node::get_loc() {
	return loc;
}
Variable* Node::get_x() {
	return x;
}