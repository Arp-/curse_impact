#include <iostream>
#include <ncurses.h>
#include <functional>
#include <algorithm>
#include "canvas_t.hpp"
#include "algorithm.hpp"

using renderer::canvas_t;
using renderer::coord_t;
using renderer::polygon_t;
using renderer::point_inclusion;

class scoped_initializer {

	public:
		scoped_initializer(std::function<void(void)>&& constructor, 
				std::function<void(void)> destructor): destructor_(std::move(destructor)) {
			constructor();
		}

		~scoped_initializer() {
			this->destructor_();
		}

	private: 
		std::function<void(void)> destructor_;

};
//-----------------------------------------------------------------------------//
#define UL 0x1
#define LL 0x2
#define LR 0x4
#define UR 0x8

struct print_flavour {

	using my_canvas_t = renderer::canvas_t<print_flavour>;
	using attribute_type = int;

	char represent(int mask, attribute_type attr) {
		std::vector<std::pair<int, char>> vals;
		if (attr == 2) {
			mvprintw(30,30, "%d", attr);
		 	vals.insert(vals.end(), {
				{ LR | LL | UR | UL, 'H' },
			});
		}
		if (attr == 3) {
			mvprintw(20,20, "%d", attr);
			vals.insert(vals.end(), {
				{ LR | LL | UR | UL, 'Z' },
			});
		}


		vals.insert(vals.end(), {
			{ UL | LL | UR, '/' },
			{ UR | LR | LL, '/' },
			{ UL | LR | LL, '\\' },
			{ UL | UR | LR, '\\' },
			{ UL | LL, '|' },
			{ UL | UR, '^' },
			{ LR | LL, '_' },
			{ UR | LR, '|' },
			{ LL, '.'},
			{ LR, '.'},
			{ UL, '`'},
			{ UR, '\''},
		});
		auto it = std::find_if(vals.begin(), vals.end(), [mask](const auto& pair) {
			return pair.first == mask;
		});
		if (it == vals.end()) {
			return ' ';
		}
		return it->second;
	}

	void render(int x, int y, char repr) {
		mvaddch(y, x, repr);
	}

};
//-----------------------------------------------------------------------------//

int main() {

	scoped_initializer init([]() -> void {
		printf("macska\n");
		initscr();
		noecho();
		cbreak();
	}, []() -> void {
		nocbreak();
		endwin();
		printf("bela\n");
	});

	clear();

	renderer::canvas_t<print_flavour> canvas;
	//renderer::polygon_t p1 {{0,10}, {10,5}, {10,1}};
	//renderer::polygon_t p2 {{1,12}, {12,12}, {12, 18}, {1,18}};
	//renderer::polygon_t p3 {{1,20}, {3,22}, {3, 24}, {4, 25}, {2, 25}};

	renderer::polygon_t tower = {
{1,6},{1,5},{2,5},{2,4},{1,2},{1,1},{12,1},{12,2},{3,2},{3,4},{3,5},{4,5},{4,6},{1,6},
	};
	renderer::polygon_t box = {
{5,3},{5,6},{7,6},{7,5},{6,5},{6,4},{9,4},{9,3},{5,3}
	};

renderer::polygon_t asd = {
{13,1},{13,6},{20,6},{20,1},{13,1}
};
	//canvas.add(p1, 1);
	//canvas.add(p2, 2);
	//canvas.add(p3, 3);
	//canvas.add(tower, 3);
	//canvas.add(box, 2);
	//canvas.add(asd, 2);
	canvas.add({
{13,3},{16,3},{16,6},{13,6},{13,3},
			}, 3);
canvas.add({
{3,1},{6,1},{8,5},{14,7},{14,10},{8,12},{6,16},{3,16},{5,12},{5,10},{3,10},{3,7},{5,7},{5,5},{3,1},
		}, 3);
	canvas.render();
	refresh();

	getchar();

}
