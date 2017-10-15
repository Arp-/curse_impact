
#include "display/border_drawer_t.hpp"

namespace d = display;



d::border_drawer_t::builder_t&
d::border_drawer_t::builder_t::window(WINDOW* win) {
	this->win_ = win;
	return *this;
}
//-----------------------------------------------------------------------------//
d::border_drawer_t::builder_t&
d::border_drawer_t::builder_t::position(const position_t& position) {
	this->pos_ = position;
	return *this;
}
//-----------------------------------------------------------------------------//
d::border_drawer_t::builder_t&
d::border_drawer_t::builder_t::rect(const rect_t& rect) {
	this->rect_ = rect;
	return *this;
}
//-----------------------------------------------------------------------------//
d::border_drawer_t::builder_t&
d::border_drawer_t::builder_t::style(char vertical, char horizontal, char corner) {
	this->style_ = { vertical, horizontal, corner };
	return *this;
}
//-----------------------------------------------------------------------------//
util::naive_optional<d::border_drawer_t>
d::border_drawer_t::builder_t::build() {
	if (this->win_ == nullptr) {
		return {};
	}
	if (this->rect_.width_ < 3 || this->rect_.height_ < 3) {
		return {};
	}

	return border_drawer_t { this->win_, this->pos_, this->rect_, this->style_ };
}
//-----------------------------------------------------------------------------//
d::border_drawer_t::border_drawer_t(WINDOW* win,
		position_t pos,
		rect_t rect,
		style_t style): 
	window_(win),
	position_(pos),
	rect_(rect),
	style_(style) 
{

}
//-----------------------------------------------------------------------------//
d::border_drawer_t::builder_t
d::border_drawer_t::builder() {
	return border_drawer_t::builder_t { };
}
//-----------------------------------------------------------------------------//
void
d::border_drawer_t::draw() {
	auto end_x = this->position_.x_ + this->rect_.width_-1;
	auto end_y = this->position_.y_ + this->rect_.height_-1;
	for (int x = this->position_.x_+1; x < end_x; x++) {
		mvwaddch(this->window_, this->position_.y_, x, this->style_.vertical_);
	}
	for (int x = this->position_.x_+1; x < end_x; x++) {
		mvwaddch(this->window_, end_y, x, this->style_.vertical_);
	}
	//for (int y = this->position_.y_+1; y < end_y; y++) {
	//	mvwaddch(this->window_, y, end_x, this->style_.horizontal_);
	//}
	//for (int y = this->position_.y_+1; y < end_y; y++) {
	//	mvwaddch(this->window_, y, this->position_.x_, this->style_.horizontal_);
	//}
}

