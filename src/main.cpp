

#include <iostream>
#include "gamefield_t.hpp"
#include <ncurses.h>
#include <unistd.h>
#include "instruction_t.hpp"
#include "pugixml/pugixml.hpp"
#include "script_t.hpp"
#include "texture_t.hpp"
#include "display/healthbar_t.hpp"
#include "display/border_drawer_t.hpp"
#include "renderer/test.hpp"

#ifndef RES_DIR
#	define RES_DIR "res"
#endif

//-----------------------------------------------------------------------------//
// STATIC CONSTANTS
//-----------------------------------------------------------------------------//
static position_t DRAW_START = { 2, 4 };
//-----------------------------------------------------------------------------//
// FUNCTIONS FOR RENDERING
//-----------------------------------------------------------------------------//
static void render_ship(const ship_t& ship, const texture_t& texture) {
	const auto& rect =  ship.rect();
	const auto& position = ship.position();
	//std::cerr << "ship: " << ship << std::endl;

	for (int x = 0; x < rect.width_; x++) {
		for (int y = 0; y < rect.height_; y++) {
			//move(2,1);
			//printw("x: %d y: %d\n", x, y);
			//move(3,1);
			//printw("size_x: %d, size_y: %d\n", texture.matrix().size(), texture[x].size());
			auto tx = texture[y][x];
			
			mvaddch(position.y_ + y + DRAW_START.y_,
					position.x_ + x + DRAW_START.x_, tx);
		}
	}
}
//-----------------------------------------------------------------------------//
static void render_clean_ship(const ship_t& ship) {
		const auto& rect = ship.rect();
		const auto& position = ship.position();

		for (int x = 0; x < rect.width_; x++) {
			for (int y = 0; y < rect.height_; y++) {
				mvaddch(position.y_ + y + DRAW_START.y_,
						position.x_ + x + DRAW_START.x_, ' ');
			}
		}
}
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
		mvaddch(position.y_ + DRAW_START.y_, position.x_ + DRAW_START.x_, texture);
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
static position_t get_middle_choords(WINDOW* win) {
	position_t pos;
	getmaxyx(win, pos.y_, pos.x_);
	pos.x_ >>= 1;
	pos.y_ >>= 1;
	return pos;
}
//-----------------------------------------------------------------------------//
static void print_gameover(WINDOW* win) {
	// TODO ascii art
	wclear(win);
	position_t pos = get_middle_choords(win);
	mvwprintw(win, pos.y_, pos.x_,"GAME OVER (BITCH)");
	refresh();
	sleep(1);
}
//-----------------------------------------------------------------------------//
static void run_level_finish_animation(WINDOW* win) {
	wclear(win);
	position_t pos = get_middle_choords(win);
	mvwprintw(win, pos.y_, pos.x_, "VICTORY");
	refresh();
	sleep(1);

}
//-----------------------------------------------------------------------------//
static void game() {

	gamefield_t gf({140,30});
	script_t script { gf, RES_DIR };
	script.read_xml(RES_DIR "/game_script.xml");
	gf.set_player(
			position_t {3,3},
			rect_t {3,3},
			0,
			3,
			0);

	util::scoped_initializer init([]() -> void {
		initscr();
		noecho();
		cbreak();
		timeout(0);
	}, []() -> void {
		nocbreak();
		endwin();
	});


	//ship_t enemy {{ 60, 10 }, { 3, 3 }, -1, 10};
	//gf.add_enemy(enemy);
	//render_gamefield(gf);
	auto&& player_texture = 
		load_player_texture(RES_DIR "/texture/player_ship.txt");

	display::healthbar_t healthbar(stdscr, { 0, 0 });
	//wborder(stdscr, '|', '|', '_', '_', '.', '.', '.', '.');
	auto&& border_drawer = display::border_drawer_t::builder()
		.position({DRAW_START.x_ -1 , DRAW_START.y_ -1})
		.rect({ gf.rect().width_+2, gf.rect().height_+2 })
		.window(stdscr)
		.style(' ','_',' ')
		.build();

	if (!border_drawer) {
		fprintf(stderr, "I fucked up the border_drawer");
		exit(1);
	}


	while (true) {
		instruction_t instruction = get_instruction();
		if (instruction.other == instruction_t::other_t::QUIT) {
			break;
		}
		if (gf.player().hp() <= 0) {
			print_gameover(stdscr);
			break;
		}
		 if (script.end() && gf.enemy_list().empty()) {
		   run_level_finish_animation(stdscr);
			 break;
		   //if (!has_next_level()) {
		   //  print_victory();
		   //  break;  // end game with some victory stuff
		   //}
		   //load_next_level();
		 }
		//printw("instruction %02X, %02X", (int)instruction.movement, (int)instruction.attack );
		//printw("hp: %d\n", gf.player().hp());
		healthbar.update(gf.player().hp());
		healthbar.display();
		render_clean(gf);
		game_logic(script, gf, instruction);
		render(gf, script.texture_ship_assoc(), player_texture);
		border_drawer->draw();
		refresh();
		usleep(100000);
	}


}
//-----------------------------------------------------------------------------//
int main() {


	game();

}
