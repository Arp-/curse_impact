
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
ship_t::move(ship_event_t::direction dir, const rect_t& gamefield_rect ) {
	using direction = ship_event_t::direction;
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
		default:
			break;
	}
}
//-----------------------------------------------------------------------------//
void
ship_t::tick() {
	this->position_.x_ += this->speed_;
}
//-----------------------------------------------------------------------------//
position_t
ship_t::front_center() {
	return position_t {
		this->position_.x_ + this->rect_.width_,
		this->position_.y_ + (this->rect_.height_ >> 1)
	};
}
//-----------------------------------------------------------------------------//
std::ostream& operator<<(std::ostream& out, const position_t& pos) {
	out << "{ x: " << pos.x_ << ", y: " << pos.y_ << "}";
	return out;
}
//-----------------------------------------------------------------------------//
std::ostream& operator<<(std::ostream& out, const ship_t& ship) {
	out << "{\n";
	out << "\tid: " << ship.id_ << ",\n";
	out << "\tposition: " << ship.position_ << ",\n";
	out << "\tspeed: " << ship.speed_ << ",\n";
	out << "\thp: " << ship.hp_ << "\n";

	out << "}";
	return out;
}
