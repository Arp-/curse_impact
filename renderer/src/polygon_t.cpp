#include "polygon_t.hpp"


renderer::util::optional<renderer::polygon_t::rect_t>
renderer::polygon_t::containing_rect() const {
	auto it = this->cbegin();
	if (it == this->cend()) {
		return {};
	}
	coord_t min = *it;
	coord_t max = *it;
	++it;
	for (;it != this->cend(); ++it) {
		if (it->x < min.x) {
			min.x = it->x;
		} 
		if (it->x > max.x) {
			max.x = it->x;
		}

		if (it->y < min.y) {
			min.y = it->y;
		}
		if (it->y > max.y) {
			max.y = it->y;
		}
	}

	return { rect_t{ min, max }};
}
//-----------------------------------------------------------------------------//
renderer::coord_t renderer::polygon_t::operator[](size_t index) const {
	return this->polygon_base_t::operator[](index % this->size());
}
