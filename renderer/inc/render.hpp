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

	template<typename flavour_T>
	typename impl::flavour_traits<flavour_T>::return_type
	render(const canvas_t<flavour_T>& canvas) {
		flavour_T flavour;
		return flavour.render(canvas);
	}


} // namespace renderer


#endif // CURSE_IMPACT_RENDERER_RENDER_HPP
