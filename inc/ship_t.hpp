

#ifndef CURSE_IMPACT_SHIP_T
#define CURSE_IMPACT_SHIP_T

#include "rect_t.hpp"
#include "position_t.hpp"


class ship_t {

	public: //-- public functions --//

		ship_t() = default;

		ship_t(position_t pos, rect_t rect, int speed);

		ship_t(const ship_t& ship) = default;

		~ship_t() = default;

		const rect_t& rect() const;
		
		const position_t& position() const;

		void position(const position_t& position);


	private: //-- private functions --//

		position_t position_;

		rect_t rect_;

		int speed_;

};


#endif // CURSE_IMPACT_SHIP_T
