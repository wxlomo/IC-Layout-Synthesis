/*
 * router.cpp
 * Main logic of the router
 *
 * Author:Weixuan Yang
 * Date: Oct 08, 2021
 */

#include "router.h"

extern int segments = 0;
const float resolution = 100;
char msg[200] = { 0 };
int width_ = -1;
int grid_size_ = -1;
int grid_type_ = -1;
Router* circuit_;

Router::Router(int grid_size, int width, int grid_type) {
	grid_size_ = grid_size;
	width_ = width;
	grid_type_ = grid_type;
	source_x_ = -1;
	source_y_ = -1;
	source_pin_ = -1;
	sink_x_ = -1;
	sink_y_ = -1;
	sink_pin_ = -1;
	xx = -1;
	yy = -1;
	ww = -1;
}

Router::~Router() {
	for (unsigned int x = 0; x < net_list.size(); x++) {
		if (net_list[x]) delete net_list[x];
		net_list[x] = NULL;
	}
	for (unsigned int x = 0; x < grid_map.size(); x++) {
		for (unsigned int y = 0; y < grid_map[x].size(); y++) {
			for (unsigned int w = 0; w < grid_map[x][y].size(); w++) {
				if (grid_map[x][y][w]) delete grid_map[x][y][w];
				grid_map[x][y][w] = NULL;
			}
		}
	}
}

