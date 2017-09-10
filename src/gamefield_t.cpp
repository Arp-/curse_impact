

#include "gamefield_t.hpp"
#include <algorithm>

static bool is_ship_in_bound(
		const rect_t& gf_rect, const position_t& ship_pos, const rect_t& ship_rect) {

	if (gf_rect.width_ < (ship_pos.x_ + ship_rect.width_)) {
		return false;
	}
	if (gf_rect.height_ < (ship_pos.y_ + ship_rect.height_)) { 
		return false;
	}
	if (ship_pos.x_ < 0) {
		return false;
	}
	if (ship_pos.y_ < 0) {
		return false;
	}

	return true;
}
//-----------------------------------------------------------------------------//
static bool is_position_in_bound(
		const rect_t& gf_rect, const position_t& position) {
	
	if (gf_rect.width_ < position.x_) {
		return false;
	}
	if (gf_rect.height_ < position.y_) {
		return false;
	}
	if (position.x_ < 0) {
		return false;
	}
	if (position.y_ < 0) {
		return false;
	}

	return true;
}
//-----------------------------------------------------------------------------//
gamefield_t::gamefield_t(rect_t rect): rect_(rect) {

}
//-----------------------------------------------------------------------------//
const ship_t&
gamefield_t::ship() const {
	return this->ship_;
}
//-----------------------------------------------------------------------------//
const gamefield_t::ship_list_t&
gamefield_t::enemy_list() const {
	return this->enemy_list_;
}
//-----------------------------------------------------------------------------//
const gamefield_t::bullet_list_t&
gamefield_t::bullet_list() const {
	return this->bullet_list_;
}
//-----------------------------------------------------------------------------//
const rect_t&
gamefield_t::rect() const {
	return this->rect_;
}
//-----------------------------------------------------------------------------//
namespace impl {
	// IMPORTANT NOTE!!! This is why I shouldn't always use templates
	// it was a nice finger training though
	enum class dim_t {
		X,
		Y,
	};
	enum class sign_t {
		PLUS,
		MINUS,
	};

	template <dim_t this_dim, dim_t dim_V, sign_t sign>
	struct add {
		static constexpr int value = 0;
	};

	template <dim_t dim_V>
	struct add<dim_V, dim_V, sign_t::PLUS> {
		static constexpr int value = 1;
	};

	template <dim_t dim_V>
	struct add<dim_V, dim_V, sign_t::MINUS> {
		static constexpr int value = -1;
	};


	template <instruction_t::movement_t inst_V,
					 impl::dim_t dim_V, impl::sign_t sign_V>
	inline void move_helper(
			const rect_t& gf_rect, ship_t& ship, instruction_t::movement_t inst) {

		const auto& sh_pos = ship.position();
		const auto& sh_rect = ship.rect();

		if (inst == inst_V) {
			position_t pos = { sh_pos.x_ + impl::add<dim_t::X, dim_V, sign_V>::value,
				sh_pos.y_ + impl::add<dim_t::Y, dim_V, sign_V>::value };
			if (is_ship_in_bound(gf_rect, pos, sh_rect)) {
				ship.position(pos);
			}
		}
	}
}
//-----------------------------------------------------------------------------//
void 
gamefield_t::move_ship(instruction_t instruction) {
	using movement_t = instruction_t::movement_t;
	auto movement = instruction.movement;

	impl::move_helper<movement_t::UP, impl::dim_t::Y, impl::sign_t::MINUS>(
			this->rect_, this->ship_, movement);
	impl::move_helper<movement_t::DOWN, impl::dim_t::Y, impl::sign_t::PLUS>(
			this->rect_, this->ship_, movement);
	impl::move_helper<movement_t::FORWARD, impl::dim_t::X, impl::sign_t::PLUS>(
			this->rect_, this->ship_, movement);
	impl::move_helper<movement_t::BACKWARD, impl::dim_t::X, impl::sign_t::MINUS>(
			this->rect_, this->ship_, movement);

}
//-----------------------------------------------------------------------------//
void
gamefield_t::move_enemy(int ship_id, ship_event_t::direction dir) {
	auto it = this->find_enemy(ship_id);
	if (it == this->enemy_list_.end()) { return; }
	it->move(dir, this->rect());
}
//-----------------------------------------------------------------------------//
void
gamefield_t::ship_shoot() {
	const auto& ship_pos = this->ship_.position();
	const auto& ship_rect = this->ship_.rect();
	bullet_t bullet { ship_pos.x_ + ship_rect.width_ +1,
		ship_pos.y_ + (ship_rect.height_ >> 1), this->ship_.speed() +1 };
	this->bullet_list_.push_back(bullet);
}
//-----------------------------------------------------------------------------//
void
gamefield_t::enemy_shoot(int ship_id) {
	auto it = find_enemy(ship_id);
	if (it == this->enemy_list_.end()) { return; }
	const auto& ship_pos = it->position();
	const auto& ship_rect = it->rect();
	bullet_t bullet { ship_pos.x_ - 1,
		ship_pos.y_ + (ship_rect.height_ >> 1), it->speed() -1 };
	this->bullet_list_.push_back(bullet);
}
//-----------------------------------------------------------------------------//
void 
gamefield_t::bullet_list_tick() {
	// given a tick in the timer the list will move forward according to it's
	// speed
	for (auto& bullet : this->bullet_list_) {
		bullet.tick();
	}
	// remove last element if out of bounds;
	// have to copy because the pop would make a dnalging reference
	// and it looks like thats why i segfault
	// or maybe not
	if (!this->bullet_list_.empty()) {
		auto last_pos = this->bullet_list_.front().position();
		if (!is_position_in_bound(this->rect_, last_pos)) {
			this->bullet_list_.pop_front();
		}
	}
}
//-----------------------------------------------------------------------------//
void 
gamefield_t::add_enemy(const ship_t& enemy) {
	this->enemy_list_.push_back(enemy);
}
//-----------------------------------------------------------------------------//
void
gamefield_t::enemy_list_tick() {
	for (auto& ship : this->enemy_list_) {
		ship.tick();
	}

	if (!this->enemy_list_.empty()) {
		const auto& last_ship = this->enemy_list_.front();
		if (!is_ship_in_bound(this->rect_, last_ship.position(), last_ship.rect())) {
			this->enemy_list_.pop_front();
		}
	}
}
//-----------------------------------------------------------------------------//
static bool is_position_in_rect(const position_t& bullet_pos, 
		const position_t& rect_pos, const rect_t& rect) {
		
	if (bullet_pos.x_ >= rect_pos.x_ + rect.width_) {
		return false;
	}
	if (bullet_pos.y_ >= rect_pos.y_ + rect.height_) {
		return false;
	}
	if (bullet_pos.y_ < rect_pos.y_) {
		return false;
	}
	if (bullet_pos.x_ < rect_pos.x_) {
		return false;
	}
	return true;
}
//-----------------------------------------------------------------------------//
static bool is_rect_overlapping(const position_t& pos1, const rect_t& rect1,
		const position_t& pos2, const rect_t& rect2) {

	for (int x = 0; x < rect1.width_; x++) {
		for (int y = 0; y < rect1.width_; y++) {
			if (is_position_in_rect({ x + pos1.x_, y + pos1.y_}, pos2, rect2)) {
				return true;
			}
		}
	}
	return false;
}
//-----------------------------------------------------------------------------//
template <typename bullet_T, typename enemy_T, typename bullet_it_T,
				 typename enemy_it_T>
