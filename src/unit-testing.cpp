#include <gtest/gtest.h>

#include "calc_controller.cc"
#include "calc_model.cc"
#include "credit_calc.cc"

s21::calc_controller controller;

TEST(Calculator, SumMinus1) {
  std::string exp = "-1+2+3-4+(-5)+6+7-8-9";
  long double answer = controller.calculate(exp);

  EXPECT_EQ(answer, -9);
}

TEST(Calculator, SumMinus2) {
  std::string exp = "9-(-0.30928)*3-4";
  long double answer = controller.calculate(exp);

  EXPECT_DOUBLE_EQ(answer, 5.92784);
}

TEST(Calculator, SumFunc) {
  std::string exp = "(132+sin(asin(sqrt(ln(log(228.11)))))-4*5^6*(123))";
  long double answer = controller.calculate(exp);

  EXPECT_DOUBLE_EQ(answer, -7687367.0737845786);
}

TEST(Calculator, MultByBracket) {
  std::string exp =
      "2*cos(1) + 3*sin(2) - 4*tan(1) + 2*acos(0.32) - 3*asin(0.2) + 4*atan(2) "
      "- "
      "4*ln(10) + 7*log(99) / 4*x";
  controller.set_x(10);
  long double answer = controller.calculate(exp);

  EXPECT_DOUBLE_EQ(answer, 29.606796314880558);
}

TEST(Calculator, Long) {
  std::string exp =
      "15/(7-(1+1))*3-(2+(1+1))*15/(7-(200+1))*3-(2+(1+1))*(15/"
      "(7-(1+1))*3-(2+(1+1))+15/(7-(1+1))*3-(2+(1+1)))";
  long double answer = controller.calculate(exp);

  EXPECT_DOUBLE_EQ(answer, -30.072164948453608);
}

TEST(Calculator, Functions) {
  std::string exp =
      "sin(1)+cos(1)+asin(1)+acos(1)+atan(1)+tan(1)+sqrt(16)+ln(10)+log(10)";
  long double answer = controller.calculate(exp);

  EXPECT_DOUBLE_EQ(answer, 12.597960598517329);
}

TEST(Calculator, Power) {
  std::string exp = "2^2^3 * 4";
  long double answer = controller.calculate(exp);

  EXPECT_EQ(answer, 1024);
}

TEST(Calculator, Mod) {
  std::string exp = "((2 * 5437 mod 521 * 224 mod (3 + 2)) * 2000 / 500) mod 3";
  long double answer = controller.calculate(exp);

  EXPECT_EQ(answer, 1);
}

TEST(Calculator, X) {
  std::string exp = "2*x*(2*x+1) + 4";
  controller.set_x(3.15);
  long double answer = controller.calculate(exp);

  EXPECT_DOUBLE_EQ(answer, 49.989999999999995);
}

TEST(Calculator, InvalidTest1) {
  std::string exp = "sin(1) * cos)(";
  EXPECT_ANY_THROW(controller.calculate(exp));

  exp = "__trash/";
  EXPECT_ANY_THROW(controller.calculate(exp));

  exp = "2--2**12*//4mod*31";
  EXPECT_ANY_THROW(controller.calculate(exp));

  exp = "2-2)";
  EXPECT_ANY_THROW(controller.calculate(exp));

  exp = "2...5132 - 1.24.12";
  EXPECT_ANY_THROW(controller.calculate(exp));

  exp = "";
  EXPECT_ANY_THROW(controller.calculate(exp));
}

TEST(Calculator, InvalidTest2) {
  std::string exp = "siren";
  EXPECT_ANY_THROW(controller.calculate(exp));

  exp = "thanos";
  EXPECT_ANY_THROW(controller.calculate(exp));

  exp = "call me";
  EXPECT_ANY_THROW(controller.calculate(exp));

  exp = "lol";
  EXPECT_ANY_THROW(controller.calculate(exp));

  exp = "apple";
  EXPECT_ANY_THROW(controller.calculate(exp));

  exp = "my man";
  EXPECT_ANY_THROW(controller.calculate(exp));

  exp = "xor is ^";
  EXPECT_ANY_THROW(controller.calculate(exp));
}

TEST(Calculator, InvalidTest3) {
  std::string exp = "2*^3";
  EXPECT_ANY_THROW(controller.calculate(exp));

  exp = "33^mod^33";
  EXPECT_ANY_THROW(controller.calculate(exp));

  exp = "11^/22/^11";
  EXPECT_ANY_THROW(controller.calculate(exp));

  exp = "sqrt(15)253";
  EXPECT_ANY_THROW(controller.calculate(exp));

  exp = "sqrt()";
  EXPECT_ANY_THROW(controller.calculate(exp));
}

TEST(Credit, Annuity) {
  double sum = 1500000;
  int term = 33;
  double percent = 22.3;
  int type = 1;
  controller.set_credit_calc_data(sum, percent, term, type);

  EXPECT_FLOAT_EQ(controller.get_credit_payment()[0], 61215.94);
  EXPECT_FLOAT_EQ(controller.get_credit_overpayment()[term - 1], 520126.02);
  EXPECT_FLOAT_EQ(controller.get_credit_out_sum()[term - 1], 2020126.02);
}

TEST(Credit, Differ) {
  double sum = 1000000;
  int term = 21;
  double percent = 9.4;
  int type = 2;
  controller.set_credit_calc_data(sum, percent, term, type);

  EXPECT_FLOAT_EQ(controller.get_credit_payment()[0], 55452.38);
  EXPECT_FLOAT_EQ(controller.get_credit_payment()[term - 1], 47992.06);
  EXPECT_FLOAT_EQ(controller.get_credit_overpayment()[term - 1], 86166.67);
  EXPECT_FLOAT_EQ(controller.get_credit_out_sum()[term - 1], 1086166.67);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
