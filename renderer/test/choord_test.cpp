
#include <catch.hpp>
#include "scale.hpp"
#include "coord_t.hpp"

SCENARIO("choordinates can be compared to each other") {

	GIVEN("two choordinates") {
		WHEN("one of the numbers of the first coordinate is smaller") {
			renderer::coord_t c { 1, 2};
			renderer::coord_t d { 1, 3};

			THEN("the the two coordinates are different") {
				REQUIRE(c != d);
			}
		}
	}
}

