#ifndef CURSE_IMPACT_RENDERER_SCALE_HPP
#define CURSE_IMPACT_RENDERER_SCALE_HPP

#include "coord_t.hpp"
#include "util.hpp"
#include "polygon_t.hpp"
#include <cmath>
#include <iostream>


namespace renderer {

	using rect_t = polygon_t::rect_t;

	struct scale_operation_t {
		coord_t base_offset;
		coord_t end_offset;
		double scale_x;
		double scale_y;
	};

	inline std::ostream& operator<<(std::ostream& out, const scale_operation_t& op) {
		out << "scale_op: {\n";
		out << "  base_offset: " << op.base_offset << "\n";
		out << "  end_offset: " << op.end_offset << "\n";
		out << "  scale_x: " << op.scale_x << "\n";
		out << "  scale_y: " << op.scale_y << "\n";
		out << "};";

		return out;
	}

	inline bool is_negative(const renderer::coord_t& ch) {
		return ch.x < 0 || ch.y < 0;
	}

	// TODO put t his crap into a cpp file and make this lib into a standrad one
	// not a header only

	inline scale_operation_t scale(const rect_t& base, const rect_t& end) {
		coord_t base_vec = { 
			base.second.x - base.first.x,
			base.second.y - base.first.y
		};
		
		coord_t end_vec = {
			end.second.x - end.first.x,
			end.second.y - end.first.y
		};

		return scale_operation_t {
			base.first,
			end.first,
			static_cast<double>(base_vec.x) / end_vec.x,
			static_cast<double>(base_vec.y) / end_vec.y
		};
	}

	inline coord_t scale(const coord_t& ch, const scale_operation_t& op) {
		return coord_t {
			static_cast<int>((ch.x - op.base_offset.x) / op.scale_x + op.end_offset.x),
			static_cast<int>((ch.y - op.base_offset.y) / op.scale_y + op.end_offset.y),
		};
	}

} // namespace renderer


#endif // CURSE_IMPACT_RENDERER_SCALE_HPP
