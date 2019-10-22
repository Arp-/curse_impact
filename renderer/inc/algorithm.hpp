#ifndef CURSE_IMPACT_RENDERER_ALGORITHM_HPP
#define CURSE_IMPACT_RENDERER_ALGORITHM_HPP

#include "polygon_t.hpp"
#include "coord_t.hpp"

namespace renderer {

	using vector_t = std::array<coord_t, 2>;

	// Also returns true if point is on the polygon
	bool point_inclusion(const polygon_t& poly, const coord_t& point);
	util::optional<coord_t> crossection(const vector_t& A, const vector_t& B);
	int slope(const vector_t& vec);
	coord_t normalize(const vector_t& vec);
	bool is_parallel(const vector_t& A, const vector_t& B);
	// Can only determine the rotation if [a,b] vector crosses o.y line
	bool is_clockwise(const coord_t& o, const coord_t& a, const coord_t& b);

}



#endif // CURSE_IMPACT_RENDERER_ALGORITHM_HPP
