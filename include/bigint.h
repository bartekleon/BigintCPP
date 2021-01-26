#pragma once

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
  bool positive{ true };

public:
  // Constructors
  Bigint() = default;
  ~Bigint() = default;
  Bigint(const Bigint &bigint) = default;
  Bigint(Bigint &&bigint) = default;

  Bigint(int64_t value);
  Bigint(std::string_view string);

  // Adding
  Bigint operator+(const Bigint &right) const;
  Bigint &operator+=(const Bigint &right);
  Bigint operator+(int64_t value) const;
  Bigint &operator+=(int64_t value);

  // Subtraction
  Bigint operator-(const Bigint &right) const;
  Bigint &operator-=(const Bigint &right);
  Bigint operator-(int64_t value) const;
  Bigint &operator-=(int64_t value);

  // Unary minus
  Bigint operator-() noexcept;

  // Multiplication
  Bigint operator*(const Bigint &right) const;
  Bigint &operator*=(const Bigint &right);
  Bigint operator*(int64_t value) const;
  Bigint &operator*=(int64_t value);

  // Division
  Bigint operator/(const Bigint &right) const;
  Bigint &operator/=(const Bigint &right);

  // Modulo
  [[nodiscard]] int64_t operator%(const int64_t &value) const;

  // Comparison
  [[nodiscard]] bool operator<(const Bigint &other) const noexcept;
  [[nodiscard]] bool operator>(const Bigint &other) const noexcept;
  [[nodiscard]] bool operator<=(const Bigint &other) const noexcept;
  [[nodiscard]] bool operator>=(const Bigint &other) const noexcept;
  [[nodiscard]] bool operator==(const Bigint &other) const noexcept;
  [[nodiscard]] bool operator!=(const Bigint &other) const noexcept;

  // Allocation
  Bigint &operator=(const std::string &str) noexcept;
  Bigint &operator=(const Bigint &other) = default;
  Bigint &operator=(Bigint &&other) = default;

  // Access
  [[nodiscard]] int32_t operator[](const int32_t &which) const;
  [[nodiscard]] int32_t operator()(const int32_t &which) const;

  // Input & Output
  friend std::istream &operator>>(std::istream &stream, Bigint &bigint);
  friend std::ostream &operator<<(std::ostream &stream, const Bigint &bigint);

  // Helpers
  void clear() noexcept;
  Bigint &abs() noexcept;
  [[nodiscard]] std::string to_string() const;
  [[nodiscard]] int32_t digits() const noexcept;
  [[nodiscard]] bool is_even() const noexcept;
  [[nodiscard]] bool is_negative() const noexcept;
  [[nodiscard]] bool is_zero() const noexcept;
  [[nodiscard]] Bigint clone() const noexcept;

  Bigint &add_zeroes(uint64_t amount);
  Bigint &remove_trailing(uint64_t amount);

  // Power
  Bigint &pow(uint32_t power);

  [[nodiscard]] int8_t compare(const Bigint &right) const noexcept;// 0 a == b, -1 a < b, 1 a > b
  [[nodiscard]] int8_t compare(const Bigint &right, bool absolute) const noexcept;// 0 a == b, -1 a < b, 1 a > b
private:
  [[nodiscard]] Bigint &absolute_addition(const Bigint &right);
  [[nodiscard]] Bigint &absolute_subtraction(const Bigint &right);
  [[nodiscard]] Bigint pow(uint32_t power, std::map<uint32_t, Bigint> *lookup);
  static void get_fragment(const Bigint &bigint, Bigint &result, int32_t which);
};

std::string to_string(const Bigint &bigint);

}// namespace BigMath
