

#ifndef CURSE_IMPACT_GAMEFIELD_HPP
#define CURSE_IMPACT_GAMEFIELD_HPP

#include <vector>
#include <tuple>
#include <deque>
#include <memory>
#include "ship_t.hpp"
#include "bullet_t.hpp"
#include "rect_t.hpp"
#include "instruction_t.hpp"
#include "event_t.hpp"

class gamefield_t {

	public: 

		using ship_list_t = std::deque<ship_t>;
		using bullet_list_t = std::deque<bullet_t>;
		using enemy_bullet_id_t = unsigned;
		using bullet_id_t = unsigned;
		using distance_vec_t = std::vector<
			std::tuple<enemy_bullet_id_t,bullet_id_t,position_t>>;

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

		const ship_list_t& enemy_list() const;

		const bullet_list_t& bullet_list() const;

		const bullet_list_t& enemy_bullet_list() const;

		const rect_t& rect() const;

		void move_ship(instruction_t instruction);

		void move_enemy(int ship_id, ship_event_t::direction dir);

		void ship_shoot();

		void enemy_shoot(int ship_id);

		void bullet_list_tick();

		void add_enemy(const ship_t& enemy);

		void enemy_list_tick();

		void hitcheck(const gamefield_t& prev_gf);


	private: //-- private functions --//

		ship_list_t::iterator find_enemy(int ship_id);

		const distance_vec_t bullet_distance() const;


	private: //-- private stuff --// 

//		int time_;

		rect_t rect_;

		ship_t ship_;

		ship_list_t enemy_list_;

		bullet_list_t bullet_list_;

		bullet_list_t enemy_bullet_list_;

};


#endif // CURSE_IMPACT_GAMEFIELD_HPP