static void 
handle_ship_bullet_check(
		bullet_T& bullets_to_delete,
		enemy_T& enemies_to_delete,
		bullet_it_T& bullet_it,
		enemy_it_T& enemy_it) {

	if (is_position_in_rect(bullet_it->position(),
				enemy_it->position(), enemy_it->rect())) {
		bullets_to_delete.push_back(bullet_it);
		enemy_it->hp_decrement();
		if (enemy_it->hp() <= 0) {
			enemies_to_delete.push_back(enemy_it);
		}
	}
}
//-----------------------------------------------------------------------------//
template <typename enemy_list_T, typename enemy_it_T>
static void
handle_player_enemy_check(
		enemy_list_T& enemies_to_delete, 
		enemy_it_T& enemy_it,
		ship_t& ship) {

	if (is_rect_overlapping(enemy_it->position(), enemy_it->rect(),
				ship.position(), ship.rect())) {
		enemy_it->hp_decrement();
		// handle player death in the main game loop for now, only decrement it here
		ship.hp_decrement(); 
		if (enemy_it->hp() <= 0) {
			enemies_to_delete.push_back(enemy_it);
		}
	}
}
//-----------------------------------------------------------------------------//
template <typename bullet_list_T, typename bullet_it_T>
static void
handle_player_bullet_check(bullet_list_T& bullets_to_delete,
		bullet_it_T& bullet_it,
		ship_t& ship) {

	if (is_position_in_rect(bullet_it->position(), ship.position(), ship.rect())) {
		bullets_to_delete.push_back(bullet_it);
		ship.hp_decrement();
	}
}
//-----------------------------------------------------------------------------//
void
gamefield_t::hitcheck() {
	// should check collision agains walls,
	// should check collision against enemies;
	// should check collision against bullets;
	std::vector<bullet_list_t::iterator> bullets_to_delete;
	std::vector<ship_list_t::iterator> enemies_to_delete;

	for (auto it = this->enemy_list_.begin();
			it != this->enemy_list_.end(); ++it) {

		for (auto bullet_it = this->bullet_list_.begin();
				bullet_it != this->bullet_list_.end(); ++bullet_it) {

			handle_ship_bullet_check(
					bullets_to_delete, enemies_to_delete, bullet_it, it);

		}

		handle_player_enemy_check(enemies_to_delete, it, this->ship_);
	}

	for (auto bullet_it = this->bullet_list_.begin();
			bullet_it != this->bullet_list_.end(); ++bullet_it) {

		handle_player_bullet_check(bullets_to_delete, bullet_it, this->ship_);
	}



	for (auto it : bullets_to_delete) {
		this->bullet_list_.erase(it);
	}
	for (auto it : enemies_to_delete) {
		this->enemy_list_.erase(it);
	}
}
//-----------------------------------------------------------------------------//
gamefield_t::ship_list_t::iterator
gamefield_t::find_enemy(int ship_id) {
	return std::find_if(this->enemy_list_.begin(), this->enemy_list_.end(),
			[ship_id](const ship_t& ship) { return ship.id() == ship_id; });
}



