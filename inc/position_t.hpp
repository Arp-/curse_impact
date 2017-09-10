

#ifndef CURSE_IMPACT_CHOORD_T_HPP
#define CURSE_IMPACT_CHOORD_T_HPP

struct position_t {

	constexpr position_t():x_(0), y_(0) { };
	constexpr position_t(int x, int y): x_(x), y_(y) { }

	int x_;
	int y_;
};


#endif // CURSE_IMPACT_CHOORD_T_HPP
