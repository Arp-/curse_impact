
#include "flavour/cloud.hpp"

namespace rf = renderer::flavour;

using texture_t = std::vector<std::vector<char>>;

static void init_texture(texture_t& texture, renderer::coord_t max) {
	for (int x = 0; x < max.x; x++) {
		texture.push_back({});
		for (int y = 0; y < max.y; y++) {
			texture[x].push_back(' ');
		}
	}
}

texture_t
rf::cloud::render(const rf::cloud::canvas_type& canvas) {
	texture_t texture;
	const auto& containing_rect = canvas.containing_rect();
	if (!containing_rect) {
		return {{}};
	}
	init_texture(texture, containing_rect->second);

	return texture;
}


