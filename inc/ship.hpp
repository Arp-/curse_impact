
#ifndef CURSE_IMPACT_SHIP_HPP
#define CURSE_IMPACT_SHIP_HPP


#include <vector>

#include "choord_t.hpp"
#include "image_t.hpp"

#include <ncurses.h> // fuck it i couldnot decuple the include


class gamefield_t; // 4wrd declr

class ship_t {

	friend void render_ship(WINDOW* win, const ship_t&);

	public: //-- public functions --//

		ship_t(gamefield_t& gamefield);

		~ship_t() = default;

	private: //-- private varaibles --//

		gamefield_t& gamefield_;

		choord_t choord_;

		std::vector<image_t<3,3>> image_list_;

};

#endif // CURSE_IMPACT_SHIP_HPP
