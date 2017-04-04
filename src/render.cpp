

#include "render.hpp"

void render_gamefield(WINDOW* win, const gamefield_t& gamefield) {
	render_ship(win, gamefield.ship_);
}
//-----------------------------------------------------------------------------//
void render_ship(WINDOW* win, const ship_t& ship) {
	int x = ship.choord_.x;
	int y = ship.choord_.y;
	for (int i = 0; i < 3; i++) {
		for (int j = 0 ;j < 3; j++) {
			mvwprintw(win, y+i, x, "XXX");
		}
	}
	//printf("macska\n");
}
