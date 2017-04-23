
#include "ship_t.hpp"


ship_t::ship_t(
		position_t pos,
		rect_t rect,
		int speed):
	position_(pos),
	rect_(rect),
	speed_(speed) {

}
//-----------------------------------------------------------------------------//
const rect_t&
ship_t::rect() const {
	return this->rect_;
}
//-----------------------------------------------------------------------------//
const position_t&
ship_t::position() const {
	return this->position_;
}
//-----------------------------------------------------------------------------//
int
ship_t::speed() const {
	return this->speed_;
}
//-----------------------------------------------------------------------------//
void 
ship_t::position(const position_t& position) {
	this->position_ = position;
}
//-----------------------------------------------------------------------------//

