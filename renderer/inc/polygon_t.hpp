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

			// NOTE coordinates from  this point onwards will be all relative to origo
			inline util::optional<rect_t> containing_rect() const {
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


	};



} // namespace renderer


#endif // CURSE_IMPACT_RENDERER_POLIGON_T_HPP
