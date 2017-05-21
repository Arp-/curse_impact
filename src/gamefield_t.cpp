

#include "gamefield_t.hpp"

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
	//const auto& sh_pos = this->ship_.position();
	//const auto& sh_rect = this->ship_.rect();
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
	//if (instruction_t::UP == instruction) { 
	//	auto pos = { sh_pos.x_, sh_pos.y_-1 };
	//	if (is_ship_in_bounds(this->rect_, pos, sh_rect)) {
	//		this->ship_.move_ship(pos);
	//	}
	//} else if (instruction_t::DOWN == instruction) {
	//	auto pos = { sh_pos.x_, sh_pos.y_+1 };
	//	if (is_ship_in_bounds(this->rect_, pos, sh_rect)) {
	//		this->ship_.move_ship(pos);
	//	}
	//} else if (instruction_t::BACKWARD == instruction) {
	//	auto pos = { sh_pos.x_-1, sh_pos.y_ };
	//	if (is_ship_in_bounds(this->rect_, pos, sh_rect)) {
	//		this->ship_.move_ship(pos);
	//	}
	//} else if (instruction_t::FORWARD == instruction) {
	//	auto pos = { sh_pos.x_+1, sh_pos.y_ };
	//	if (is_ship_in_bounds(this->rect_, pos, sh_rect)) {
	//		this->ship_.move_ship(pos);
	//	}
	//}

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
static bool is_bullet_in_rect(const position_t& bullet_pos, 
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
			
			if (is_bullet_in_rect(bullet_it->position(),
						it->position(), it->rect())) {
				bullets_to_delete.push_back(bullet_it);
				it->hp_decrement();
				if (it->hp() <= 0) {
					enemies_to_delete.push_back(it);
				}
			}
		}
	}

	for (auto it : bullets_to_delete) {
		this->bullet_list_.erase(it);
	}
	for (auto it : enemies_to_delete) {
		this->enemy_list_.erase(it);
	}
}


