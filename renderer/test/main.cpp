
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "canvas_t.hpp"
#include <iomanip>
#include "coord_t.hpp"
#include "pixel_info.hpp"
#include "polygon_t.hpp"
#include "algorithm.hpp"


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


renderer::canvas_t<test_flavour> make_canvas() {
	renderer::canvas_t<test_flavour> canvas;
	renderer::polygon_t p1 {{1,2}, {1,1}, {0,1}};
	renderer::polygon_t p2 {{1,0}, {1,1}, {1,2}};
	renderer::polygon_t p3 {{1,2}, {9,1}, {0,1}};
	canvas.add(p1, { 1, 2 });
	canvas.add(p2, { 1, 3 });
	canvas.add(p3, { 1, 3 });
	return canvas;
}


TEST_CASE ("test equality") {
	REQUIRE(1 == 1);
}

TEST_CASE ("test polygon contining_rect") {
	puts(__PRETTY_FUNCTION__);
	renderer::canvas_t<test_flavour> canvas;
	renderer::polygon_t p1 {{1,2}, {1,1}, {0,1}};
	renderer::polygon_t p2 {{1,2}, {1,1}, {1,2}};
	renderer::polygon_t p3 {{1,2}, {9,1}, {0,1}};
	renderer::polygon_t p4 {};
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

TEST_CASE ("canvas scale into") {
	puts(__PRETTY_FUNCTION__);
	renderer::canvas_t<test_flavour> canvas = make_canvas();

	std::cout << "canvas: " << *canvas.containing_rect() << std::endl;
	for (const auto& poly : canvas) {
		std::cout << "old_poly: " << poly.first << std::endl;
	}
	auto new_canvas = canvas.scale_into({{0,0},{100, 100}});
	std::cout << "new_canvas: " << *new_canvas.containing_rect() << std::endl;
	for (const auto& poly : new_canvas) {
		std::cout << "new_poly: " << poly.first << std::endl;
	}

//	auto new2_canvas = canvas.scale_into({{30,30},{40, 40}});
//	std::cout << "new2_canvas: " << *new2_canvas.containing_rect() << std::endl;
//	for (const auto& poly : new2_canvas) {
//		std::cout << "poly: " << *poly.first.containing_rect() << std::endl;
//	}

}

TEST_CASE ("test info") {

	renderer::pixel_info inf { renderer::pixel_info::axis::X_EQ_0, renderer::pixel_info::side::LEFT, 0.0f };
	std::cout << "info_size: " << sizeof(inf) << std::endl;
}

TEST_CASE ("winding number") {
	using renderer::polygon_t;
	using renderer::coord_t;
	polygon_t poly { {0, 0}, {0, 2}, {2, 0} };
	coord_t point_inside = {1, 1};
	coord_t point_outside_right = {2, 2};
	coord_t point_outside_left = {-1, 0};
	coord_t point_outside_up = {0, 3};
	coord_t point_outside_down = {1, -1};
	REQUIRE(true == point_inclusion(poly, point_inside));
	REQUIRE(false == point_inclusion(poly, point_outside_right));
	REQUIRE(false == point_inclusion(poly, point_outside_left));
	REQUIRE(false == point_inclusion(poly, point_outside_up));
	REQUIRE(false == point_inclusion(poly, point_outside_down));
	REQUIRE(false == point_inclusion(poly, coord_t{ 0, 3}));
	std::vector<coord_t> points_on = { {0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {2,0} };
	for (const auto& p : points_on) {
		REQUIRE(true == point_inclusion(poly, p));
	}
}

TEST_CASE ("winding number 2") {
	printf("windiwng number2\n");
	using renderer::polygon_t;
	using renderer::coord_t;
	polygon_t poly { {0, 10}, {10, 5}, {10, 0} };
	std::vector<coord_t> points_on = { {0,0}, {0,1}, {0,2}, {0,3}, {0,4}, {0,5} };
	for (const auto& p : points_on) {
		REQUIRE(false == point_inclusion(poly, p));
	}
	std::vector<coord_t> points_on2 = { {5,10}, {6,10}, {7,10} };
	for (const auto& p : points_on2) {
		REQUIRE(false == point_inclusion(poly, p));
	}

}


