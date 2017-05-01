

#ifndef CURSE_IMPACT_SHIP_T
#define CURSE_IMPACT_SHIP_T

#include "rect_t.hpp"
#include "position_t.hpp"


class ship_t {

	public: //-- public functions --//

		ship_t();

		ship_t(position_t pos, rect_t rect, int speed);

		ship_t(position_t pos, rect_t rect, int speed, int hp);

		ship_t(const ship_t& ship) = default;

		~ship_t() = default;

		const rect_t& rect() const;
		
		const position_t& position() const;

		int id() const;

		int speed() const;

		int hp() const;

		void hp_decrement();

		void position(const position_t& position);

		void hp(int hp);

		void tick();



	private: //-- private functions --//

		position_t position_;

		rect_t rect_;

		int speed_;

		int hp_;

		int id_;

};


#endif // CURSE_IMPACT_SHIP_T
