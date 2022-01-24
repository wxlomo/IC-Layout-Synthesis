/*
 * main.cpp
 * A Branch-and-Bound Solver for MAX-SAT
 *	
 * Author:Weixuan Yang
 * Date: Nov 17, 2021
 */

#include "maxsat.h"
extern Maxsat* problem;

int main(int argc, char* argv[]) {
	bool weighted = false;

	cout << "\n----------------------------------------------------------------------------";
	cout << "\n|                            A MAX-SAT Solver                              |";
	cout << "\n|                         Create by Weixuan Yang                           |";
	cout << "\n|         weixuan.yang@mail.utoronto.ca / weixuanyang@outlook.com          |";
	cout << "\n----------------------------------------------------------------------------";
	if (argc == 1) {
		cerr << "\n\n* Please specify the name of the input file.\n" << endl;
		return 0;
	}
	string filename = argv[1];
	if (argc == 3) {
		if (strcmp(argv[2], "-w") == 0) {
			weighted = true;
		}
	}
	problem = new Maxsat(weighted);
	if (!problem->maxsat(filename)) cerr << "\n\n* Invalid input file, please check your input.\n" << endl;
	delete problem;
	return 0;
}

