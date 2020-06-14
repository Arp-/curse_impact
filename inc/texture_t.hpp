

#ifndef CURSE_IMPACT_TEXTURE_T_HPP
#define CURSE_IMPACT_TEXTURE_T_HPP

#include <iomanip>
#include <vector>
#include "rect_t.hpp"


// generally a texture shuld be an X*Y character long matrix
class texture_t {

	public:

		using matrix_row_t = std::vector<char>;
		using matrix_t = std::vector<matrix_row_t>;
	
	public: // public functions --//

		texture_t();

		texture_t(const texture_t& texture) = default;

		texture_t(const matrix_t& mtx);

		~texture_t() = default;

		const matrix_t& matrix() const;

		const matrix_row_t& operator[](size_t) const;


	public: //-- public static functions --//

		static texture_t read_from_file(const std::string& filepath);


	private: //-- private members --//

		matrix_t matrix_;

}; 

#endif // CURSE_IMPACT_TEXTURE_T_HPP
