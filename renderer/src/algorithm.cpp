#include "algorithm.hpp"
#include <cassert>

// returns 1 if the number is negative, otherwise 0
#define NEG(X) (assert(sizeof(X) == sizeof(unsigned)), (X) & ~(-1u >> 1) && 1)

static int is_left(
		const renderer::coord_t& o, const renderer::coord_t& a, const renderer::coord_t& b) {

	return (b.x-a.x) * (o.y-a.y) - (o.x-a.x) * (b.y-a.y);
}
//-----------------------------------------------------------------------------//
static bool is_between(int num, int begin, int end) {
	if (begin > end) {
		std::swap(begin, end);
	}
	return begin <= num && num <= end;
}
//-----------------------------------------------------------------------------//
static bool is_on_line(
		const renderer::coord_t& o, const renderer::coord_t& a, const renderer::coord_t& b) {
	if ((b.x - a.x) == 0) {
		return is_between(o.y, b.y, a.y) && o.x == (b.x - a.x);
	}
	return is_between(o.x, a.x, b.x) && o.y - a.y == (b.y - a.y)/(b.x - a.x) * o.x;
}
//-----------------------------------------------------------------------------//
bool renderer::point_inclusion(const polygon_t& poly, const coord_t& point) {

	// get a ray from the point
	//int horizontal_ray = point.y;

	int wn = 0;
	for (unsigned i = 0; i < poly.size(); ++i) {
		if (is_on_line(point, poly[i], poly[i+1])) {
			return true;
		}
	}
	for (unsigned i = 0; i < poly.size(); ++i) {
		if (poly[i].y <= point.y) {
			if (poly[i+1].y > point.y && is_left(point, poly[i], poly[i+1]) > 0) {
				wn++;
			}
		} else {
			if (poly[i+1].y <= point.y && is_left(point, poly[i], poly[i+1]) < 0) {
				wn--;
			}
		}
	}

	return wn != 0;
}
//-----------------------------------------------------------------------------//
int renderer::slope(const vector_t& vec) {
	coord_t normalized = normalize(vec);
	return normalized.y / normalized.x;
}
//-----------------------------------------------------------------------------//
renderer::coord_t
renderer::normalize(const vector_t& vec) {
	coord_t normalized = {vec[1].x - vec[0].x, vec[1].y - vec[0].y};

	return normalized;
}
//-----------------------------------------------------------------------------//
bool
renderer::is_parallel(const vector_t& A, const vector_t& B) {
	//coord_t norm_A = normalize(A);
	return true;
}
//-----------------------------------------------------------------------------//
renderer::util::optional<renderer::coord_t>
renderer::crossection(const vector_t& vec_A, const vector_t& vec_B) {

	return {};
}

