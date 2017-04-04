
#ifndef CURSE_IMPACT_IMAGE_T_HPP
#define CURSE_IMPACT_IMAGE_T_HPP

#include <iomanip>

template <int dim_x_V, int dim_y_V>
class image_t {

	public:

		image_t(const std::string& img);

		~image_t() = default;;

	private: 

		std::array<std::array<char, dim_x_V>, dim_y_V> buffer_;

};

#endif // CURSE_IMPACT_IMAGE_T_HPP
