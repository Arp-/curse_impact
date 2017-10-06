

#include <iostream>
#include "gamefield_t.hpp"
#include <ncurses.h>
#include <unistd.h>
#include "instruction_t.hpp"
#include "pugixml.hpp"
#include "script_t.hpp"
#include "texture_t.hpp"

#ifndef RES_DIR
#	define RES_DIR "res"
#endif



using ship_texture_t = std::array<std::array<char, 3>, 3>;
using bullet_texture_t = char;
//-----------------------------------------------------------------------------//
// STATIC CONSTANTS
//-----------------------------------------------------------------------------//
// FUNCTIONS FOR RENDERING
//static void print_texture(const texture_t& texture) {
//	const auto& cont = texture.matrix();
//	for (const auto& row : cont) {
//		for (const auto& elem : row) {
//			putc(elem, stdout);
//		}
//		putc('\n',stdout);
//	}
//}
//-----------------------------------------------------------------------------//
static void render_ship(const ship_t& ship, const texture_t& texture) {
	const auto& rect =  ship.rect();
	const auto& position = ship.position();
	//std::cerr << "ship: " << ship << std::endl;

	for (int x = 0; x < rect.width_; x++) {
		for (int y = 0; y < rect.height_; y++) {
			move(2,1);
			printw("x: %d y: %d\n", x, y);
			move(3,1);
			printw("size_x: %d, size_y: %d\n", texture.matrix().size(), texture[x].size());
			auto tx = texture[y][x];
			
			mvaddch(position.y_ + y, position.x_ + x, tx);
		}
	}
}
//-----------------------------------------------------------------------------//
static void render_clean_ship(const ship_t& ship) {
		const auto& rect = ship.rect();
		const auto& position = ship.position();

		for (int x = 0; x < rect.width_; x++) {
			for (int y = 0; y < rect.height_; y++) {
				mvaddch(position.y_ + y, position.x_ + x, ' ');
			}
		}
}
//-----------------------------------------------------------------------------//
//static void render_gamefield(const gamefield_t& gamefield) {
//
//	const auto& rect = gamefield.rect();
//
//	for (int i = 0; i < rect.width_; i++) {
//		mvaddch(0,i, 'X');
//		mvaddch(rect.height_-1, i, 'X');
//	}
//	for (int i = 0; i < rect.height_; i++) {
//		mvaddch(i, 0, 'X');
//		mvaddch(i, rect.width_-1, 'X');
//	}
//
//	const auto& ship = (gamefield.ship());
//	texture_t t = {{
//			{ 'Y', 'Y', 'Y'},
//			{ 'Y', 'Y', 'Y'},
//			{ 'Y', 'Y', 'Y'},
//	}};
//	render_ship(ship, t);
//
//	refresh();
//}
//-----------------------------------------------------------------------------//
static instruction_t get_instruction() {
	using movement_t = instruction_t::movement_t;
	using attack_t = instruction_t::attack_t;
	using other_t = instruction_t::other_t;

	instruction_t instr;
	instr.movement = movement_t::NOP;
	instr.attack = attack_t::NOP;
	instr.other = other_t::NOP;

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
static void render_bullet_list(const gamefield_t::bullet_list_t& bullet_list, const char texture) {
	for (const auto& bullet : bullet_list) {
		const auto& position = bullet.position();
		mvaddch(position.y_, position.x_, texture);
	}
}
//-----------------------------------------------------------------------------//
static void render_ship_list(const gamefield_t::ship_list_t& ship_list, 
		const script_t::texture_ship_assoc_t& assoc_table) {
	for (const auto& ship : ship_list) {
		const auto& texture = assoc_table.at(ship.id());
		render_ship(ship, texture);
	}
}
//-----------------------------------------------------------------------------//
static void render_clean_ship_list(const gamefield_t::ship_list_t& ship_list) {
	for (const auto& ship : ship_list) {
		render_clean_ship(ship);
	}
}
//-----------------------------------------------------------------------------//
static void render_clean(gamefield_t& gf) {
	static texture_t clear_texture = {{
		{ ' ', ' ', ' ' },
		{ ' ', ' ', ' ' },
		{ ' ', ' ', ' ' },
	}};
	const auto& ship = gf.player();
	render_ship(ship, clear_texture);
	render_bullet_list(gf.bullet_list(), ' ');
	render_bullet_list(gf.enemy_bullet_list(), ' ');
	render_clean_ship_list(gf.enemy_list());
}
//-----------------------------------------------------------------------------//
static void game_logic(script_t& script, gamefield_t& gf, instruction_t instruction) {
	auto prev_gf = gf;
	script.tick();
	gf.move_ship(instruction);
	gf.bullet_list_tick();
	gf.enemy_list_tick();
	if (instruction.attack == instruction_t::attack_t::SHOOT) {
		gf.player_shoot();
	}
	gf.hitcheck(prev_gf);
}
//-----------------------------------------------------------------------------//
static void render(gamefield_t& gf,
		const script_t::texture_ship_assoc_t& assoc_table, const texture_t& player_texture) {

	render_ship(gf.player(), player_texture);
	render_bullet_list(gf.bullet_list(), '-');
	render_bullet_list(gf.enemy_bullet_list(), '*');
	render_ship_list(gf.enemy_list(), assoc_table);
}
//-----------------------------------------------------------------------------//
static texture_t
load_player_texture(const std::string path) {

	try {
		return texture_t::read_from_file(path);
	} catch (std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
	}

	std::cerr << "Defaulting player ship to initial value!" << std::endl;
	return texture_t {{
		{ 'X', 'X', 'X' },
		{ 'X', 'X', 'X' },
		{ 'X', 'X', 'X' },
	}};
}
//-----------------------------------------------------------------------------//
static void print_gameover() {
	move(2,2);
	printw("gameover");
	refresh();
	sleep(1);
}
//-----------------------------------------------------------------------------//
static void game() {

	gamefield_t gf({140,40});
	script_t script { gf, RES_DIR };
	script.read_xml(RES_DIR "/game_script.xml");
	gf.set_player(
			position_t {3,3},
			rect_t {3,3},
			0,
			3,
			0);

	initscr();
	noecho();
	cbreak();
	timeout(0);


	//ship_t enemy {{ 60, 10 }, { 3, 3 }, -1, 10};
	//gf.add_enemy(enemy);
	//render_gamefield(gf);
	auto&& player_texture = 
		load_player_texture(RES_DIR "/texture/player_ship.txt");

	while (true) {
		instruction_t instruction = get_instruction();
		if (instruction.other == instruction_t::other_t::QUIT) {
			break;
		}
		if (gf.player().hp() <= 0) {
			print_gameover();
			break;
		}
		// if ( no_more_enemies_are_on_the_field) {
		//   run_level_finish_animation();
		//   if (!has_next_level()) {
		//     print_victory();
		//     break;  // end game with some victory stuff
		//   }
		//   load_next_level();
		// }
		move(1,1);
		//printw("instruction %02X, %02X", (int)instruction.movement, (int)instruction.attack );
		printw("hp: %d\n", gf.player().hp());
		render_clean(gf);
		game_logic(script, gf, instruction);
		render(gf, script.texture_ship_assoc(), player_texture);
		refresh();
		usleep(100000);
	}

	nocbreak();
	endwin();

}
//-----------------------------------------------------------------------------//
int main() {

	//pugi::xml_document doc;
	//pugi::xml_parse_result result = doc.load_file("res/game_script.xml");
	//std::cout << "Load result: " << result.description() << std::endl;

	//std::cout << "hp: " << doc.child("level").child("enemy").attribute("hp").value() << std::endl;


	game();
	//
	//texture_t textureA = texture_t::read_from_file("./res/texture/enemy_A.txt");
	//print_texture(textureA);
	//texture_t textureB = texture_t::read_from_file("./res/texture/enemy_B.txt");
	//print_texture(textureB);
	//gamefield_t gf({100,40});
	//script_t script { gf };
	//script.read_xml("res/game_script.xml");

}
