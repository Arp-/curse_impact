#include "display/healthbar_t.hpp"

namespace d = display;

d::healthbar_t::healthbar_t(WINDOW* win):
	healthbar_t(win, { 0, 0 }) {

}
//-----------------------------------------------------------------------------//
d::healthbar_t::healthbar_t(WINDOW* win, const position_t& pos):
	win_(win),
	position_(pos) {

		this->init_texture();

}
//-----------------------------------------------------------------------------//
void
d::healthbar_t::update(int hp) {
	if (this->cached_hp_ == hp) { return; }
	this->cached_hp_ = hp;

	if (hp > 3) { 
		throw std::runtime_error { 
			std::string { "healthpoints are greater than that is displayable " } +
				"max: 3, hp: " + std::to_string(hp)
		};
	}
	auto& data_row = this->texture_[1];
	data_row = "// HP: ";
	int i = 0;
	for (i = 0; i < hp; i++) {
		data_row += "X";
	}
	for (; i < 3; i++) {
		data_row += " ";
	}
	data_row += " //";
}
//-----------------------------------------------------------------------------//
void
d::healthbar_t::position(const position_t& position) {
	this->position_ = position;
}
//-----------------------------------------------------------------------------//
const position_t& 
d::healthbar_t::position() const {
	return this->position_;
}
//-----------------------------------------------------------------------------//
void
d::healthbar_t::init_texture() {
	this->texture_.emplace_back("//---------//------------------------------------");
	this->texture_.emplace_back("// HP: XXX //");
	this->texture_.emplace_back("//---------//------------------------------------");
}
//-----------------------------------------------------------------------------//
void
d::healthbar_t::display() {
	int row_offset = 0;
	for (const auto& row :  this->texture_) {
		wmove(this->win_, this->position_.y_ + row_offset, this->position_.x_);
		wprintw(this->win_, row.c_str());
		row_offset++;
	}
}

