
#include "gamefield_t.hpp"

gamefield_t::gamefield_t(int width, int height):
	width_(width),
	height_(height),
	time_(0),
	ship_(*this) {

}
//-----------------------------------------------------------------------------//
gamefield_t::~gamefield_t() {

}
//-----------------------------------------------------------------------------//
