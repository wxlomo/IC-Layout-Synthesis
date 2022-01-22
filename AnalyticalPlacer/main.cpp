/*
 * main.cpp
 * Analytical Placer
 *
 * Author:Weixuan Yang
 * Date: Oct 27, 2021
 */

#include "placer.h"
extern Placer* circuit;

int main() {
	cout << "----------------------------------------------------------------------------\n";
	cout << "|                          An Analytical Placer                            |\n";
	cout << "|                         Weixuan Yang s1008655947                         |\n";
	cout << "|                      weixuan.yang@mail.utoronto.ca                       |\n";
	cout << "----------------------------------------------------------------------------\n";
	cout << "\nWelcome!" << endl;
	circuit = new Placer();
	circuit->placing();
	delete circuit;
	return 0;
}

