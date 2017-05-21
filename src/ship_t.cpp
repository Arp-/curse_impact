
#include "ship_t.hpp"

static int last_id_ = 0;
//-----------------------------------------------------------------------------//
ship_t::ship_t():
	position_(),
	rect_(),
	speed_(0),
	hp_(1),
	id_(last_id_++) 
{

}
//-----------------------------------------------------------------------------//
ship_t::ship_t(
		position_t pos,
		rect_t rect,
		int speed):
	position_(pos),
	rect_(rect),
	speed_(speed),
	hp_(1),
	id_(last_id_++)
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
	hp_(hp),
	id_(last_id_++)
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
int
ship_t::id() const {
	return this->id_;
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

