#ifndef CURSE_IMPACT_RENDERER_RENDER_HPP
#define CURSE_IMPACT_RENDERER_RENDER_HPP

namespace renderer {

	using info_mtx_t = std::vector<std::vector<renderer::pixel_info>>;

	namespace impl {
		template <typename flavour_T>
		struct flavour_traits {
			using canvas_type = canvas_t<flavour_T>;
			using return_type = 
				decltype(std::declval<flavour_T>().render(std::declval<canvas_type>()));
		};
//-----------------------------------------------------------------------------//
		template <typename T>
		struct is_flavour {
			// Lookup how to verify if class has member function again
			// well actually i should order the tamplate metaprogramming the complete
			// guide book because it looks interresting
		};
//-----------------------------------------------------------------------------//
		inline init_info_mtx(info_mtx_t& mtx, const coord_t& size) {
			for (int i = 0; i < size.x; i++) {
				mtx.emplace_back();
				for (int j = 0; j < size.y; j++) {
					mtx[i].emplace_back();
				}
			}
		}
	}
//-----------------------------------------------------------------------------//
	struct scale_t {
		float x;
		float y;
	};
//-----------------------------------------------------------------------------//
	inline coord_t
	to_scaled_coord(coord_t ch, scale_t scale) { return { ch.x * scale.x, ch.y * scale.y }; }
//-----------------------------------------------------------------------------//
	inline coord_t
	to_canvas_coord(coord_t ch, scale_t scale) {
		return { ch.x / scale.x, ch.y / scale.y };
	}
//-----------------------------------------------------------------------------//
	template <typename flavour_T>
	scale_t
	calc_scale(const coord_t& canvas_rect, const coord_t& size) {
		if (!canvas_rect) {
			throw std::runtime_error { "Couldn't determine canvas size!" };
		}
		return { size.x / canvas_rect->x, size.y / canvas_rect->y };
	}
//-----------------------------------------------------------------------------//
static calc_spanning_number(const poligon_t& poligon, coord_t refpoint, scale_t poligon_scale) {
	
}
//-----------------------------------------------------------------------------//
	template <typename flavour_T>
	info_mtx_t
	calc_polygon_info(const poligon_t& polygon, const coord_t& canvas_rect, const coord_t& size) {
		info_mtx_t mtx;
		init_info_mtx(mtx);
		scale_t scale = calc_scale(canvas_rect, size);
		// foreach point inside the rectangle
		//  calculate the circumvent number of the polygon
		for (unsigned x = 0; x < size.x; x++) {
			for (unsigned y = 0; y < size.y; y++) {
				calc_spanning_number(poligon, x, y);
			}
		}

		return mtx;
	}
//-----------------------------------------------------------------------------//
	template <typename flavour_T>
	std::vector<info_mtx_t>
	calc_info_mtx(const canvas_t<flavour_T>& canvas, const coord_t& size) {

		std::vector<info_mtx_t> info_mtx_vec;
		const auto& canvas_size = canvas.size();
		for (const auto& poligon : canvas) {
			info_mtx_vec.emplace_back(calc_poligon_info(poligon, canvas_size, size));
		}
		return info_mtx_vec;
	}
//-----------------------------------------------------------------------------//
	template<typename flavour_T>
	typename impl::flavour_traits<flavour_T>::return_type
	render(const canvas_t<flavour_T>& canvas, const coord_t& size) {
		flavour_T flavour;
		auto info_mtx_vec = calc_info_mtx(canvas, size);

		return flavour.render(info_mtx_vec);
	}
//-----------------------------------------------------------------------------//
} // namespace renderer


#endif // CURSE_IMPACT_RENDERER_RENDER_HPP