/* Logic for finding adjacent segments for the target */
vector<Segment*> Router::find_adjacent(Segment* segment) {
	vector<Segment*>adjacent;
	xx = segment->get_x();
	yy = segment->get_y();
	ww = segment->get_w();
	switch (grid_type_) {
	case 1:
		if (xx == 0) { // first column, vertical
			if (yy == 0) { // bottom left corner
				adjacent.push_back(grid_map[xx][yy + 1][ww]);
				adjacent.push_back(grid_map[xx + 1][yy][ww]);
				adjacent.push_back(grid_map[xx + 1][yy + 1][ww]);
			}
			else if (yy == grid_map[xx].size() - 1) { // upper left corner
				adjacent.push_back(grid_map[xx][yy - 1][ww]);
				adjacent.push_back(grid_map[xx + 1][yy][ww]);
				adjacent.push_back(grid_map[xx + 1][yy + 1][ww]);
			}
			else { // others
				adjacent.push_back(grid_map[xx][yy - 1][ww]);
				adjacent.push_back(grid_map[xx][yy + 1][ww]);
				adjacent.push_back(grid_map[xx + 1][yy][ww]);
				adjacent.push_back(grid_map[xx + 1][yy + 1][ww]);
			}
		}
		else if (xx == grid_map.size() - 1) { // last column, vertical
			if (yy == 0) { // bottom right corner
				adjacent.push_back(grid_map[xx][yy + 1][ww]);
				adjacent.push_back(grid_map[xx - 1][yy][ww]);
				adjacent.push_back(grid_map[xx - 1][yy + 1][ww]);
			}
			else if (yy == grid_map[xx].size() - 1) { // upper right corner
				adjacent.push_back(grid_map[xx][yy - 1][ww]);
				adjacent.push_back(grid_map[xx - 1][yy][ww]);
				adjacent.push_back(grid_map[xx - 1][yy + 1][ww]);
			}
			else { // others
				adjacent.push_back(grid_map[xx][yy - 1][ww]);
				adjacent.push_back(grid_map[xx][yy + 1][ww]);
				adjacent.push_back(grid_map[xx - 1][yy][ww]);
				adjacent.push_back(grid_map[xx - 1][yy + 1][ww]);
			}
		}
		else if (xx % 2 == 0) { // all other vertical
			if (yy == 0) { // bottom most
				adjacent.push_back(grid_map[xx][yy + 1][ww]);
				adjacent.push_back(grid_map[xx - 1][yy][ww]);
				adjacent.push_back(grid_map[xx + 1][yy][ww]);
				adjacent.push_back(grid_map[xx - 1][yy + 1][ww]);
				adjacent.push_back(grid_map[xx + 1][yy + 1][ww]);
			}
			else if (yy == grid_map[xx].size() - 1) { // upper most
				adjacent.push_back(grid_map[xx][yy - 1][ww]);
				adjacent.push_back(grid_map[xx - 1][yy][ww]);
				adjacent.push_back(grid_map[xx + 1][yy][ww]);
				adjacent.push_back(grid_map[xx - 1][yy + 1][ww]);
				adjacent.push_back(grid_map[xx + 1][yy + 1][ww]);
			}
			else { // others
				adjacent.push_back(grid_map[xx][yy - 1][ww]);
				adjacent.push_back(grid_map[xx][yy + 1][ww]);
				adjacent.push_back(grid_map[xx - 1][yy][ww]);
				adjacent.push_back(grid_map[xx + 1][yy][ww]);
				adjacent.push_back(grid_map[xx - 1][yy + 1][ww]);
				adjacent.push_back(grid_map[xx + 1][yy + 1][ww]);
			}
		}
		else if (xx == 1) { // first column, horizontal
			if (yy == 0) { // bottom left corner
				adjacent.push_back(grid_map[xx + 2][yy][ww]);
				adjacent.push_back(grid_map[xx - 1][yy][ww]);
				adjacent.push_back(grid_map[xx + 1][yy][ww]);
			}
			else if (yy == grid_map[xx].size() - 1) { // upper left corner
				adjacent.push_back(grid_map[xx + 2][yy][ww]);
				adjacent.push_back(grid_map[xx - 1][yy - 1][ww]);
				adjacent.push_back(grid_map[xx + 1][yy - 1][ww]);
			}
			else { // others
				adjacent.push_back(grid_map[xx + 2][yy][ww]);
				adjacent.push_back(grid_map[xx - 1][yy][ww]);
				adjacent.push_back(grid_map[xx + 1][yy][ww]);
				adjacent.push_back(grid_map[xx - 1][yy - 1][ww]);
				adjacent.push_back(grid_map[xx + 1][yy - 1][ww]);
			}
		}
		else if (xx == grid_map.size() - 2) { // last column, horizontal
			if (yy == 0) { // bottom right corner
				adjacent.push_back(grid_map[xx - 2][yy][ww]);
				adjacent.push_back(grid_map[xx - 1][yy][ww]);
				adjacent.push_back(grid_map[xx + 1][yy][ww]);
			}
			else if (yy == grid_map[xx].size() - 1) { // upper right corner
				adjacent.push_back(grid_map[xx - 2][yy][ww]);
				adjacent.push_back(grid_map[xx - 1][yy - 1][ww]);
				adjacent.push_back(grid_map[xx + 1][yy - 1][ww]);
			}
			else { // others
				adjacent.push_back(grid_map[xx - 2][yy][ww]);
				adjacent.push_back(grid_map[xx - 1][yy][ww]);
				adjacent.push_back(grid_map[xx + 1][yy][ww]);
				adjacent.push_back(grid_map[xx - 1][yy - 1][ww]);
				adjacent.push_back(grid_map[xx + 1][yy - 1][ww]);
			}
		}
		else { // all other horizontal
			if (yy == 0) { // bottom most
				adjacent.push_back(grid_map[xx - 2][yy][ww]);
				adjacent.push_back(grid_map[xx + 2][yy][ww]);
				adjacent.push_back(grid_map[xx - 1][yy][ww]);
				adjacent.push_back(grid_map[xx + 1][yy][ww]);
			}
			else if (yy == grid_map[xx].size() - 1) { // upper most
				adjacent.push_back(grid_map[xx - 2][yy][ww]);
				adjacent.push_back(grid_map[xx + 2][yy][ww]);
				adjacent.push_back(grid_map[xx - 1][yy - 1][ww]);
				adjacent.push_back(grid_map[xx + 1][yy - 1][ww]);
			}
			else { // others
				adjacent.push_back(grid_map[xx - 2][yy][ww]);
				adjacent.push_back(grid_map[xx + 2][yy][ww]);
				adjacent.push_back(grid_map[xx - 1][yy][ww]);
				adjacent.push_back(grid_map[xx + 1][yy][ww]);
				adjacent.push_back(grid_map[xx - 1][yy - 1][ww]);
				adjacent.push_back(grid_map[xx + 1][yy - 1][ww]);
			}
		}
		break;
	case 2:
		if (xx == 0) { // first column, vertical
			if (yy == 0) { // bottom left corner
				adjacent.push_back(grid_map[xx][yy + 1][ww]);
				adjacent.push_back(grid_map[xx + 1][yy][ww]);
				adjacent.push_back(grid_map[xx + 1][yy + 1][ww]);
				adjacent.push_back(grid_map[xx + 1][(yy + 1) * 2 + grid_size_][ww]);
				adjacent.push_back(grid_map[xx + 1][(yy + 1) * 2 + grid_size_ - 1][ww]);
				adjacent.push_back(grid_map[xx + 1][(yy + 1) * 2 + grid_size_ + 1][ww]);
			}
			else if (yy == grid_size_ - 1) { // upper left corner
				adjacent.push_back(grid_map[xx][yy - 1][ww]);
				adjacent.push_back(grid_map[xx + 1][yy][ww]);
				adjacent.push_back(grid_map[xx + 1][yy + 1][ww]);
				adjacent.push_back(grid_map[xx + 1][(yy + 1) * 2 + grid_size_][ww]);
				adjacent.push_back(grid_map[xx + 1][(yy + 1) * 2 + grid_size_ - 1][ww]);
				adjacent.push_back(grid_map[xx + 1][(yy + 1) * 2 + grid_size_ - 2][ww]);
			}
			else { // others
				adjacent.push_back(grid_map[xx][yy - 1][ww]);
				adjacent.push_back(grid_map[xx][yy + 1][ww]);
				adjacent.push_back(grid_map[xx + 1][yy][ww]);
				adjacent.push_back(grid_map[xx + 1][yy + 1][ww]);
				adjacent.push_back(grid_map[xx + 1][(yy + 1) * 2 + grid_size_][ww]);
				adjacent.push_back(grid_map[xx + 1][(yy + 1) * 2 + grid_size_ - 1][ww]);
				adjacent.push_back(grid_map[xx + 1][(yy + 1) * 2 + grid_size_ - 2][ww]);
				adjacent.push_back(grid_map[xx + 1][(yy + 1) * 2 + grid_size_ + 1][ww]);
			}
		}
		else if (xx == grid_map.size() - 1) { // last column, vertical
			if (yy == 0) { // bottom right corner
				adjacent.push_back(grid_map[xx][yy + 1][ww]);
				adjacent.push_back(grid_map[xx - 1][yy][ww]);
				adjacent.push_back(grid_map[xx - 1][yy + 1][ww]);
				adjacent.push_back(grid_map[xx - 1][(yy + 1) * 2 + grid_size_][ww]);
				adjacent.push_back(grid_map[xx - 1][(yy + 1) * 2 + grid_size_ - 1][ww]);
				adjacent.push_back(grid_map[xx - 1][(yy + 1) * 2 + grid_size_ + 2][ww]);
			}
			else if (yy == grid_size_ - 1) { // upper right corner
				adjacent.push_back(grid_map[xx][yy - 1][ww]);
				adjacent.push_back(grid_map[xx - 1][yy][ww]);
				adjacent.push_back(grid_map[xx - 1][yy + 1][ww]);
				adjacent.push_back(grid_map[xx - 1][(yy + 1) * 2 + grid_size_][ww]);
				adjacent.push_back(grid_map[xx - 1][(yy + 1) * 2 + grid_size_ - 1][ww]);
				adjacent.push_back(grid_map[xx - 1][(yy + 1) * 2 + grid_size_ - 3][ww]);
			}
			else { // others
				adjacent.push_back(grid_map[xx][yy - 1][ww]);
				adjacent.push_back(grid_map[xx][yy + 1][ww]);
				adjacent.push_back(grid_map[xx - 1][yy][ww]);
				adjacent.push_back(grid_map[xx - 1][yy + 1][ww]);
				adjacent.push_back(grid_map[xx - 1][(yy + 1) * 2 + grid_size_][ww]);
				adjacent.push_back(grid_map[xx - 1][(yy + 1) * 2 + grid_size_ - 1][ww]);
				adjacent.push_back(grid_map[xx - 1][(yy + 1) * 2 + grid_size_ + 2][ww]);
				adjacent.push_back(grid_map[xx - 1][(yy + 1) * 2 + grid_size_ - 3][ww]);
			}
		}
		else if (xx % 2 == 0) { // all other vertical
			if (yy == 0) { // bottom most
				adjacent.push_back(grid_map[xx][yy + 1][ww]);
				adjacent.push_back(grid_map[xx - 1][yy][ww]);
				adjacent.push_back(grid_map[xx + 1][yy][ww]);
				adjacent.push_back(grid_map[xx - 1][yy + 1][ww]);
				adjacent.push_back(grid_map[xx + 1][yy + 1][ww]);
				adjacent.push_back(grid_map[xx + 1][(yy + 1) * 2 + grid_size_][ww]);
				adjacent.push_back(grid_map[xx + 1][(yy + 1) * 2 + grid_size_ - 1][ww]);
				adjacent.push_back(grid_map[xx + 1][(yy + 1) * 2 + grid_size_ + 1][ww]);
				adjacent.push_back(grid_map[xx - 1][(yy + 1) * 2 + grid_size_][ww]);
				adjacent.push_back(grid_map[xx - 1][(yy + 1) * 2 + grid_size_ - 1][ww]);
				adjacent.push_back(grid_map[xx - 1][(yy + 1) * 2 + grid_size_ + 2][ww]);

			}
			else if (yy == grid_size_ - 1) { // upper most
				adjacent.push_back(grid_map[xx][yy - 1][ww]);
				adjacent.push_back(grid_map[xx - 1][yy][ww]);
				adjacent.push_back(grid_map[xx + 1][yy][ww]);
				adjacent.push_back(grid_map[xx - 1][yy + 1][ww]);
				adjacent.push_back(grid_map[xx + 1][yy + 1][ww]);
				adjacent.push_back(grid_map[xx + 1][(yy + 1) * 2 + grid_size_][ww]);
				adjacent.push_back(grid_map[xx + 1][(yy + 1) * 2 + grid_size_ - 1][ww]);
				adjacent.push_back(grid_map[xx + 1][(yy + 1) * 2 + grid_size_ - 2][ww]);
				adjacent.push_back(grid_map[xx - 1][(yy + 1) * 2 + grid_size_][ww]);
				adjacent.push_back(grid_map[xx - 1][(yy + 1) * 2 + grid_size_ - 1][ww]);
				adjacent.push_back(grid_map[xx - 1][(yy + 1) * 2 + grid_size_ - 3][ww]);

			}
			else { // others
				adjacent.push_back(grid_map[xx][yy - 1][ww]);
				adjacent.push_back(grid_map[xx][yy + 1][ww]);
				adjacent.push_back(grid_map[xx - 1][yy][ww]);
				adjacent.push_back(grid_map[xx + 1][yy][ww]);
				adjacent.push_back(grid_map[xx - 1][yy + 1][ww]);
				adjacent.push_back(grid_map[xx + 1][yy + 1][ww]);
				adjacent.push_back(grid_map[xx + 1][(yy + 1) * 2 + grid_size_][ww]);
				adjacent.push_back(grid_map[xx + 1][(yy + 1) * 2 + grid_size_ - 1][ww]);
				adjacent.push_back(grid_map[xx + 1][(yy + 1) * 2 + grid_size_ - 2][ww]);
				adjacent.push_back(grid_map[xx + 1][(yy + 1) * 2 + grid_size_ + 1][ww]);
				adjacent.push_back(grid_map[xx - 1][(yy + 1) * 2 + grid_size_][ww]);
				adjacent.push_back(grid_map[xx - 1][(yy + 1) * 2 + grid_size_ - 1][ww]);
				adjacent.push_back(grid_map[xx - 1][(yy + 1) * 2 + grid_size_ + 2][ww]);
				adjacent.push_back(grid_map[xx - 1][(yy + 1) * 2 + grid_size_ - 3][ww]);

			}
		}
		else if (yy <= grid_size_) { // horizontal
			if (xx == 1) { // first column, horizontal
				if (yy == 0) { // bottom left corner
					adjacent.push_back(grid_map[xx + 2][yy][ww]);
					adjacent.push_back(grid_map[xx - 1][yy][ww]);
					adjacent.push_back(grid_map[xx + 1][yy][ww]);
					adjacent.push_back(grid_map[xx][(yy + 1) * 2 + grid_size_][ww]);
					adjacent.push_back(grid_map[xx][(yy + 1) * 2 + grid_size_ - 1][ww]);
					adjacent.push_back(grid_map[xx + 2][(yy + 1) * 2 + grid_size_ - 1][ww]);
				}
				else if (yy == grid_size_) { // upper left corner
					adjacent.push_back(grid_map[xx + 2][yy][ww]);
					adjacent.push_back(grid_map[xx - 1][yy - 1][ww]);
					adjacent.push_back(grid_map[xx + 1][yy - 1][ww]);
					adjacent.push_back(grid_map[xx][yy * 2 + grid_size_][ww]);
					adjacent.push_back(grid_map[xx][yy * 2 + grid_size_ - 1][ww]);
					adjacent.push_back(grid_map[xx + 2][yy * 2 + grid_size_][ww]);
				}
				else { // others
					adjacent.push_back(grid_map[xx + 2][yy][ww]);
					adjacent.push_back(grid_map[xx - 1][yy][ww]);
					adjacent.push_back(grid_map[xx + 1][yy][ww]);
					adjacent.push_back(grid_map[xx - 1][yy - 1][ww]);
					adjacent.push_back(grid_map[xx + 1][yy - 1][ww]);
					adjacent.push_back(grid_map[xx][(yy + 1) * 2 + grid_size_][ww]);
					adjacent.push_back(grid_map[xx][(yy + 1) * 2 + grid_size_ - 1][ww]);
					adjacent.push_back(grid_map[xx + 2][(yy + 1) * 2 + grid_size_ - 1][ww]);
					adjacent.push_back(grid_map[xx][yy * 2 + grid_size_][ww]);
					adjacent.push_back(grid_map[xx][yy * 2 + grid_size_ - 1][ww]);
					adjacent.push_back(grid_map[xx + 2][yy * 2 + grid_size_][ww]);
				}
			}
			else if (xx == grid_map.size() - 2) { // last column, horizontal
				if (yy == 0) { // bottom right corner
					adjacent.push_back(grid_map[xx - 2][yy][ww]);
					adjacent.push_back(grid_map[xx - 1][yy][ww]);
					adjacent.push_back(grid_map[xx + 1][yy][ww]);
					adjacent.push_back(grid_map[xx][(yy + 1) * 2 + grid_size_][ww]);
					adjacent.push_back(grid_map[xx][(yy + 1) * 2 + grid_size_ - 1][ww]);
					adjacent.push_back(grid_map[xx - 2][(yy + 1) * 2 + grid_size_][ww]);
				}
				else if (yy == grid_size_) { // upper right corner
					adjacent.push_back(grid_map[xx - 2][yy][ww]);
					adjacent.push_back(grid_map[xx - 1][yy - 1][ww]);
					adjacent.push_back(grid_map[xx + 1][yy - 1][ww]);
					adjacent.push_back(grid_map[xx][yy * 2 + grid_size_][ww]);
					adjacent.push_back(grid_map[xx][yy * 2 + grid_size_ - 1][ww]);
					adjacent.push_back(grid_map[xx - 2][yy * 2 + grid_size_ - 1][ww]);
				}
				else { // others
					adjacent.push_back(grid_map[xx - 2][yy][ww]);
					adjacent.push_back(grid_map[xx - 1][yy][ww]);
					adjacent.push_back(grid_map[xx + 1][yy][ww]);
					adjacent.push_back(grid_map[xx - 1][yy - 1][ww]);
					adjacent.push_back(grid_map[xx + 1][yy - 1][ww]);
					adjacent.push_back(grid_map[xx][(yy + 1) * 2 + grid_size_][ww]);
					adjacent.push_back(grid_map[xx][(yy + 1) * 2 + grid_size_ - 1][ww]);
					adjacent.push_back(grid_map[xx - 2][(yy + 1) * 2 + grid_size_][ww]);
					adjacent.push_back(grid_map[xx][yy * 2 + grid_size_][ww]);
					adjacent.push_back(grid_map[xx][yy * 2 + grid_size_ - 1][ww]);
					adjacent.push_back(grid_map[xx - 2][yy * 2 + grid_size_ - 1][ww]);
				}
			}
			else { // all other horizontal
				if (yy == 0) { // bottom most
					adjacent.push_back(grid_map[xx - 2][yy][ww]);
					adjacent.push_back(grid_map[xx + 2][yy][ww]);
					adjacent.push_back(grid_map[xx - 1][yy][ww]);
					adjacent.push_back(grid_map[xx + 1][yy][ww]);
					adjacent.push_back(grid_map[xx][(yy + 1) * 2 + grid_size_][ww]);
					adjacent.push_back(grid_map[xx][(yy + 1) * 2 + grid_size_ - 1][ww]);
					adjacent.push_back(grid_map[xx - 2][(yy + 1) * 2 + grid_size_][ww]);
					adjacent.push_back(grid_map[xx + 2][(yy + 1) * 2 + grid_size_ - 1][ww]);
				}
				else if (yy == grid_size_) { // upper most
					adjacent.push_back(grid_map[xx - 2][yy][ww]);
					adjacent.push_back(grid_map[xx + 2][yy][ww]);
					adjacent.push_back(grid_map[xx - 1][yy - 1][ww]);
					adjacent.push_back(grid_map[xx + 1][yy - 1][ww]);
					adjacent.push_back(grid_map[xx][yy * 2 + grid_size_][ww]);
					adjacent.push_back(grid_map[xx][yy * 2 + grid_size_ - 1][ww]);
					adjacent.push_back(grid_map[xx - 2][yy * 2 + grid_size_ - 1][ww]);
					adjacent.push_back(grid_map[xx + 2][yy * 2 + grid_size_][ww]);
				}
				else { // others
					adjacent.push_back(grid_map[xx - 2][yy][ww]);
					adjacent.push_back(grid_map[xx + 2][yy][ww]);
					adjacent.push_back(grid_map[xx - 1][yy][ww]);
					adjacent.push_back(grid_map[xx + 1][yy][ww]);
					adjacent.push_back(grid_map[xx - 1][yy - 1][ww]);
					adjacent.push_back(grid_map[xx + 1][yy - 1][ww]);
					adjacent.push_back(grid_map[xx][(yy + 1) * 2 + grid_size_][ww]);
					adjacent.push_back(grid_map[xx][(yy + 1) * 2 + grid_size_ - 1][ww]);
					adjacent.push_back(grid_map[xx - 2][(yy + 1) * 2 + grid_size_][ww]);
					adjacent.push_back(grid_map[xx + 2][(yy + 1) * 2 + grid_size_ - 1][ww]);
					adjacent.push_back(grid_map[xx][yy * 2 + grid_size_][ww]);
					adjacent.push_back(grid_map[xx][yy * 2 + grid_size_ - 1][ww]);
					adjacent.push_back(grid_map[xx - 2][yy * 2 + grid_size_ - 1][ww]);
					adjacent.push_back(grid_map[xx + 2][yy * 2 + grid_size_][ww]);
				}
			}
		}
		else { // diagonal
			if ((yy - grid_size_) % 2 == 1) { // bottom left to upper right
				if (xx == 1) { // first column
					if (yy == grid_size_ + 1) { // bottom left corner
						adjacent.push_back(grid_map[xx][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx][(yy - grid_size_ + 1) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ + 1) / 2][ww]);
						adjacent.push_back(grid_map[xx + 2][(yy - grid_size_ + 1) / 2][ww]);
						adjacent.push_back(grid_map[xx + 2][yy + 1][ww]);
						adjacent.push_back(grid_map[xx + 2][yy + 2][ww]);
						adjacent.push_back(grid_map[xx][yy + 3][ww]);
					}
					else if (yy == grid_map[xx].size() - 2) { // upper left corner
						adjacent.push_back(grid_map[xx][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx][(yy - grid_size_ + 1) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_ - 3) / 2][ww]);
						adjacent.push_back(grid_map[xx + 2][(yy - grid_size_ + 1) / 2][ww]);
						adjacent.push_back(grid_map[xx][yy - 1][ww]);
						adjacent.push_back(grid_map[xx + 2][yy + 1][ww]);
					}
					else { // others
						adjacent.push_back(grid_map[xx][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx][(yy - grid_size_ + 1) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_ - 3) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ + 1) / 2][ww]);
						adjacent.push_back(grid_map[xx + 2][(yy - grid_size_ + 1) / 2][ww]);
						adjacent.push_back(grid_map[xx + 2][yy + 1][ww]);
						adjacent.push_back(grid_map[xx + 2][yy + 2][ww]);
						adjacent.push_back(grid_map[xx][yy + 3][ww]);
						adjacent.push_back(grid_map[xx][yy - 1][ww]);
					}
				}
				else if (xx == grid_map.size() - 2) { // last column
					if (yy == grid_size_ + 1) { // bottom right corner
						adjacent.push_back(grid_map[xx][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx][(yy - grid_size_ + 1) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx - 2][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ + 1) / 2][ww]);
						adjacent.push_back(grid_map[xx - 2][yy + 1][ww]);
						adjacent.push_back(grid_map[xx][yy + 3][ww]);
					}
					else if (yy == grid_map[xx].size() - 2) { // upper right corner
						adjacent.push_back(grid_map[xx][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx][(yy - grid_size_ + 1) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_ - 3) / 2][ww]);
						adjacent.push_back(grid_map[xx - 2][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx][yy - 1][ww]);
						adjacent.push_back(grid_map[xx - 2][yy + 1][ww]);
						adjacent.push_back(grid_map[xx - 2][yy - 2][ww]);
					}
					else { // others
						adjacent.push_back(grid_map[xx][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx][(yy - grid_size_ + 1) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx - 2][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_ - 3) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ + 1) / 2][ww]);
						adjacent.push_back(grid_map[xx][yy - 1][ww]);
						adjacent.push_back(grid_map[xx][yy + 3][ww]);
						adjacent.push_back(grid_map[xx - 2][yy + 1][ww]);
						adjacent.push_back(grid_map[xx - 2][yy - 2][ww]);
					}
				}
				else { // others
					if (yy == grid_size_ + 1) { // most bottom
						adjacent.push_back(grid_map[xx][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx][(yy - grid_size_ + 1) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx - 2][(yy - grid_size_ - 1)][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ + 1)][ww]);
						adjacent.push_back(grid_map[xx + 2][(yy - grid_size_ + 1)][ww]);
						adjacent.push_back(grid_map[xx - 2][yy + 1][ww]);
						adjacent.push_back(grid_map[xx][yy + 3][ww]);
						adjacent.push_back(grid_map[xx + 2][yy + 2][ww]);
						adjacent.push_back(grid_map[xx + 2][yy + 1][ww]);
					}
					else if (yy == grid_map[xx].size() - 2) { // most upper
						adjacent.push_back(grid_map[xx][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx][(yy - grid_size_ + 1) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx + 2][(yy - grid_size_ + 1) / 2][ww]);
						adjacent.push_back(grid_map[xx - 2][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_ - 3) / 2][ww]);
						adjacent.push_back(grid_map[xx - 2][yy + 1][ww]);
						adjacent.push_back(grid_map[xx - 2][yy - 2][ww]);
						adjacent.push_back(grid_map[xx][yy - 1][ww]);
						adjacent.push_back(grid_map[xx + 2][yy + 1][ww]);
					}
					else { // all others
						adjacent.push_back(grid_map[xx][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx][(yy - grid_size_ + 1) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx - 2][(yy - grid_size_ - 1) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_ - 3) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ + 1) / 2][ww]);
						adjacent.push_back(grid_map[xx + 2][(yy - grid_size_ + 1) / 2][ww]);
						adjacent.push_back(grid_map[xx - 2][yy + 1][ww]);
						adjacent.push_back(grid_map[xx - 2][yy - 2][ww]);
						adjacent.push_back(grid_map[xx][yy - 1][ww]);
						adjacent.push_back(grid_map[xx][yy + 3][ww]);
						adjacent.push_back(grid_map[xx + 2][yy + 2][ww]);
						adjacent.push_back(grid_map[xx + 2][yy + 1][ww]);
					}
				}
			}
			else { // upper right to bottom left
				if (xx == 1) { // first column
					if (yy == grid_size_ + 2) { // bottom left corner
						adjacent.push_back(grid_map[xx][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx][(yy - grid_size_) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx + 2][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_) / 2][ww]);
						adjacent.push_back(grid_map[xx][yy + 1][ww]);
						adjacent.push_back(grid_map[xx + 2][yy - 1][ww]);
					}
					else if (yy == grid_map[xx].size() - 1) { // upper left corner
						adjacent.push_back(grid_map[xx][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx][(yy - grid_size_) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ - 4) / 2][ww]);
						adjacent.push_back(grid_map[xx + 2][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx][yy - 3][ww]);
						adjacent.push_back(grid_map[xx + 2][yy - 2][ww]);
						adjacent.push_back(grid_map[xx + 2][yy - 1][ww]);
					}
					else { // others
						adjacent.push_back(grid_map[xx][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx][(yy - grid_size_) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ - 4) / 2][ww]);
						adjacent.push_back(grid_map[xx + 2][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx][yy + 1][ww]);
						adjacent.push_back(grid_map[xx][yy - 3][ww]);
						adjacent.push_back(grid_map[xx + 2][yy - 2][ww]);
						adjacent.push_back(grid_map[xx + 2][yy - 1][ww]);
					}
				}
				else if (xx == grid_map.size() - 2) { // last column
					if (yy == grid_size_ + 2) { // bottom right corner
						adjacent.push_back(grid_map[xx][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx][(yy - grid_size_) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_) / 2][ww]);
						adjacent.push_back(grid_map[xx - 2][(yy - grid_size_) / 2][ww]);
						adjacent.push_back(grid_map[xx][yy + 1][ww]);
						adjacent.push_back(grid_map[xx - 2][yy - 1][ww]);
						adjacent.push_back(grid_map[xx - 2][yy + 2][ww]);
					}
					else if (yy == grid_map[xx].size() - 1) { // upper right corner
						adjacent.push_back(grid_map[xx][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx][(yy - grid_size_) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx - 2][(yy - grid_size_) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ - 4) / 2][ww]);
						adjacent.push_back(grid_map[xx - 2][yy - 1][ww]);
						adjacent.push_back(grid_map[xx][yy - 3][ww]);
					}
					else { // others
						adjacent.push_back(grid_map[xx][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx][(yy - grid_size_) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_) / 2][ww]);
						adjacent.push_back(grid_map[xx - 2][(yy - grid_size_) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ - 4) / 2][ww]);
						adjacent.push_back(grid_map[xx][yy + 1][ww]);
						adjacent.push_back(grid_map[xx][yy - 3][ww]);
						adjacent.push_back(grid_map[xx - 2][yy - 1][ww]);
						adjacent.push_back(grid_map[xx - 2][yy + 2][ww]);
					}
				}
				else { // others
					if (yy == grid_size_ + 2) { // most bottom
						adjacent.push_back(grid_map[xx][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx][(yy - grid_size_) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_) / 2][ww]);
						adjacent.push_back(grid_map[xx - 2][(yy - grid_size_) / 2][ww]);
						adjacent.push_back(grid_map[xx + 2][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx][yy + 1][ww]);
						adjacent.push_back(grid_map[xx - 2][yy - 1][ww]);
						adjacent.push_back(grid_map[xx - 2][yy + 2][ww]);
						adjacent.push_back(grid_map[xx + 2][yy - 1][ww]);
					}
					else if (yy == grid_map[xx].size() - 1) { // most upper
						adjacent.push_back(grid_map[xx][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx][(yy - grid_size_) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx - 2][(yy - grid_size_) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ - 4) / 2][ww]);
						adjacent.push_back(grid_map[xx + 2][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx][yy - 3][ww]);
						adjacent.push_back(grid_map[xx - 2][yy - 1][ww]);
						adjacent.push_back(grid_map[xx + 2][yy - 2][ww]);
						adjacent.push_back(grid_map[xx + 2][yy - 1][ww]);
					}
					else { // all others
						adjacent.push_back(grid_map[xx][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx][(yy - grid_size_) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx - 1][(yy - grid_size_) / 2][ww]);
						adjacent.push_back(grid_map[xx - 2][(yy - grid_size_) / 2][ww]);
						adjacent.push_back(grid_map[xx + 2][(yy - grid_size_ - 2) / 2][ww]);
						adjacent.push_back(grid_map[xx + 1][(yy - grid_size_ - 4) / 2][ww]);
						adjacent.push_back(grid_map[xx][yy + 1][ww]);
						adjacent.push_back(grid_map[xx][yy - 3][ww]);
						adjacent.push_back(grid_map[xx - 2][yy - 1][ww]);
						adjacent.push_back(grid_map[xx - 2][yy + 2][ww]);
						adjacent.push_back(grid_map[xx + 2][yy - 1][ww]);
						adjacent.push_back(grid_map[xx + 2][yy - 2][ww]);
					}
				}
			}
		}
		break;
	}
	return adjacent;
}

