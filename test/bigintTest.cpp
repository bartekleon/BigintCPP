#include <gtest/gtest.h>

#include "bigint.h"

TEST(ComparisonTests, ComparisonTests) {
	ASSERT_TRUE(Bigint(123) == Bigint(123));
	ASSERT_TRUE(Bigint(0) == 0);
	ASSERT_TRUE(Bigint(0) == Bigint("0"));
	ASSERT_TRUE(Bigint(123) != Bigint(-123));
	ASSERT_TRUE(Bigint(123) < Bigint(124));
	ASSERT_TRUE(Bigint(123) > Bigint(122));
	ASSERT_TRUE(Bigint(123) >= Bigint(123));
	ASSERT_TRUE(Bigint(124) >= Bigint(123));
	ASSERT_TRUE(Bigint(123) <= Bigint(123));
	ASSERT_TRUE(Bigint(122) <= Bigint(123));
	ASSERT_TRUE(Bigint(-125) <= Bigint(123));
	ASSERT_TRUE(Bigint(125) >= Bigint(-128));
	ASSERT_TRUE(Bigint(-6) < Bigint(-5));
	ASSERT_TRUE(Bigint(-6) > Bigint(-7));
	ASSERT_TRUE(Bigint(-6) >= Bigint(-6));
	ASSERT_TRUE(Bigint(-9) <= Bigint(-6));
}

TEST(SumationTests, SumationTests) {
	EXPECT_TRUE(Bigint(123) + Bigint(32) == Bigint(155));
	EXPECT_TRUE(Bigint(123) + Bigint(-32) == Bigint(91));
	EXPECT_TRUE(Bigint(22) + Bigint(-82) == Bigint(-60));
	EXPECT_TRUE(Bigint(-13) + Bigint(-32) == Bigint(-45));
	EXPECT_TRUE(Bigint(-32) + Bigint(123) == Bigint(91));

	EXPECT_TRUE(Bigint(123) + 32 == Bigint(155));
	EXPECT_TRUE(Bigint(123) + -32 == Bigint(91));
	EXPECT_TRUE(Bigint(22) + -82 == Bigint(-60));
	EXPECT_TRUE(Bigint(-13) + -32 == Bigint(-45));
	EXPECT_TRUE(Bigint(-32) + 123 == Bigint(91));
}

TEST(SubtractionTests, SubtractionTests) {
	EXPECT_TRUE(Bigint(123) - Bigint(32) == Bigint(91));
	EXPECT_TRUE(Bigint(123) - Bigint(-32) == Bigint(155));
	EXPECT_TRUE(Bigint(22) - Bigint(-82) == Bigint(104));
	EXPECT_TRUE(Bigint(-13) - Bigint(-32) == Bigint(19));
	EXPECT_TRUE(Bigint(-32) - Bigint(123) == Bigint(-155));
	EXPECT_TRUE(Bigint(32) - Bigint(123) == Bigint(-91));
	EXPECT_TRUE(Bigint(0) - Bigint(123) == Bigint(-123));
	EXPECT_TRUE(Bigint(123) - Bigint() == Bigint(123));
	EXPECT_TRUE(Bigint(123) - Bigint(0) == Bigint(123));

	EXPECT_TRUE(Bigint(123) - 32 == Bigint(91));
	EXPECT_TRUE(Bigint(123) - -32 == Bigint(155));
	EXPECT_TRUE(Bigint(22) - -82 == Bigint(104));
	EXPECT_TRUE(Bigint(-13) - -32 == Bigint(19));
	EXPECT_TRUE(Bigint(-32) - 123 == Bigint(-155));
	EXPECT_TRUE(Bigint(32) - 123 == Bigint(-91));
	EXPECT_TRUE(Bigint(0) - 123 == Bigint(-123));
	EXPECT_TRUE(Bigint(123) - 0 == Bigint(123));
}

TEST(MultiplicationTests, MultiplicationTests) {
	EXPECT_TRUE(Bigint(123) * Bigint(32) == Bigint(3936));
	EXPECT_TRUE(Bigint(123) * Bigint(-32) == Bigint(-3936));
	EXPECT_TRUE(Bigint(-22) * Bigint(82) == Bigint(-1804));
	EXPECT_TRUE(Bigint(-13) * Bigint(-32) == Bigint(416));

	EXPECT_TRUE(Bigint(-13) * 0 == Bigint(0));
	EXPECT_TRUE(Bigint(-13) * Bigint(0) == Bigint(0));


	EXPECT_TRUE(Bigint(123) * 32 == Bigint(3936));
	EXPECT_TRUE(Bigint(123) * -32 == Bigint(-3936));
	EXPECT_TRUE(Bigint(-22) * 82 == Bigint(-1804));
	EXPECT_TRUE(Bigint(-13) * -32 == Bigint(416));
}

TEST(DivisionTests, DivisionTests) {
	EXPECT_TRUE((Bigint(8) / Bigint(2)) == Bigint(4));
	EXPECT_TRUE((Bigint(141) / Bigint(2)) == Bigint(70));
	EXPECT_TRUE((Bigint(1411) / Bigint(123)) == Bigint(11));
	EXPECT_TRUE((Bigint(-141) / Bigint(2)) == Bigint(-70));
	EXPECT_TRUE((Bigint(1411) / Bigint(-123)) == Bigint(-11));
	EXPECT_TRUE((Bigint(-141) / Bigint(-2)) == Bigint(70));
}

TEST(PowerTests, PowerTests) {
	EXPECT_TRUE(Bigint(8).pow(0) == Bigint(1));
	EXPECT_TRUE(Bigint(-5).pow(0) == Bigint(1));
	EXPECT_TRUE(Bigint(21).pow(6) == Bigint(85766121));
	EXPECT_TRUE(Bigint(3).pow(7) == Bigint(2187));
	EXPECT_TRUE(Bigint(-2).pow(11) == Bigint(-2048));
	EXPECT_TRUE(Bigint(-2).pow(8) == Bigint(256));
}
