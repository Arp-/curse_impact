
#ifndef CURSE_IMPACT_RENDERER_PIXEL_INFO_HPP
#define CURSE_IMPACT_RENDERER_PIXEL_INFO_HPP

namespace renderer {

	struct pixel_info {
		enum class axis {
			X_EQ_0,       // -
			Y_EQ_0,       // |
			X_EQ_MINUS_X, // backslash
			X_EQ_PLUS_X,  // /
		};

		enum class side {
			LEFT,
			RIGHT,
		};

		axis axis;
		side side;
		float percentage; // should be between 1.0 and 0.0 inclusive
	};


} // namespace renderer


#endif // CURSE_IMPACT_RENDERER_PIXEL_INFO_HPP
