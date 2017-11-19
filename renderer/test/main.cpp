
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "canvas_t.hpp"
#include <iomanip>
#include "coord_t.hpp"
#include "pixel_info.hpp"


struct test_flavour {

	using my_canvas_t = renderer::canvas_t<test_flavour>;

		struct attribute_type {
			int macska;
			int karacsony;
		};

		std::string render(const my_canvas_t& canvas) {
			return "karacsony";
		}

};

std::ostream& operator<<(std::ostream& out,
		const std::pair<renderer::coord_t, renderer::coord_t>& rect) {
	return out << "rect: " << rect.first << ", " << rect.second;
}


TEST_CASE ("macska") {
	REQUIRE(1 == 1);
}

TEST_CASE ("test poligon contining_rect") {

	renderer::canvas_t<test_flavour> canvas;
	renderer::poligon_t p1 {{1,2}, {1,1}, {0,1}};
	renderer::poligon_t p2 {{1,2}, {1,1}, {0,2}};
	renderer::poligon_t p3 {{1,2}, {9,1}, {0,1}};
	renderer::poligon_t p4 {};
	canvas.add(p1, { 1, 2 });
	canvas.add(p2, { 1, 3 });
	canvas.add(p3, { 1, 3 });
	canvas.add(p4, { 1, 3 });

	auto pair = canvas.containing_rect();
	std::cout << "canvas: " << *pair << std::endl;
	std::cout << "p1: " << *p1.containing_rect() << std::endl;
	std::cout << "p2: " << *p2.containing_rect() << std::endl;
	std::cout << "p3: " << *p3.containing_rect() << std::endl;
	std::cout << "p4: " << *p4.containing_rect() << std::endl;
}

TEST_CASE ("test info") {

	renderer::pixel_info inf { 0.0f, renderer::pixel_info::side::UP, 0 };
	std::cout << sizeof(inf) << std::endl;


}


