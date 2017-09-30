
#ifndef CURSE_IMPACT_BULLET_T_HPP
#define CURSE_IMPACT_BULLET_T_HPP

#include "unique.hpp"
#include "position_t.hpp"

class bullet_t : public unique<bullet_t> {

	public: //-- public functions --//

		bullet_t() = default;

		bullet_t(int x, int y, int speed);

		~bullet_t() = default;

		void tick();

		const position_t& position() const;

		int speed() const;


	private: //-- private functions --//

		position_t position_;

		int speed_;

};


#endif // CURSE_IMPACT_BULLET_T_HPP