vector<Net*>* Router::to_net_list() { 
	return &net_list; 
}

/* Maze router main process */
bool Router::maze_routing() {
	switch (grid_type_) { // grid mapping
	case 1:
		for (int x = 0; x < 2 * grid_size_ + 1; x++) {
			vector<vector<Segment*>>trackx;
			grid_map.push_back(trackx);
			for (int y = 0; y < grid_size_ + x % 2; y++) {
				vector<Segment*>tracky;
				grid_map[x].push_back(tracky);
				for (int w = 0; w < width_; w++) {
					grid_map[x][y].push_back(new Segment(x, y, w));
					grid_map[x][y][w]->reset_segment();
				}
			}
		}
		break;
	case 2:
		for (int x = 0; x < 2 * grid_size_ + 1; x++) {
			vector<vector<Segment*>>trackx;
			grid_map.push_back(trackx);
			if (x % 2 == 0) { // vertical
				for (int y = 0; y < grid_size_; y++) {
					vector<Segment*>tracky;
					grid_map[x].push_back(tracky);
					for (int w = 0; w < width_; w++) {
						grid_map[x][y].push_back(new Segment(x, y, w));
						grid_map[x][y][w]->reset_segment();
					}
				}
			}
			else { // horizontal and diagonal
				for (int y = 0; y < 3 * grid_size_ + 1; y++) {
					vector<Segment*>tracky;
					grid_map[x].push_back(tracky);
					for (int w = 0; w < width_; w++) {
						grid_map[x][y].push_back(new Segment(x, y, w));
						grid_map[x][y][w]->reset_segment();
					}
				}
			}
		}
		break;
	}

	/*	for (unsigned int n = 0; n < net_list.size(); n++) {
			int ii = 0; */
	for (unsigned int i = 0; i < net_list.size(); i++) {
		//ii = i;
		while (!expansion_list.empty()) { // initialize the expansion list
			expansion_list.pop();
		}
		bool flag = false;
		Segment* segj = NULL; // j segment
		Segment* segt = NULL; // target segment
		source_x_ = net_list[i]->net_get_source_x();
		source_y_ = net_list[i]->net_get_source_y();
		source_pin_ = net_list[i]->net_get_source_pin();
		sink_x_ = net_list[i]->net_get_sink_x();
		sink_y_ = net_list[i]->net_get_sink_y();
		sink_pin_ = net_list[i]->net_get_sink_pin();
		for (unsigned int x = 0; x < grid_map.size(); x++) {
			for (unsigned int y = 0; y < grid_map[x].size(); y++) {
				for (unsigned int w = 0; w < grid_map[x][y].size(); w++) {
					grid_map[x][y][w]->label(-1);
				}
			}
		}
		for (int w = 0; w < width_; w++) {  // mapping the source pin and the sink pin
			switch (source_pin_) {
			case 1:
				segj = grid_map[2 * source_x_ + 1][source_y_][w];
				break;
			case 2:
				segj = grid_map[2 * source_x_][source_y_][w];
				break;
			case 3:
				segj = grid_map[2 * source_x_ + 1][source_y_ + 1][w];
				break;
			case 4:
				segj = grid_map[2 * source_x_ + 2][source_y_][w];
				break;
			}
			if (segj->get_available()) {
				segj->label(1); // mark 'O'
				segj->set_source(true, source_x_, source_y_, source_pin_);
				expansion_list.push(segj);
				flag = true;
			}
			else if (segj->get_source() && segj->get_source_x() == source_x_ && segj->get_source_y() == source_y_ && segj->get_source_pin() == source_pin_) {
				segj->label(1); // share the segments from the same source pin
				expansion_list.push(segj);
				flag = true;
			}
		}
		for (int w = 0; w < width_; w++) {
			switch (sink_pin_) {
			case 1:
				segt = grid_map[2 * sink_x_ + 1][sink_y_][w];
				break;
			case 2:
				segt = grid_map[2 * sink_x_][sink_y_][w];
				break;
			case 3:
				segt = grid_map[2 * sink_x_ + 1][sink_y_ + 1][w];
				break;
			case 4:
				segt = grid_map[2 * sink_x_ + 2][sink_y_][w];
				break;
			}
			if (segt->get_available()) {
				segt->label(0); // mark 'T'
				segt->set_sink(true, sink_x_, sink_y_, sink_pin_);
				flag = true;
			}
			else if (segt->get_sink() && segt->get_sink_x() == sink_x_ && segt->get_sink_y() == sink_y_ && segt->get_sink_pin() == sink_pin_) {
				segt->label(0); // share the segments from the same sink pin
				flag = true;
			}
		}
		if (!flag) {
			cerr << "* Source/sink mapping failed at net " << i << endl;
			return false;
		}
		while (!expansion_list.empty()) { // routing the net
			flag = false;
			segj = expansion_list.front(); // pop the first segment from the expansion list
			expansion_list.pop();
			vector<Segment*>adjacent = find_adjacent(segj); // find all the segments connected with the first segment thru a switch
			for (unsigned int j = 0; j < adjacent.size(); j++) {
				Segment* next_segment = adjacent[j];
				if (next_segment->get_label() == 0 && next_segment->get_available()) { // break if reach the target
					segt = next_segment;
					segt->set_available(false);
					net_list[i]->to_route_list()->push_back(segt);
					flag = true;
					break;
				}
				else if (next_segment->get_label() == -1) { // make sure no label overwrite
					if (next_segment->get_available()) { // continue routing if the next segment is available
						next_segment->set_source(true, segj->get_source_x(), segj->get_source_y(), segj->get_source_pin());
						next_segment->label(segj->get_label() + 1);
						expansion_list.push(next_segment);
					}
					else if (next_segment->get_source_x() == segj->get_source_x() && next_segment->get_source_y() == segj->get_source_y() && next_segment->get_source_pin() == segj->get_source_pin()) {
						next_segment->label(segj->get_label() + 1); // share the segments that route from the same source pin
						expansion_list.push(next_segment);
					}
				}
			}
			if (flag == true) {
				break;
			}
		}
		if (!flag) {
			// cerr << "* Routing failed at net " << i << endl;
			return false;
		}
		for (int s = segj->get_label(); s >= 1; s--) { // tracing back with the label numbers until hits the source pin
			flag = false;
			vector<Segment*>adjacent = find_adjacent(segt);
			for (unsigned int l = 0; l < adjacent.size(); l++) {
				Segment* next_segment = adjacent[l];
				if (next_segment->get_label() == s) {
					segt = next_segment;
					segt->set_available(false);
					net_list[i]->to_route_list()->push_back(segt);
					flag = true;
					break;
				}
			}
			if (!flag) {
				cerr << "* Tracing back failed at net " << i << endl;
				return false;
			}
		}
	}
	segments = 0;
	for (unsigned int x = 0; x < grid_map.size(); x++) { // calculate the number of the used segments
		for (unsigned int y = 0; y < grid_map[x].size(); y++) {
			for (unsigned int w = 0; w < grid_map[x][y].size(); w++) {
				if (!grid_map[x][y][w]->get_available()) { segments++; };
			}
		}
	}
	sprintf(msg, "[Complete] Segment usage: %d. Channel Width usage: %d. [Created by: Weixuan Yang]", segments, width_);
	return true;
}

