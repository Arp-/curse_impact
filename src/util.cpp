#include <util.hpp>


util::scoped_initializer::scoped_initializer(
		std::function<void(void)>&& constructor, 
			std::function<void(void)>&& destructor): destructor_(destructor) {

	constructor();
}
//-----------------------------------------------------------------------------//
util::scoped_initializer::~scoped_initializer() {
	this->destructor_();
}
