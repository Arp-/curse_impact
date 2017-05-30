

#define MAX_CHAR_TO_IGNORE 999

#include <fstream>
#include <stdexcept>
#include "texture_t.hpp"


texture_t::texture_t(): matrix_() { 
}
//-----------------------------------------------------------------------------//
texture_t
texture_t::read_from_file(const std::string& filename) {
	texture_t texture;
	std::ifstream in_file(filename);

	int i = 0;
	while (in_file.good()) {
		texture.matrix_.emplace_back();
		auto& row = texture.matrix_[i++];
		char c;
		while((c = in_file.get()) != '\n' && in_file.good()) {
			row.push_back(c);
		}
	}

	return texture;
}
//-----------------------------------------------------------------------------//
const texture_t::matrix_t&
texture_t::matrix() const {
	return this->matrix_;
}
//-----------------------------------------------------------------------------//
const texture_t::matrix_row_t&
texture_t::operator[](size_t i) const {
	return this->matrix_[i];
}
