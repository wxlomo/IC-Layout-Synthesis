#include "segment.h"

enum result_type {
	success,
	fail
};

static char buffer[1024] = { 0 };
extern int segments;
extern bool mode;
extern bool debug;

class Router {
public:
	Router(int grid_size, int width, int grid_type) {
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
		result = success;
	}

	~Router() {
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

	vector<Segment*> find_adjacent(Segment* segment) {
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
	vector<Net*>* to_net_list() { return &net_list; };

	result_type maze_routing() {
		if (debug) { cout << "start mapping the grid..." << endl; }
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
			result = fail;
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
			if (debug) { cout << "net" << i << ": start mapping source and sink..." << endl; }
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
					result = success;
				}
				else if (segj->get_source() && segj->get_source_x() == source_x_ && segj->get_source_y() == source_y_ && segj->get_source_pin() == source_pin_) {
					segj->label(1); // share the segments from the same source pin
					expansion_list.push(segj);
					result = success;
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
					result = success;
				}
				else if (segt->get_sink() && segt->get_sink_x() == sink_x_ && segt->get_sink_y() == sink_y_ && segt->get_sink_pin() == sink_pin_) {
					segt->label(0); // share the segments from the same sink pin
					result = success;
				}
			}
			if (result == fail) {
				if (debug) { cerr << "source/sink mapping failed at net " << i << endl; }
				break;
			}
			if (debug) { cout << "net" << i << ": start routing..." << endl; }
			while (!expansion_list.empty()) { // routing the net
				result = fail;
				segj = expansion_list.front(); // pop the first segment from the expansion list
				expansion_list.pop();
				vector<Segment*>adjacent = find_adjacent(segj); // find all the segments connected with the first segment thru a switch
				for (unsigned int j = 0; j < adjacent.size(); j++) {
					Segment* next_segment = adjacent[j];
					if (next_segment->get_label() == 0 && next_segment->get_available()) { // break if reach the target
						segt = next_segment;
						segt->set_available(false);
						net_list[i]->to_route_list()->push_back(segt);
						result = success;
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
				if (result == success) {
					break;
				}
			}
			if (result == fail) {
				if (debug) { cerr << "routing failed at net " << i << endl; }
				break;
			}
			if (debug) { cout << "net" << i << ": start tracing back..." << endl; }
			for (int s = segj->get_label(); s >= 1; s--) { // tracing back with the label numbers until hits the source pin
				result = fail;
				vector<Segment*>adjacent = find_adjacent(segt);
				for (unsigned int l = 0; l < adjacent.size(); l++) {
					Segment* next_segment = adjacent[l];
					if (next_segment->get_label() == s) {
						segt = next_segment;
						segt->set_available(false);
						net_list[i]->to_route_list()->push_back(segt);
						result = success;
						break;
					}
				}
				if (result == fail) {
					if (debug) { cerr << "tracing back failed at net " << i << endl; }
					break;
				}
			}
			if (debug && result == success) { cout << "net" << i << ": routing success" << endl; }
			if (result == fail) {
				break;
			}
		}
		/*		if (result == fail) {
					net_sourcex.push_front(source_x_); net_sourcey.push_front(source_y_); net_sourcepin.push_front(source_pin_);
					net_sinkx.push_front(sink_x_); net_sinky.push_front(sink_y_); net_sinkpin.push_front(sink_pin_);
					net_sourcex.erase(net_sourcex.begin() + ii); net_sourcey.erase(net_sourcey.begin() + ii); net_sourcepin.erase(net_sourcepin.begin() + ii);
					net_sinkx.erase(net_sinkx.begin() + ii); net_sinky.erase(net_sinky.begin() + ii); net_sinkpin.erase(net_sinkpin.begin() + ii);
				}
				else {
					break;
				}
			}*/
		if (result == success) {
			segments = 0;
			for (unsigned int x = 0; x < grid_map.size(); x++) { // calculate the number of the used segments
				for (unsigned int y = 0; y < grid_map[x].size(); y++) {
					for (unsigned int w = 0; w < grid_map[x][y].size(); w++) {
						if (!grid_map[x][y][w]->get_available()) { segments++; };
					}
				}
			}
			if (debug) { cout << segments << " segments that are used when w = " << width_ << endl; };
		}
		return result;
	}

private:
	int grid_size_;
	int width_;
	int grid_type_;
	int source_x_;
	int source_y_;
	int source_pin_;
	int sink_x_;
	int sink_y_;
	int sink_pin_;
	int xx;
	int yy;
	int ww;
	result_type result;
	vector<Net*>net_list;
	queue<Segment*>expansion_list;
	vector<vector<vector<Segment*>>>grid_map;
};
