
#include "event_t.hpp"

appear_event_t::appear_event_t():
	id_(0),
	rect_id_(0),
	position_(0,0),
	hp_(0),
	speed_(0) {

}
//-----------------------------------------------------------------------------//
ship_event_t::ship_event_t():
	type_(ship_event_t::type::NOP),
	direction_(ship_event_t::direction::NOP) {

}
