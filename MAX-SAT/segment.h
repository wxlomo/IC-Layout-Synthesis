/*
 * segment.h
 * Head file of segment.cpp
 *
 * Author:Weixuan Yang
 * Date: Nov 17, 2021
 */

#ifndef SEGMENT_H
#define SEGMENT_H

#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <math.h>
#include <time.h>
using namespace std;

class Variable {
public:
	Variable(int i);
	void set_boolean(int b);
	void set_best_boolean(int bb);
	void set_depth(int d);
	void set_supply();
	int get_index();
	int get_boolean();
	int get_best_boolean();
	int get_depth();
	int get_supply();

private:
	int index;
	int boolean;
	int best_boolean;
	int depth;
	int supply;
};

class Clause {
public:
	Clause();
	void set_weight(int w);
	int get_weight() const;
	vector<pair<Variable*, int>>& to_disj();
	struct clause_sort {
		bool operator()(const Clause* front, const Clause* back) {
			if (front->disj.size() == back->disj.size()) return front->get_weight() > back->get_weight();
			else return front->disj.size() < back->disj.size();
		}
	};
	struct x_sort {
		bool operator()(const pair<Variable*, int> front, const pair<Variable*, int> back) {
			return front.first->get_supply() < back.first->get_supply();
		}
	};

private:
	int weight;
	vector<pair<Variable*, int>> disj;
};

class Node {
public:
	Node(Variable* xn, int b, float location);
	void set_left_child(Node* left);
	void set_right_child(Node* right);
	float get_loc ();
	Variable* get_x();

private:
	float loc;
	Node* left_child;
	Node* right_child;
	Variable* x;
};

#endif
