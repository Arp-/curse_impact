#ifndef CURSE_IMPACT_RENDERER_RENDER_HPP
#define CURSE_IMPACT_RENDERER_RENDER_HPP

namespace renderer {

	namespace impl {
		template <typename flavour_T>
		struct flavour_traits {
			using canvas_type = canvas_t<flavour_T>;
			using return_type = 
				decltype(std::declval<flavour_T>().render(std::declval<canvas_type>()));
		};

		template <typename T>
		struct is_flavour {
			// Lookup how to verify if class has member function again
			// well actually i should order the tamplate metaprogramming the complete
			// guide book because it looks interresting
		};
	}

	struct scale_t {
		float x;
		float y;
	};


	inline coord_t
	canvas_coord_to_scaled_coord(coord_t ch, scale_t scale) {
		return { ch.x * scale.x, ch.y * scale.y };
	}

	inline coord_t
	scaled_coord_to_canvas_coord(coord_t ch, scale_t scale) {
		return { ch.x / scale.x, ch.y / scale.y };
	}

	template <typename flavour_T>
	scale_t
	calc_scale(const canvas_t<flavour_T>& canvas, const coord_t& size) {

		const auto& canvas_rect = canvas.containing_rect();
		if (!canvas_rect) {
			throw std::runtime_error { "Couldn't determine canvas size!" };
		}
		return { size.x / canvas_rect->x, size.y / canvas_rect->y };
	}


	template<typename flavour_T>
	typename impl::flavour_traits<flavour_T>::return_type
	render(const canvas_t<flavour_T>& canvas, const coord_t& size) {
		flavour_T flavour;

		scale_t scale = calc_scale(canvas, size);
		for (int x = 0; x < size.x; ++x) {
			for (int y = 0; y < size.y; ++y) {
				calc_coord_containment({ 0, 0 }, scale_x, scale_y, canvas);

				pixel_info info { 
					flavour.accumulate(flavour.render(const pixel_info& info));
			}
		}
		return flavour;
	}


} // namespace renderer


#endif // CURSE_IMPACT_RENDERER_RENDER_HPP
