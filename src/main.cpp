

#include <iostream>
#include "gamefield_t.hpp"
#include <ncurses.h>
#include <unistd.h>
#include "instruction_t.hpp"


#define MOVEMENT_TICK 1

using texture_t = std::array<std::array<char, 3>, 3>;

//-----------------------------------------------------------------------------//
static void render_ship(const ship_t& ship, const texture_t& texture) {
	const auto& rect =  ship.rect();
	const auto& position = ship.position();

	for (int x = 0; x < rect.width_; x++) {
		for (int y = 0; y < rect.height_; y++) {
			mvaddch(position.y_ + y, position.x_ + x, texture[y][x]);
		}
	}

}
//-----------------------------------------------------------------------------//
static void render_gamefield(const gamefield_t& gamefield) {

	const auto& rect = gamefield.rect();

	for (int i = 0; i < rect.width_; i++) {
		mvaddch(0,i, 'X');
		mvaddch(rect.height_-1, i, 'X');
	}
	for (int i = 0; i < rect.height_; i++) {
		mvaddch(i,0, 'X');
		mvaddch(i, rect.width_-1, 'X');
	}

	const auto& ship = (gamefield.ship());
	texture_t t = {{
			{ 'Y', 'Y', 'Y'},
			{ 'Y', 'Y', 'Y'},
			{ 'Y', 'Y', 'Y'},
	}};
	render_ship(ship, t);

	refresh();
}
//-----------------------------------------------------------------------------//
static instruction_t get_instruction() {
	int move = getch();
	switch (move) {
		case 'i':
			return instruction_t::UP;
		case 'k': 
			return instruction_t::DOWN;
		case 'j':
			return instruction_t::BACKWARD;
		case 'l':
			return instruction_t::FORWARD;
		case ' ':
			return instruction_t::SHOOT;
	}

	return instruction_t::NOP;
}
//-----------------------------------------------------------------------------//
static void render_movement(gamefield_t& gf, instruction_t instruction) {
	texture_t clear_texture = {{
		{ ' ', ' ', ' ' },
		{ ' ', ' ', ' ' },
		{ ' ', ' ', ' ' },
	}};
	texture_t ship_texture  = {{
			{ 'Y', 'Y', 'Y'},
			{ 'Y', 'Y', 'Y'},
			{ 'Y', 'Y', 'Y'},
	}};
	const auto& ship = gf.ship();
	render_ship(ship, clear_texture);
	gf.move_ship(instruction);
	render_ship(ship, ship_texture);
}
//-----------------------------------------------------------------------------//
int main() {

	gamefield_t gf({100,40});
	gf.set_ship(
			position_t {3,3},
			rect_t {3,3},
			0);

	initscr();
	noecho();

	render_gamefield(gf);
	sleep(1);

	while (true) {
		instruction_t instruction = get_instruction();
		render_movement(gf, instruction);
		refresh();
	}

	endwin();
	return 0;
}
