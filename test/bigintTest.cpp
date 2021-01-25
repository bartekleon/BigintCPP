#include <gtest/gtest.h>

#include <climits>
#include <fstream>

#include "bigint.h"

using BigMath::Bigint;

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
  EXPECT_TRUE(Bigint(12) + Bigint(93) == Bigint(105));

  EXPECT_TRUE(Bigint(123) + 32 == Bigint(155));
  EXPECT_TRUE(Bigint(123) + -32 == Bigint(91));
  EXPECT_TRUE(Bigint(22) + -82 == Bigint(-60));
  EXPECT_TRUE(Bigint(-13) + -32 == Bigint(-45));
  EXPECT_TRUE(Bigint(-32) + 123 == Bigint(91));
  EXPECT_TRUE(Bigint(999999999) + Bigint(1) == Bigint(1000000000));
  EXPECT_TRUE(Bigint(999999999) + 1 == Bigint(1000000000));

  Bigint a(123);
  a += 30;
  a += 0;

  EXPECT_TRUE(a == Bigint(153));

  Bigint b(1);

  b += 999999999;

  EXPECT_TRUE(b == Bigint(1000000000));
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

  Bigint k = 3049023;
  k -= 34234;

  EXPECT_TRUE(k == Bigint(3014789));

  Bigint f = 3123;
  f -= 42309420844924;

  EXPECT_TRUE(f == Bigint(-42309420841801));

  Bigint g = 42309420844929;
  g -= 42309420844924;

  EXPECT_TRUE(g == Bigint(5));
}

TEST(MultiplicationTests, MultiplicationTests) {
  EXPECT_TRUE(Bigint(123) * Bigint(32) == Bigint(3936));
  EXPECT_TRUE(Bigint(123) * Bigint(-32) == Bigint(-3936));
  EXPECT_TRUE(Bigint(-22) * Bigint(82) == Bigint(-1804));
  EXPECT_TRUE(Bigint(-13) * Bigint(-32) == Bigint(416));

  EXPECT_TRUE(Bigint("342387239127313213") * Bigint("2312074892343") == Bigint("791624939044899694217816428059"));

  EXPECT_TRUE(Bigint(-13) * 0 == Bigint(0));
  EXPECT_TRUE(Bigint(-13) * Bigint(0) == Bigint(0));

  EXPECT_TRUE(Bigint(123) * 32 == Bigint(3936));
  EXPECT_TRUE(Bigint(123) * -32 == Bigint(-3936));
  EXPECT_TRUE(Bigint(-22) * 82 == Bigint(-1804));
  EXPECT_TRUE(Bigint(-13) * -32 == Bigint(416));

  Bigint a(123);
  a *= 13;

  EXPECT_TRUE(a == Bigint(1599));

  Bigint b("23498223123342898");
  b *= Bigint("92384723897592748924789234389");

  EXPECT_TRUE(b == Bigint("2170876855334063153876652533841878955740519322"));

  EXPECT_TRUE(Bigint(454513) * 54234575622444 == Bigint("24650319669883889772"));
  EXPECT_TRUE(Bigint(454513) * LLONG_MAX == Bigint("4192142494586974716366991"));
}

TEST(DivisionTests, DivisionTests) {
  EXPECT_TRUE((Bigint(8) / Bigint(2)) == Bigint(4));
  EXPECT_TRUE((Bigint(141) / Bigint(2)) == Bigint(70));
  EXPECT_TRUE((Bigint(1411) / Bigint(123)) == Bigint(11));
  EXPECT_TRUE((Bigint(-141) / Bigint(2)) == Bigint(-70));
  EXPECT_TRUE((Bigint(1411) / Bigint(-123)) == Bigint(-11));
  EXPECT_TRUE((Bigint(-141) / Bigint(-2)) == Bigint(70));
  EXPECT_TRUE((Bigint(141) / Bigint(1323)) == Bigint(0));
  EXPECT_TRUE((Bigint("23472390718913") / Bigint(13)) == Bigint("1805568516839"));
  EXPECT_TRUE(
    (Bigint("23472331074189168931468914618941490718913") / Bigint("942394729842789"))
    == Bigint("24907111989160678895558821"));

  Bigint a(4113);
  a /= Bigint(20);

  EXPECT_TRUE(a == 205);

  EXPECT_THROW(Bigint(123) / 0, std::invalid_argument);
}

