#include <catch.hpp>
#include "scale.hpp"
#include "util.hpp"

TEST_CASE("gcd") {
	int a = 32;
	int b = 76;
	REQUIRE(4 == renderer::util::binary_gcd(76,32));
	REQUIRE(5 == renderer::util::binary_gcd(10,5));
	REQUIRE(5 == renderer::util::binary_gcd(5,10));
	REQUIRE(1 == renderer::util::binary_gcd(10,1));
	REQUIRE(50 == renderer::util::binary_gcd(50,250));
	REQUIRE(4 == renderer::util::binary_gcd(8,4));

}
