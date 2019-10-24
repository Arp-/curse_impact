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
		static std::vector<std::pair<int, char>> vals {
			{ LR | LL | UR | UL, ' ' },
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
		};
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
		{0,0},
		{0,1},
		{2,2},
		{2,4},
		{1,5},
		{2,6},
		{4,6},
		{5,5},
		{4,4},
		{2,4},
		{6,1},
		{6,0},
	};
	//canvas.add(p1, 1);
	//canvas.add(p2, 2);
	//canvas.add(p3, 3);
	canvas.add(tower, 1);
	canvas.render();
	refresh();

	getchar();

}