void plot(Router* circuit, int width, int grid_size, int grid_type) {
	width_ = width;
	grid_size_ = grid_size;
	grid_type_ = grid_type;
	circuit_ = circuit;
	float window_size_x = resolution * (2 * grid_size_ + 1);
	float window_size_y = resolution * (2 * grid_size_ + 1);
	init_graphics("Maze Router", WHITE);
	init_world(0., 0., window_size_x, window_size_y);
	//create_button("Window", "All Net", act_on_new_button_func);
	set_draw_mode(DRAW_NORMAL);
	event_loop(act_on_button_press, NULL, NULL, plot_circuit);
	close_graphics();
}

void plot_circuit(void) {
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
	setlinewidth(1);
	for (int x = 0; x < grid_size_; x++) { // draw horizontal segments
		for (int y = 0; y < grid_size_ + 1; y++) {
			for (int w = 0; w < width_; w++) {
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
		setlinewidth(1);
		for (unsigned int n = 0; n < circuit_->to_net_list()->at(i)->to_route_list()->size(); n++) { // draw route	
			unsigned int x = circuit_->to_net_list()->at(i)->to_route_list()->at(n)->get_x();
			unsigned int y = circuit_->to_net_list()->at(i)->to_route_list()->at(n)->get_y();
			unsigned int w = circuit_->to_net_list()->at(i)->to_route_list()->at(n)->get_w();
			switch (grid_type_) {
			case 1:
				if (x % 2 == 0) { // vertical
					drawline((resolution * (x + 0.25 + w * linegap1)), (resolution * ((2 * (grid_size_ - 1 - y)) + 0.75)), (resolution * (x + 0.25 + w * linegap1)), (resolution * ((2 * (grid_size_ - 1 - y)) + 2.25)));
				}
				else { // horizontal
					drawline((resolution * (x - 0.25)), (resolution * (2 * (grid_size_ - y) + 0.25 + w * linegap1)), (resolution * (x + 1.25)), (resolution * (2 * (grid_size_ - y) + 0.25 + w * linegap1)));
				}
				break;
			case 2:
				if (x % 2 == 0) { // vertical
					drawline((resolution * (x + 0.4 + w * linegap2)), (resolution * ((2 * (grid_size_ - 1 - y)) + 0.8)), (resolution * (x + 0.4 + w * linegap2)), (resolution * ((2 * (grid_size_ - 1 - y)) + 2.2)));
				}
				else if (y <= grid_size_) { // horizontal
					drawline((resolution * (x - 0.2)), (resolution * (2 * (grid_size_ - y) + 0.4 + w * linegap2)), (resolution * (x + 1.2)), (resolution * (2 * (grid_size_ - y) + 0.4 + w * linegap2)));
				}
				else { // diagonal
					if (y % 2 == 0) { // upper right to bottom left
						drawline((resolution * (x - 0.2 - w * linegap2)), (resolution * (2 * (grid_size_ - 1 - (y - grid_size_) / 2) + 0.6 + w * linegap2)), (resolution * (x + 1.4 - w * linegap2)), (resolution * (2 * (grid_size_ - 1 - (y - grid_size_) / 2) + 2.2 + w * linegap2)));
					}
					else { // bottom right to upper left
						drawline((resolution * (x - 0.2 - w * linegap2)), (resolution * (2 * (grid_size_ - 1 - (y - grid_size_) / 2) + 2.4 - w * linegap2)), (resolution * (x + 1.4 - w * linegap2)), (resolution * (2 * (grid_size_ - 1 - (y - grid_size_) / 2) + 0.8 - w * linegap2)));
					}
				}

			}

		}
	}
	update_message(msg);
}

void act_on_button_press(float x, float y) {
}
