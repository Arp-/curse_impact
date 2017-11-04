#ifndef CURSE_IMPACT_RENDERER_POLIGON_T_HPP
#define CURSE_IMPACT_RENDERER_POLIGON_T_HPP

#include <vector>
#include "coord_t.hpp"
#include <stdexcept>

namespace renderer {

	using poligon_base_t = std::vector<coord_t>;

	class poligon_t : private poligon_base_t {

		public: 
			using rect_t = std::pair<coord_t, coord_t>;
			using poligon_base_t::vector;
			using poligon_base_t::operator[];
			using poligon_base_t::begin;
			using poligon_base_t::end;
			using poligon_base_t::cbegin;
			using poligon_base_t::cend;
			using poligon_base_t::empty;

			inline util::optional<rect_t> containing_rect() const {
				auto it = this->cbegin();
				if (it == this->cend()) {
					return {};
				}
				coord_t min = *it;
				coord_t max = *it;
				++it;
				for (;it != this->cend(); ++it) {
					if (it->x < 0) {
						min.x += it->x;
					} else {
						max.x += it->x;
					}
					if (it->y < 0) {
						min.y += it->y;
					} else {
						max.y += it->y;
					}
				}

				return { rect_t{ min, max }};
			}


	};



} // namespace renderer


#endif // CURSE_IMPACT_RENDERER_POLIGON_T_HPP
