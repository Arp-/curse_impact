#include "algorithm.hpp"
#include <cassert>

// returns 1 if the number is negative, otherwise 0
#define NEG(X) (assert(sizeof(X) == sizeof(unsigned)), (X) & ~(-1u >> 1) && 1)
static bool dbg = true;
#define DBG(...) if (dbg) { printf(__VA_ARGS__); }

static int is_left(
		const renderer::coord_t& o, const renderer::coord_t& a, const renderer::coord_t& b) {

	return (b.x-a.x) * (o.y-a.y) - (o.x-a.x) * (b.y-a.y);
}
//-----------------------------------------------------------------------------//
static bool is_between(int num, int begin, int end) {
	if (begin > end) {
		std::swap(begin, end);
	}
	DBG("begin %d, num: %d, end: %d\n", begin, num, end);
	return begin <= num && num <= end;
}
//-----------------------------------------------------------------------------//
static bool is_on_line(
		const renderer::coord_t& o, const renderer::coord_t& a, const renderer::coord_t& b) {
	if ((b.x - a.x) == 0) {
		DBG("is horizontal\n");
		return is_between(o.y, b.y, a.y) && o.x == a.x;
	}
	printf("is_between(%d, %d, %d): %d\n", o.x,a.x,b.x, is_between(o.x,a.x,b.x));
	printf("is_between(%d, %d, %d): %d\n", o.y,a.y,b.y, is_between(o.y,a.y,b.y));
	printf("oy-ay = %d\n", o.y-a.y);
	printf("other: %lf\n", (double)(b.y-a.y)/(b.x-a.x) * (o.x-a.x));
	return is_between(o.x, a.x, b.x)
		&& is_between(o.y, a.y, b.y)
		&& ( (o.y-a.y) * (b.x-a.x) == (b.y-a.y) * (o.x-a.x) );
}
//-----------------------------------------------------------------------------//
bool renderer::point_inclusion(const polygon_t& poly, const coord_t& point) {

	// get a ray from the point
	//int horizontal_ray = point.y;

	int wn = 0;
	DBG("point: (%d,%d)\n", point.x, point.y);
	for (unsigned i = 0; i < poly.size(); ++i) {
		DBG("poly[%d]: (%d, %d)\n", i, poly[i].x, poly[i].y);
		DBG("poly[%d]: (%d, %d)\n", i+1, poly[i+1].x, poly[i+1].y);
		if (is_on_line(point, poly[i], poly[i+1])) {
			DBG("was on_line\n");
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

