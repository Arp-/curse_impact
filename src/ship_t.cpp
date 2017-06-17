
#include "ship_t.hpp"

//-----------------------------------------------------------------------------//
ship_t::ship_t():
	position_(),
	rect_(),
	speed_(0),
	hp_(1),
	id_(0)
{

}
//-----------------------------------------------------------------------------//
ship_t::ship_t(
		position_t pos,
		rect_t rect,
		int speed,
		int hp,
		int id
		):
	position_(pos),
	rect_(rect),
	speed_(speed),
	hp_(hp),
	id_(id)
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
ship_t::move(event_t::direction dir, const rect_t& gamefield_rect ) {
	using direction = event_t::direction;
	switch (dir) {
		case direction::UP:
			if (this->position_.y_ > 0) {
				--(this->position_.y_);
			}
			break;
		case direction::DOWN:
			if (this->position_.y_ < (gamefield_rect.height_-1)) {
				++(this->position_.y_);
			}
			break;
	}
}
//-----------------------------------------------------------------------------//
void
ship_t::tick() {
	this->position_.x_ += this->speed_;
}


