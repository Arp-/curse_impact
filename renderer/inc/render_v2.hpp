#ifndef CURSE_IMPACT_RENDERER_RENDER_V2_HPP
#define CURSE_IMPACT_RENDERER_RENDER_V2_HPP

namespace renderer {

	template <typename canvas_T, typename flavour_T>
	void render(const canvas_T& canvas, const flavour_T& flavour) {

		canvas scaled_canvas = canvas.scale_into({{0, 0}, {30, 30}});
	}


} // namespace renderer


#endif // CURSE_IMPACT_RENDERER_RENDER_V2_HPP
