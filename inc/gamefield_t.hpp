

#ifndef CURSE_IMPACT_GAMEFIELD_HPP
#define CURSE_IMPACT_GAMEFIELD_HPP

#include <vector>
#include <memory>
#include "ship_t.hpp"
#include "bullet_t.hpp"
#include "rect_t.hpp"
#include "instruction_t.hpp"

class gamefield_t {

	using ship_list_t = std::vector<ship_t>;
	using bullet_list_t = std::vector<bullet_t>;

	public: //-- public functions --//

		gamefield_t(rect_t);

		~gamefield_t() = default;

		template <typename ...Args>
		void set_ship(Args&& ... args) {
			this->ship_ = ship_t { args... };
		}

		template <typename ...Args>
		void emplace_enemy_ship(Args&& ...args) {
			this->enemy_list_.emplace_back(args...);
		}


		const ship_t& ship() const;

		const std::vector<ship_t>& enemy_list() const;

		const std::vector<bullet_t>& bullet_list() const;

		const rect_t& rect() const;

		void move_ship(instruction_t instruction);


	private: //-- private stuff --// 

		int time_;

		rect_t rect_;

		ship_t ship_;

		ship_list_t enemy_list_;

		bullet_list_t bullet_list_;

};


#endif // CURSE_IMPACT_GAMEFIELD_HPP
