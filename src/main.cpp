

#include "ncurses.h"
#include "render.hpp"



int main() {

	initscr();
	noecho();

	gamefield_t gamefield { 10, 10 }; 

	render_gamefield(stdscr, gamefield);
	refresh();


	getch();
	endwin();
	return 0;
}
