#define BOOST_TEST_MODULE bigintTests
#include <boost/test/included/unit_test.hpp>

#include "bigint.h"

BOOST_AUTO_TEST_CASE(ComparisonTests) {
	BOOST_REQUIRE(Bigint(123) == Bigint(123));
	BOOST_REQUIRE(Bigint(0) == 0);
	BOOST_REQUIRE(Bigint(0) == Bigint("0"));
	BOOST_REQUIRE(Bigint(123) != Bigint(-123));
	BOOST_REQUIRE(Bigint(123) < Bigint(124));
	BOOST_REQUIRE(Bigint(123) > Bigint(122));
	BOOST_REQUIRE(Bigint(123) >= Bigint(123));
	BOOST_REQUIRE(Bigint(124) >= Bigint(123));
	BOOST_REQUIRE(Bigint(123) <= Bigint(123));
	BOOST_REQUIRE(Bigint(122) <= Bigint(123));
	BOOST_REQUIRE(Bigint(-125) <= Bigint(123));
	BOOST_REQUIRE(Bigint(125) >= Bigint(-128));
	BOOST_REQUIRE(Bigint(-6) < Bigint(-5));
	BOOST_REQUIRE(Bigint(-6) > Bigint(-7));
	BOOST_REQUIRE(Bigint(-6) >= Bigint(-6));
	BOOST_REQUIRE(Bigint(-9) <= Bigint(-6));
}

BOOST_AUTO_TEST_CASE(SumationTests) {
	BOOST_TEST(Bigint(123) + Bigint(32) == Bigint(155));
	BOOST_TEST(Bigint(123) + Bigint(-32) == Bigint(91));
	BOOST_TEST(Bigint(22) + Bigint(-82) == Bigint(-60));
	BOOST_TEST(Bigint(-13) + Bigint(-32) == Bigint(-45));
	BOOST_TEST(Bigint(-32) + Bigint(123) == Bigint(91));

	BOOST_TEST(Bigint(123) + 32 == Bigint(155));
	BOOST_TEST(Bigint(123) + -32 == Bigint(91));
	BOOST_TEST(Bigint(22) + -82 == Bigint(-60));
	BOOST_TEST(Bigint(-13) + -32 == Bigint(-45));
	BOOST_TEST(Bigint(-32) + 123 == Bigint(91));
}

BOOST_AUTO_TEST_CASE(SubtractionTests) {
	BOOST_TEST(Bigint(123) - Bigint(32) == Bigint(91));
	BOOST_TEST(Bigint(123) - Bigint(-32) == Bigint(155));
	BOOST_TEST(Bigint(22) - Bigint(-82) == Bigint(104));
	BOOST_TEST(Bigint(-13) - Bigint(-32) == Bigint(19));
	BOOST_TEST(Bigint(-32) - Bigint(123) == Bigint(-155));
	BOOST_TEST(Bigint(32) - Bigint(123) == Bigint(-91));
	BOOST_TEST(Bigint(0) - Bigint(123) == Bigint(-123));
	BOOST_TEST(Bigint(123) - Bigint() == Bigint(123));
	BOOST_TEST(Bigint(123) - Bigint(0) == Bigint(123));

	BOOST_TEST(Bigint(123) - 32 == Bigint(91));
	BOOST_TEST(Bigint(123) - -32 == Bigint(155));
	BOOST_TEST(Bigint(22) - -82 == Bigint(104));
	BOOST_TEST(Bigint(-13) - -32 == Bigint(19));
	BOOST_TEST(Bigint(-32) - 123 == Bigint(-155));
	BOOST_TEST(Bigint(32) - 123 == Bigint(-91));
	BOOST_TEST(Bigint(0) - 123 == Bigint(-123));
	BOOST_TEST(Bigint(123) - 0 == Bigint(123));
}

BOOST_AUTO_TEST_CASE(MultiplicationTests) {
	BOOST_TEST(Bigint(123) * Bigint(32) == Bigint(3936));
	BOOST_TEST(Bigint(123) * Bigint(-32) == Bigint(-3936));
	BOOST_TEST(Bigint(-22) * Bigint(82) == Bigint(-1804));
	BOOST_TEST(Bigint(-13) * Bigint(-32) == Bigint(416));

	BOOST_TEST(Bigint(-13) * 0 == Bigint(0));
	BOOST_TEST(Bigint(-13) * Bigint(0) == Bigint(0));


	BOOST_TEST(Bigint(123) * 32 == Bigint(3936));
	BOOST_TEST(Bigint(123) * -32 == Bigint(-3936));
	BOOST_TEST(Bigint(-22) * 82 == Bigint(-1804));
	BOOST_TEST(Bigint(-13) * -32 == Bigint(416));
}

BOOST_AUTO_TEST_CASE(DivisionTests) {
	BOOST_TEST((Bigint(8) / Bigint(2)) == Bigint(4));
	BOOST_TEST((Bigint(141) / Bigint(2)) == Bigint(70));
	BOOST_TEST((Bigint(1411) / Bigint(123)) == Bigint(11));
	BOOST_TEST((Bigint(-141) / Bigint(2)) == Bigint(-70));
	BOOST_TEST((Bigint(1411) / Bigint(-123)) == Bigint(-11));
	BOOST_TEST((Bigint(-141) / Bigint(-2)) == Bigint(70));
}

BOOST_AUTO_TEST_CASE(PowerTests) {
	BOOST_TEST(Bigint(8).pow(0) == Bigint(1));
	BOOST_TEST(Bigint(-5).pow(0) == Bigint(1));
	BOOST_TEST(Bigint(21).pow(6) == Bigint(85766121));
	BOOST_TEST(Bigint(3).pow(7) == Bigint(2187));
	BOOST_TEST(Bigint(-2).pow(11) == Bigint(-2048));
	BOOST_TEST(Bigint(-2).pow(8) == Bigint(256));
}
