
#ifndef CURSE_IMPACT_HEALTHBAR_HPP
#define CURSE_IMPACT_HEALTHBAR_HPP

#include <ncurses.h>
#include <iomanip>
#include <vector>
#include "position_t.hpp"

namespace display {

class healthbar_t {

	public: //-- public functions --//

		healthbar_t(WINDOW* win);

		healthbar_t(WINDOW* win, const position_t& pos);

		~healthbar_t() = default;

		void update(int hp);

		void position(const position_t& pos);

		const position_t& position() const;

		void display();

	private:

		void init_texture();


	private: //-- private members --//

		WINDOW* win_;

		position_t position_;

		std::vector<std::string> texture_;

		int cached_hp_;


};

}

#endif // CURSE_IMPACT_HEALTHBAR_HPP
