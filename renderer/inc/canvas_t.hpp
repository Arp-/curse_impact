#ifndef CURSE_IMPACT_RENDERER_CANVAS_T_HPP
#define CURSE_IMPACT_RENDERER_CANVAS_T_HPP

#include <vector>
#include <utility>
#include <iostream>
#include "polygon_t.hpp"
#include "scale.hpp"

namespace renderer {

	template <typename flavour_T>
	class canvas_t {

		public:

			using attribute_type = typename flavour_T::attribute_type;
			using value_type = std::pair<polygon_t, attribute_type>;
			using container_type = std::vector<value_type>;
			using const_iterator = typename container_type::const_iterator;
			using rect_t = polygon_t::rect_t;

		public: //-- public functions --//

			canvas_t() = default;
			~canvas_t() = default;

			void add(const polygon_t& polygon, const attribute_type& attribute) {
				this->container_.push_back(std::make_pair(polygon, attribute));
			}

			const_iterator begin() const {
				return this->container_.cbegin();
			}

			const_iterator end() const {
				return this->container_.cend();
			}

			util::optional<rect_t> containing_rect() const {
				auto it = this->container_.begin();
				for (;it != this->container_.end() && !it->first.containing_rect(); ++it);
				if (it == this->container_.end()) {
					return {};
				}
				const auto& cont_rect = *(it->first.containing_rect());
				coord_t min = cont_rect.first;
				coord_t max = cont_rect.second;
				++it;
				for (;it != this->container_.end(); ++it) {
					const auto& cont_rect = it->first.containing_rect();
					if (cont_rect) {
						if (cont_rect->first.x < min.x) {
							min.x = cont_rect->first.x;
						} 
						if (cont_rect->first.y < min.y) {
							min.y = cont_rect->first.y;
						}
						if (cont_rect->second.x > max.x) {
							max.x = cont_rect->second.x;
						}
						if (cont_rect->second.y > max.y) {
							max.y = cont_rect->second.y;
						}
					}
				}
				return { rect_t { min, max }};
			}

			canvas_t scale_into(rect_t rect) {
				canvas_t canvas = {};
				const auto& containing_rect = this->containing_rect();
				if (!containing_rect) {
					throw std::runtime_error("Could not determine containing rect for canvas!");
				}
				scale_operation_t scale_op = scale(*containing_rect, rect);
				auto& container = canvas.container_;
				for (const auto& poly_attr_pair : this->container_) {
					const auto& polygon = poly_attr_pair.first;
					typename std::decay<decltype(polygon)>::type scaled_polygon;
					for (const auto& coord : polygon) {
						scaled_polygon.push_back(scale(coord, scale_op));
					}
					canvas.add(scaled_polygon, poly_attr_pair.second);
				}

				return canvas;
			}

		private: //-- private variables --//


			container_type container_;


	};


} // namespace renderer



#endif // CURSE_IMPACT_RENDERER_CANVAS_T_HPP
