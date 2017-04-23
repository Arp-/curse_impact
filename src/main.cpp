

#include <iostream>
#include "gamefield_t.hpp"
#include <ncurses.h>
#include <unistd.h>
#include "instruction_t.hpp"


#define MOVEMENT_TICK 1

using ship_texture_t = std::array<std::array<char, 3>, 3>;
using bullet_texture_t = char;

//-----------------------------------------------------------------------------//
static void render_ship(const ship_t& ship, const ship_texture_t& texture) {
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
	ship_texture_t t = {{
			{ 'Y', 'Y', 'Y'},
			{ 'Y', 'Y', 'Y'},
			{ 'Y', 'Y', 'Y'},
	}};
	render_ship(ship, t);

	refresh();
}
//-----------------------------------------------------------------------------//
static instruction_t get_instruction() {
	using movement_t = instruction_t::movement_t;
	using attack_t = instruction_t::attack_t;
	using other_t = instruction_t::other_t;

	instruction_t instr;
	instr.movement = movement_t::NOP;
	instr.attack = attack_t::NOP;

	char ch = 0;
	do {
		ch = getch();
		switch (ch) {
			case 'k':
				instr.movement = movement_t::UP;
				break;
			case 'j': 
				instr.movement = movement_t::DOWN;
				break;
			case 'h':
				instr.movement = movement_t::BACKWARD;
				break;
			case 'l':
				instr.movement =  movement_t::FORWARD;
				break;
		}
		switch (ch) {
			case ' ':
				instr.attack = attack_t::SHOOT;
				break;
		}
		switch (ch) {
			case 'q':
				instr.other = other_t::QUIT;
		}
	} while (ERR != ch);

	return instr;
}
//-----------------------------------------------------------------------------//
static void render_enemy_list(gamefield_t& gf) {
	ship_texture_t clear_texture = {{
		{ ' ', ' ', ' ' },
		{ ' ', ' ', ' ' },
		{ ' ', ' ', ' ' },
	}};
	ship_texture_t ship_texture  = {{
		{ ' ', 'e', ' ' },
		{ 'e', 'e', 'e' },
		{ ' ', 'e', ' ' },
	}};
	for (const auto& ship : gf.enemy_list()) {
		render_ship(ship, clear_texture);
	}
	gf.enemy_list_tick();
	for (const auto& ship : gf.enemy_list()) {
		render_ship(ship, ship_texture);
	}
}
//-----------------------------------------------------------------------------//
static void render_movement(gamefield_t& gf, instruction_t instruction) {
	ship_texture_t clear_texture = {{
		{ ' ', ' ', ' ' },
		{ ' ', ' ', ' ' },
		{ ' ', ' ', ' ' },
	}};
	ship_texture_t ship_texture  = {{
			{ '|', '\\', ' '},
			{ '}', ']', '>'},
			{ '|', '/', ' '},
	}};
	const auto& ship = gf.ship();
	render_ship(ship, clear_texture);
	gf.move_ship(instruction);
	render_ship(ship, ship_texture);
	render_enemy_list(gf);
}
//-----------------------------------------------------------------------------//
static void render_bullet_list(const gamefield_t::bullet_list_t& bullet_list, const char texture) {
	for (const auto& bullet : bullet_list) {
		const auto& position = bullet.position();
		mvaddch(position.y_, position.x_, texture);
	}
}
//-----------------------------------------------------------------------------//
static void render_shoot(gamefield_t& gf, instruction_t instruction) {

	render_bullet_list(gf.bullet_list(), ' ');
	gf.bullet_list_tick();
	if (instruction.attack == instruction_t::attack_t::SHOOT) {
		// Add the bullet to the bullet list with the ending of the current ship
		gf.ship_shoot();
	}
	render_bullet_list(gf.bullet_list(), '-');
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
	cbreak();
	timeout(0);


	ship_t enemy {{ 10, 10 }, { 3, 3 }, 0, 10};
	gf.add_enemy(enemy);
	render_gamefield(gf);

	while (true) {
		instruction_t instruction = get_instruction();
		if (instruction.other == instruction_t::other_t::QUIT) {
			break;
		}
		move(1,1);
		printw("instruction %02X, %02X", (int)instruction.movement, (int)instruction.attack );
		render_shoot(gf, instruction);
		render_movement(gf, instruction);
		gf.hitcheck();
		refresh();
		usleep(100000);
	}

	nocbreak();
	endwin();
	return 0;
}
