#ifndef CURSE_IMPACT_RENDERER_POLIGON_T_HPP
#define CURSE_IMPACT_RENDERER_POLIGON_T_HPP

#include "coord_t.hpp"
#include "util.hpp"
#include <vector>
#include <stdexcept>

namespace renderer {

	using polygon_base_t = std::vector<coord_t>;

	// TODO add builder to make sure there are at least 3 points
	class polygon_t : private polygon_base_t {

		public: 
			using rect_t = std::pair<coord_t, coord_t>;
			using polygon_base_t::vector;
			using polygon_base_t::begin;
			using polygon_base_t::end;
			using polygon_base_t::cbegin;
			using polygon_base_t::cend;
			using polygon_base_t::empty;
			using polygon_base_t::push_back;
			using polygon_base_t::size;

			// NOTE coordinates from  this point onwards will be all relative to origo
			util::optional<rect_t> containing_rect() const;
			coord_t operator[](size_t index) const;

	};

	inline std::ostream& operator<<(
			std::ostream& out, const polygon_t::rect_t& rect) {
		out << "rect: {\n";
		out << "  { " << rect.first << " },\n";
		out << "  { " << rect.second << " },\n";
		out << "};";
		return out;
	}

	inline std::ostream& operator<<(std::ostream& out, const polygon_t& poly) {
		out << "polygon: {\n";
		for (const auto& coord : poly) {
			out << "  " << coord << "\n";
		}
		out << "};";
		return out;
	}




} // namespace renderer


#endif // CURSE_IMPACT_RENDERER_POLIGON_T_HPP
