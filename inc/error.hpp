

#ifndef CURSE_IMPACT_ERROR_HPP
#define CURSE_IMPACT_ERROR_HPP

#include <stdexcept>

struct invalid_format_error : public std::runtime_error {

	invalid_format_error(const std::string& what):
		std::runtime_error("The input specified was of an invalid format: (" + what + ")") {}

};


#endif // CURSE_IMPACT_ERROR_HPP
