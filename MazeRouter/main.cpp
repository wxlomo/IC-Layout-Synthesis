#include "plot.h"

int main() {
	string filename;
	ifstream filein;
	int grid_size = -1;
	int grid_type = -1;
	int w_max = -1;
	int width = 1;
	char mode_input;
	char debug_input;
	int source_x = -1;
	int source_y = -1;
	int source_pin = -1;
	int sink_x = -1;
	int sink_y = -1;
	int sink_pin = -1;
	bool get_result = false;
	result_type routing_result;

	cout << "----------------------------------------------------------------------------\n";
	cout << "|                          ECE1387H Assignment 1                           |\n";
	cout << "|                              A Maze Router                               |\n";
	cout << "|                         Weixuan Yang s100865594                          |\n";
	cout << "|                      weixuan.yang@mail.utoronto.ca                       |\n";
	cout << "----------------------------------------------------------------------------\n";
	cout << "\nWelcome!\n\n";
	for (;;) {
		cout << "Please enter the name of the input file:\n";
		getline(cin, filename);
		filein.open(filename, ios::in);
		if (!filein.is_open()) {
			cerr << "\nThe file you entered does not exist, please try again.\n\n";
		}
		else {
			//cout << "\nInput file " << filename << " is opened.\n" << endl;
			break;
		}
	}

	for (;;) {
		cout << "\nPlease select the style of the switch blocks: \n[1] Square     [2] Octagon\n";
		cin >> grid_type;
		if (grid_type == 1 || grid_type == 2) {
			break;
		}
		else {
			cerr << "\nThe number you entered is not correct, please try again.\n\n";
		}
	}

	filein.getline(buffer, sizeof(buffer));
	sscanf(buffer, "%d", &grid_size);
	filein.getline(buffer, sizeof(buffer));
	sscanf(buffer, "%d", &w_max);
	filein.close();
	cout << "\nInput [y] to find the minimum w, or anything else to continue with given w.\n";
	cin >> mode_input;
	if (mode_input == 'y') {
		mode = true;
	}
/*	cout << "\nInput [y] to start with debug mode, or anything else to continue.\n";
	cin >> debug_input;
	if (debug_input == 'y') {
		debug = true;
	}*/
	if (debug) {
		cout << "\n---------------------------- Run Under Debug Mode --------------------------\n";
		cout << "\nThere are " << grid_size << " by " << grid_size << " dimensions of the chip in logic blocks.\n";
		cout << "There are up to " << w_max << " tracks per channel.\n";
	}
	cout << "\n------------------------- Start Routing, Please Wait -----------------------\n\n";
	Router* circuit = NULL;
	for (width = w_max; width >= 1; width--) {
		if (debug) { cout << "\nrouting with w = " << width << "\n" << endl; }
		if(get_result){
			mode = false;
		}
		delete circuit;
		Router* circuit = new Router(grid_size, width, grid_type);
		filein.open(filename, ios::in);
		filein.getline(buffer, sizeof(buffer));
		filein.getline(buffer, sizeof(buffer));
		while (filein.getline(buffer, sizeof(buffer))) {
			source_x = source_y = source_pin = sink_x = sink_y = sink_pin = -1;
			sscanf(buffer, "%d %d %d %d %d %d ", &source_x, &source_y, &source_pin, &sink_x, &sink_y, &sink_pin);
			if (source_x == -1 && source_y == -1 && source_pin == -1 && sink_x == -1 && sink_y == -1 && sink_pin == -1) {
				break;
			}
			circuit -> to_net_list() -> push_back(new Net(source_x, source_y, source_pin, sink_x, sink_y, sink_pin));
			//if (debug) { cout <<  "Source:(" << source_x << "," << source_y << ") pin " << source_pin << " ==> Sink: (" << sink_x << "," << sink_y << ") pin " << sink_pin << "\n"; }
		}
		filein.close();
		routing_result = circuit -> maze_routing();
		if (routing_result == fail) {
			if (debug) { cout << "\nfailed to routing with w = " << width << endl; }
			if (width == w_max) {
				cout << "\nRouting failed.\n\n";
				break;
			}
			else {
				cout << "The minimum channel width is " << width + 1 << " and ";
				get_result = true;
				width = width + 2;
			}
		}
		if (!mode) {
			cout << segments << " segments are used in routing.\n\n" << "Routing success. \n\n";
			cout << "--------------------------------- Complete ---------------------------------\n" << endl;
			plot(circuit, width, grid_size, grid_type);
			break;
		}	
	}
	while (1) {}
	return 0;
}

