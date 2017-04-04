
#ifndef CURSE_IMPACT_RENDER_HPP
#define CURSE_IMPACT_RENDER_HPP


#include <ncurses.h>
#include "gamefield_t.hpp"
#include "ship.hpp"


void render_gamefield(WINDOW* win, const gamefield_t& gamefield);

void render_ship(WINDOW* win, const ship_t& ship);



#endif // CURSE_IMPLACT_RENDER_HPP
