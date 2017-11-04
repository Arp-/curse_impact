#ifndef CURSE_IMPACT_RENDERER_CANVAS_T_HPP
#define CURSE_IMPACT_RENDERER_CANVAS_T_HPP

#include <vector>
#include <utility>
#include "polygon_t.hpp"

namespace renderer {

	template <typename flavour_T>
	class canvas_t final {

		public:

			using attribute_type = typename flavour_T::attribute_type;
			using value_type = std::pair<poligon_t, attribute_type>;
			using container_type = std::vector<value_type>;
			using const_iterator = typename container_type::const_iterator;
			using rect_t = poligon_t::rect_t;

		public: //-- public functions --//

			canvas_t() = default;
			~canvas_t() = default;

			void add(const poligon_t& poligon, const attribute_type& attribute) {
				this->container_.push_back(std::make_pair(poligon, attribute));
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

		private: //-- private variables --//


			container_type container_;


	};


} // namespace renderer



#endif // CURSE_IMPACT_RENDERER_CANVAS_T_HPP