TEST(AllocationTests, AllocationTests) {
  Bigint b;
  Bigint e;
  EXPECT_NO_THROW(Bigint a = 12345);
  EXPECT_NO_THROW(b = 159753);
  EXPECT_NO_THROW(Bigint c("12345"));
  EXPECT_NO_THROW(Bigint d = b);
  EXPECT_NO_THROW(e = "23498523524");
}

TEST(UnaryMinusTest, UnaryMinusTest) {
  EXPECT_TRUE(-Bigint(13) == Bigint(-13));
}

TEST(IsNegativeTests, IsNegativeTests) {
  EXPECT_FALSE(Bigint(13).is_negative());
  EXPECT_TRUE(Bigint(-13).is_negative());
}

TEST(PowerTests, PowerTests) {
  EXPECT_TRUE(Bigint(8).pow(0) == Bigint(1));
  EXPECT_TRUE(Bigint(-5).pow(0) == Bigint(1));
  EXPECT_TRUE(Bigint(21).pow(6) == Bigint(85766121));
  EXPECT_TRUE(Bigint(3).pow(7) == Bigint(2187));
  EXPECT_TRUE(Bigint(-2).pow(11) == Bigint(-2048));
  EXPECT_TRUE(Bigint(-2).pow(8) == Bigint(256));
}

TEST(Modulo, Modulo) {
  EXPECT_TRUE(Bigint("239847892391") % 13 == 3);
  EXPECT_TRUE(Bigint("-239847892391") % 13 == -3);
}

TEST(IsEven, IsEven) {
  EXPECT_TRUE(Bigint().is_even());
  EXPECT_TRUE(Bigint(132).is_even());
  EXPECT_FALSE(Bigint(-5).is_even());
  EXPECT_FALSE(Bigint("32923947912312189328682376811").is_even());
}

TEST(ABS, ABS) {
  EXPECT_TRUE(Bigint("-239847892391").abs() == Bigint("239847892391"));
  EXPECT_TRUE(Bigint("239847892391").abs() == Bigint("239847892391"));
}

TEST(Clone, Clone) {
  Bigint a(123);
  Bigint b = a.clone();

  EXPECT_TRUE(a == b);
  EXPECT_TRUE(&a != &b);
}

TEST(Streams, Streams) {
  std::string sLine = "2343239041089250841095749017409275918212719";

  Bigint a(sLine);

  EXPECT_TRUE(a.to_string() == sLine);
  EXPECT_TRUE(to_string(a) == sLine);
  EXPECT_TRUE(Bigint(-9).to_string() == "-9");
  EXPECT_TRUE(Bigint().to_string() == "0");

  Bigint b;
  std::istringstream stream(sLine);

  EXPECT_NO_THROW(stream >> b);

  EXPECT_TRUE(b == Bigint("2343239041089250841095749017409275918212719"));
}

TEST(Access, Access) {
  EXPECT_TRUE(Bigint("304839054389543804382543790782030318932382904234")[13] == 4);
  EXPECT_TRUE(Bigint("304839054389543804382543790782030318932382904234")[44] == 4);
  EXPECT_THROW(Bigint(1234)[4], std::out_of_range);

  EXPECT_TRUE(Bigint("304839054389543804382543790782030318932382904234")(13) == 1);
  EXPECT_TRUE(Bigint("304839054389543804382543790782030318932382904234")(44) == 8);
  EXPECT_THROW(Bigint(1234)(4), std::out_of_range);
}

TEST(Digits, Digits) {
  EXPECT_TRUE(Bigint().digits() == 1);
  EXPECT_TRUE(Bigint(0).digits() == 1);
  EXPECT_TRUE(Bigint(34331231).digits() == 8);
}

TEST(AddZeroes, AddZeroes) {
  EXPECT_TRUE(Bigint(34331231).add_zeroes(0) == Bigint(34331231));
  EXPECT_TRUE(Bigint(34331231).add_zeroes(5) == Bigint(3433123100000));
  EXPECT_TRUE(Bigint(34331231).add_zeroes(11) == Bigint("3433123100000000000"));
}

TEST(RemoveTrailing, RemoveTrailing) {
  EXPECT_TRUE(Bigint(34331231).remove_trailing(0) == Bigint(34331231));
  EXPECT_TRUE(Bigint(3433123100000).remove_trailing(5) == Bigint(34331231));
  EXPECT_TRUE(Bigint("3433123156364113314123").remove_trailing(11) == Bigint(34331231563));
}
