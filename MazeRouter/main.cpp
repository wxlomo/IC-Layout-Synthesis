/*
 * main.cpp
 * A Maze router
 *
 * Author:Weixuan Yang
 * Date: Oct 08, 2021
 */

#include "router.h"
extern Router* circuit;

int main(int argc, char* argv[]) {
	string filename;
	ifstream filein;
	int grid_size = -1;
	int grid_type = 1;
	int w_max = -1;
	int width = 1;
	int source_x = -1;
	int source_y = -1;
	int source_pin = -1;
	int sink_x = -1;
	int sink_y = -1;
	int sink_pin = -1;
	bool get_result = false;
	bool mode = false;

	cout << "\n----------------------------------------------------------------------------";
	cout << "\n|                              A Maze Router                               |";
	cout << "\n|                         Create by Weixuan Yang                           |";
	cout << "\n|         weixuan.yang@mail.utoronto.ca / weixuanyang@outlook.com          |";
	cout << "\n----------------------------------------------------------------------------";
	if (argc == 1) {
		cerr << "\n\n* Please specify the name of the input file.\n" << endl;
		return -1;
	}
	filename = argv[1];
	filein.open(filename, ios::in);
	if (filein.is_open()) cout << "\n\n" << filename << " is opened successfully. ";
	else {
		cerr << "\n\n* Invalid input file, please check your input.\n" << endl;
		return -1;
	}
	if (argc > 2) {
		for (int i = 2; i < argc; i++) {
			if (strcmp(argv[i], "-oct") == 0) {
				grid_type = 2;
				cout << "[Octagon switch blocks]";
			}
			if (strcmp(argv[i], "-min") == 0) {
				mode = true;
				cout << "[Minimize channel width]";
			}
		}
	}
	if (grid_type == 1) cout << "[Square switch blocks]";
	if (!mode) cout << "[Routing only]";
	cout << endl;
	
	filein.getline(buffer, sizeof(buffer));
	sscanf(buffer, "%d", &grid_size);
	filein.getline(buffer, sizeof(buffer));
	sscanf(buffer, "%d", &w_max);
	filein.close();
	if (grid_size == -1 || w_max == -1) {
		cerr << "\n\n* Invalid input file, please check your input.\n" << endl;
		return -1;
	}
	if (!mode) cout << "\n-------------------------- Routing, please wait... -------------------------\n\n";
	else cout << "\n--------------- Finding minimum channel width, please wait... --------------\n\n";
	Router* circuit = NULL;
	for (width = w_max; width >= 0; width--) {
		if(get_result){
			mode = false;
		}
		delete circuit;
		filein.open(filename, ios::in);
		filein.getline(buffer, sizeof(buffer));
		filein.getline(buffer, sizeof(buffer));
		Router* circuit = new Router(grid_size, width, grid_type);
		while (filein.getline(buffer, sizeof(buffer))) {
			source_x = source_y = source_pin = sink_x = sink_y = sink_pin = -1;
			sscanf(buffer, "%d %d %d %d %d %d ", &source_x, &source_y, &source_pin, &sink_x, &sink_y, &sink_pin);
			if (source_x == -1 && source_y == -1 && source_pin == -1 && sink_x == -1 && sink_y == -1 && sink_pin == -1) {
				break;
			}
			circuit -> to_net_list() -> push_back(new Net(source_x, source_y, source_pin, sink_x, sink_y, sink_pin));
		}
		filein.close();
		bool flag = circuit -> maze_routing();
		if (!flag) {
			if (width == w_max) {
				cout << "\n* Routing failed.\n\n";
				break;
			}
			else {
				cout << "The minimum channel width is " << width + 1 << " and ";
				get_result = true;
				width = width + 2;
			}
		}
		if (!mode) {
			cout << segments << " segments are used in routing.\n\n";
			cout << "--------------------------------- Complete ---------------------------------\n" << endl;
			plot(circuit, width, grid_size, grid_type);
			break;
		}	
	}
	return 0;
}

