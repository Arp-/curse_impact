
#ifndef CURSE_IMPACT_RECT_T_HPP
#define CURSE_IMPACT_RECT_T_HPP

struct rect_t {

	rect_t() = default;
	constexpr rect_t(int w, int h): width_(w), height_(h) { }

	int width_;
	int height_;
};


#endif // CURSE_IMPACT_RECT_T_HPP
