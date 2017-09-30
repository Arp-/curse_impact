#include <unique.hpp>

class bullet_t;

template <>
unsigned unique<bullet_t>::counter_ = 0;
