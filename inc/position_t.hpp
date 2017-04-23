

#ifndef CURSE_IMPACT_CHOORD_T_HPP
#define CURSE_IMPACT_CHOORD_T_HPP

struct position_t {
	position_t() = default;
	position_t(int x, int y): x_(x), y_(y) { }

	int x_;
	int y_;
};


#endif // CURSE_IMPACT_CHOORD_T_HPP
