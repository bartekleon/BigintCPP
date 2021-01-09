#pragma once

#ifndef BIGINT_H_
#define BIGINT_H_

#include <algorithm>
#include <array>
#include <cmath>
#include <exception>
#include <map>
#include <sstream>
#include <string>
#include <string_view>

#include "smallvector.h"

namespace BigMath {

class Bigint {
private:
  SmallVector<int32_t> number;// max size of vector uint32_t
  mutable bool positive = true;

public:
  // Constructors
  Bigint() = default;
  ~Bigint() = default;
  Bigint(const Bigint &bigint) = default;
  Bigint(Bigint &&bigint) = default;

  Bigint(int64_t value);
  Bigint(std::string_view string);

  // Adding
  Bigint operator+(Bigint const &right) const;
  Bigint &operator+=(Bigint const &right);
  Bigint operator+(int64_t const &value) const;
  Bigint &operator+=(int64_t value);

  // Subtraction
  Bigint operator-(Bigint const &right) const;
  Bigint &operator-=(Bigint const &right);
  Bigint operator-(int64_t const &value) const;
  Bigint &operator-=(int64_t value);

  // Unary minus
  Bigint operator-();

  // Multiplication
  Bigint operator*(Bigint const &right) const;
  Bigint &operator*=(Bigint const &right);
  Bigint operator*(int64_t value) const;
  Bigint &operator*=(int64_t const &value);

  // Division
  Bigint operator/(Bigint right);
  Bigint &operator/=(Bigint const &right);

  // Modulo
  [[nodiscard]] int64_t operator%(int64_t const &value) const;

  // Comparison
  [[nodiscard]] bool operator<(const Bigint &other) const;
  [[nodiscard]] bool operator>(const Bigint &other) const;
  [[nodiscard]] bool operator<=(const Bigint &other) const;
  [[nodiscard]] bool operator>=(const Bigint &other) const;
  [[nodiscard]] bool operator==(const Bigint &other) const;
  [[nodiscard]] bool operator!=(const Bigint &other) const;

  // Allocation
  Bigint &operator=(const std::string &str);
  Bigint &operator=(const Bigint &other) = default;
  Bigint &operator=(Bigint &&other) = default;

  // Access
  [[nodiscard]] int32_t operator[](int32_t const &which) const;

  // Input & Output
  friend std::istream &operator>>(std::istream &stream, Bigint &bigint);
  friend std::ostream &operator<<(std::ostream &stream, Bigint const &bigint);

  // Helpers
  void clear();
  Bigint &abs();
  [[nodiscard]] std::string to_string() const;
  [[nodiscard]] int32_t digits() const;
  [[nodiscard]] bool is_even() const;
  [[nodiscard]] bool is_negative() const;
  Bigint clone();
  constexpr void flip_sign() const;

  Bigint &add_zeroes(uint32_t amount);

  // Power
  Bigint &pow(uint32_t const &power);

private:
  [[nodiscard]] constexpr static int32_t segment_length(int32_t segment);
  [[nodiscard]] Bigint pow(uint32_t const &power, std::map<int32_t, Bigint> *lookup);
  [[nodiscard]] int8_t compare(Bigint const &right) const;// 0 a == b, -1 a < b, 1 a > b
  [[nodiscard]] static Bigint get_fragment(const Bigint &bigint, int32_t which);
};

std::string to_string(Bigint const &bigint);

}// namespace BigMath

#endif /* BIGINT_H_ */
