

#ifndef CURSE_IMPACT_GAMEFIELD_T_HPP
#define CURSE_IMPACT_GAMEFIELD_T_HPP

#include "ship.hpp"

#include <ncurses.h> // fuck it i couldnot decuple the include

class gamefield_t {

	friend void render_gamefield(WINDOW* win, const gamefield_t&);


	public: //-- public functions --//

		gamefield_t(int width, int height);

		~gamefield_t();


	private: //-- private variables --//

		int width_;

		int height_;

		int time_; // the current time of the rendered playfield, at the end we reach the boss

		ship_t ship_;


};

#endif // CURSE_IMPACT_GAMEFIELD_T_HPP
