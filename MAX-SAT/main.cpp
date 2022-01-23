/*
 * main.cpp
 * A MAX-SAT Solver
 *	
 * Author:Weixuan Yang
 * Date: Nov 17, 2021
 */

#include "maxsat.h"
extern Maxsat* problem;

int main(int argc, char* argv[]) {
	cout << "----------------------------------------------------------------------------\n";
	cout << "|                            A MAX-SAT Solver                              |\n";
	cout << "|                         Create by Weixuan Yang                           |\n";
	cout << "|         weixuan.yang@mail.utoronto.ca / weixuanyang@outlook.com          |\n";
	cout << "----------------------------------------------------------------------------\n";
	if (argc == 1) {
		cerr << "\n\n* Please specify the name of the input file.\n" << endl;
		return 0;
	}
	string filename = argv[1];
	problem = new Maxsat();
	if (!problem->maxsat(filename)) cerr << "\n\n* Invalid input file, please check your input.\n" << endl;
	delete problem;
	return 0;
}

