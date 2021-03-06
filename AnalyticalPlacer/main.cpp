/*
 * main.cpp
 * Analytical Placer
 *
 * Author:Weixuan Yang
 * Date: Oct 27, 2021
 */

#include "placer.h"
extern Placer* circuit;

int main(int argc, char* argv[]) {
	cout << "\n----------------------------------------------------------------------------";
	cout << "\n|                          An Analytical Placer                            |";
	cout << "\n|                         Create by Weixuan Yang                           |";
	cout << "\n|         weixuan.yang@mail.utoronto.ca / weixuanyang@outlook.com          |";
	cout << "\n----------------------------------------------------------------------------";
	if (argc == 1) {
		cerr << "\n\n* Please specify the name of the input file.\n" << endl;
		return 0;
	}
	string filename = argv[1];
	circuit = new Placer();\
	if (!circuit->placing(filename)) cerr << "\n\n* Invalid input file, please check your input.\n" << endl;
	delete circuit;
	return 0;
}

