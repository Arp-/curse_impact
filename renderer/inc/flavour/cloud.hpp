
#ifndef CURSE_IMPACT_RENDERER_FLAVOUR_CLOUD_HPP
#define CURSE_IMPACT_RENDERER_FLAVOUR_CLOUD_HPP

#include <vector>
#include "canvas_t.hpp"

namespace renderer { namespace flavour {

	struct cloud {

		using canvas_type = renderer::canvas_t<cloud>;

		struct attribute_type {
		};

		std::vector<std::vector<char>> render(const canvas_type& str);

	};

}} // namespace flavour // namespace renderer


#endif // CURSE_IMPACT_RENDERER_FLAVOUR_CLOUD_HPP
