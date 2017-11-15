#ifndef CURSE_IMPACT_RENDERER_CHOORD_T_HPP
#define CURSE_IMPACT_RENDERER_CHOORD_T_HPP

#include <climits>
#include <iomanip>
#include "util.hpp"

namespace renderer {

	struct coord_t : 
			public util::equality_compareable<coord_t> {

		constexpr coord_t(int x, int y): x(x), y(y) { }
		int x;
		int y;

		inline bool operator!=(const coord_t& ch) {
			return this->x != ch.x || this->y != ch.y;
		}

	};

	inline std::ostream& operator<<(std::ostream& out, const coord_t& t) {
		return out << "{" << t.x << ", " << t.y << "}";
	}


} // namespace renderer


#endif // CURSE_IMPACT_RENDERER_CHOORD_T_HPP