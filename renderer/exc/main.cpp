#include <iostream>
#include <ncurses.h>
#include <functional>
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

struct print_flavour {

	using my_canvas_t = renderer::canvas_t<print_flavour>;
	using attribute_type = int;

	static bool it_contains_at_least_one(int x, int y, polygon_t p) {
		return point_inclusion(p, {x, y})
			|| point_inclusion(p, {x+1,y})
			|| point_inclusion(p, {x,y+1})
			|| point_inclusion(p, {x+1,y+1});
	}

	std::string render(const my_canvas_t& canvas) {
		my_canvas_t::rect_t rect = *canvas.containing_rect();
		auto poly = canvas.begin()->first;
		mvprintw(20, 0, "first: %d, %d\n", rect.first.x, rect.first.y);
		mvprintw(21, 0, "second: %d, %d\n", rect.second.x, rect.second.y);
		for (int x = rect.first.x; x < rect.second.x; ++x) {
			for (int y = rect.first.y; y < rect.second.y; ++y) {
				if (it_contains_at_least_one(x,y,poly)) {
					mvaddch(y+1, x+1, 'X');
				}
			}
		}
		return "";
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
	renderer::polygon_t p1 {{0,10}, {10,5}, {10,0}};
	canvas.add(p1, 1);
	print_flavour p;
	p.render(canvas);
	refresh();

	getchar();

}
