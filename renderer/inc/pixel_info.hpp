
#ifndef CURSE_IMPACT_RENDERER_PIXEL_INFO_HPP
#define CURSE_IMPACT_RENDERER_PIXEL_INFO_HPP

namespace renderer {

	struct pixel_info {
		enum class side {
			FULL = 0,
			UP,
			UP_LEFT,
			UP_RIGHT,
			RIGHT,
			LEFT,
			DOWN,
			DOWN_LEFT,
			DOWN_RIGHT,
		};

		float containment;
		side side;
		unsigned depth;
	};


} // namespace renderer


#endif // CURSE_IMPACT_RENDERER_PIXEL_INFO_HPP
