
#include "ship_t.hpp"


ship_t::ship_t(
		position_t pos,
		rect_t rect,
		int speed):
	position_(pos),
	rect_(rect),
	speed_(speed),
	hp_(1)
{

}
//-----------------------------------------------------------------------------//
ship_t::ship_t(
		position_t pos,
		rect_t rect,
		int speed,
		int hp):
	position_(pos),
	rect_(rect),
	speed_(speed),
	hp_(hp)
{

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
int
ship_t::hp() const {
	return this->hp_;
}
//-----------------------------------------------------------------------------//
void
ship_t::hp_decrement() {
	this->hp_--;
}
//-----------------------------------------------------------------------------//
void 
ship_t::position(const position_t& position) {
	this->position_ = position;
}
//-----------------------------------------------------------------------------//
void
ship_t::hp(int hp) {
	this->hp_ = hp;
}
//-----------------------------------------------------------------------------//
void
ship_t::tick() {
	this->position_.x_ += this->speed_;
}


