#ifndef CURSE_IMPACT_SHIP_T
#define CURSE_IMPACT_SHIP_T

#include "rect_t.hpp"
#include "position_t.hpp"
#include "event_t.hpp"


class ship_t {

	public: //-- public functions --//

		ship_t();

		ship_t(position_t pos, rect_t rect, int speed, int hp, int id);

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

		void move(ship_event_t::direction dir, const rect_t& gf_rect);

		void tick();

		position_t front_center();

		friend std::ostream& operator<<(std::ostream&, const ship_t&);



	private: //-- private functions --//

		position_t position_;

		rect_t rect_;

		int speed_;

		int hp_;

		int id_;

};

std::ostream& operator<<(std::ostream& out, const position_t& pos);

std::ostream& operator<<(std::ostream& out, const ship_t& ship);


#endif // CURSE_IMPACT_SHIP_T
