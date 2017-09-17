
#include "bullet_t.hpp"


bullet_t::bullet_t(int x, int y, int speed):
	position_(x, y),
	speed_(speed) {
	
}
//-----------------------------------------------------------------------------//
void
bullet_t::tick() {
	this->position_.x_ += this->speed_;
}
//-----------------------------------------------------------------------------//
const position_t&
bullet_t::position() const {
	return this->position_;
}
//-----------------------------------------------------------------------------//
int
bullet_t::speed() const {
	return this->speed_;
}
//-----------------------------------------------------------------------------//

