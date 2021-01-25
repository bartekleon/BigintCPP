#include "bigint.h"

namespace BigMath {

namespace Details {
  constexpr int32_t segment_length(int32_t segment) noexcept {
    if (segment == 0) {
      return 1;
    }

    return static_cast<int32_t>(std::log10(segment)) + 1;
  }
}// namespace Details

// represents maximum length of single element of Bigint arraym which is "9" repeated BLOCK_SIZE times
constexpr static const int32_t BLOCK_SIZE{ 9 };
// represents maximum + 1 value of single element of Bigint array - must be equal 10^BLOCK_SIZE
constexpr static const int32_t BLOCK{ 1000000000 };
constexpr static const int64_t BLOCK_SQ{ static_cast<int64_t>(BLOCK) * static_cast<int64_t>(BLOCK) };
constexpr static std::array<int32_t, 10> POW10{ 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };

Bigint::Bigint(int64_t value) : number{ 8 } {
  if (value < 0) {
    positive = false;
    value = -value;
  } else {
    positive = true;
  }

  while (value != 0) {
    number.push_back(value % BLOCK);
    value /= BLOCK;
  }
}

Bigint::Bigint(std::string_view string) : number{ string.length() / BLOCK_SIZE } {
  size_t size{ string.length() };

  positive = string[0] != '-';

  while (true) {
    if (size == 0) {
      break;
    }
    if (!positive && size == 1) {
      break;
    }

    uint32_t length{ 0 };
    int32_t num{ 0 };
    const size_t v{ size > BLOCK_SIZE ? size - BLOCK_SIZE : 0 };

    for (size_t idx{ size - 1 }; idx >= v; --idx) {
      if (string[idx] >= '0' && string[idx] <= '9') {
        num += (string[idx] - '0') * POW10.at(length);
      }
      ++length;
      if (idx == 0) {
        break;
      }
    }
    number.push_back(num);
    size -= length;
  }
}

Bigint &Bigint::absolute_addition(const Bigint &right) {
  number.resize(std::max(number.size(), right.number.size()), 0);

  int32_t *ptr_left{ number.begin() };
  int32_t *ptr_right{ right.number.begin() };
  int32_t sum{ 0 };

  while (ptr_left != number.end()) {
    sum += *ptr_left;

    if (ptr_right != right.number.end()) {
      sum += *ptr_right;
      ++ptr_right;
    }
    *ptr_left = sum % BLOCK;
    ptr_left++;
    sum /= BLOCK;
  }
  if (sum != 0) {
    number.push_back(1);
  }

  return *this;
}

Bigint &Bigint::absolute_subtraction(const Bigint &right) {
  int32_t *ptr_right{ right.number.begin() };
  int32_t *ptr_right_end{ right.number.end() };

  if(compare(right, true) == -1) {
    const Bigint new_right = *this;
    ptr_right = new_right.number.begin();
    ptr_right_end = new_right.number.end();
    number = right.number;
    positive = !positive;
  }

  int32_t *ptr_left{ number.begin() };

  int32_t diff{ 0 };

  while (ptr_left != number.end() || ptr_right != ptr_right_end) {
    if (ptr_left != number.end()) {
      diff += *ptr_left;
      ++ptr_left;
    } else {
      number.push_back(0);
      ptr_left = number.end();
    }
    if (ptr_right != ptr_right_end) {
      diff -= *ptr_right;
      ++ptr_right;
    }
    if (diff < 0) {
      *(ptr_left - 1) = (diff + BLOCK) % BLOCK;
      diff = -1;
    } else {
      *(ptr_left - 1) = diff % BLOCK;
      diff /= BLOCK;
    }
  }
  while (!number.empty() && number.back() == 0) {
    number.pop_back();
  }

  return *this;
}

Bigint Bigint::operator+(const Bigint &right) const {
  Bigint c{ *this };
  c += right;

  return c;
}

Bigint &Bigint::operator+=(const Bigint &right) {
  if (positive && !right.positive) {
    *this = absolute_subtraction(right);

    return *this;
  }
  if (!positive && right.positive) {
    positive = !positive;

    *this = absolute_subtraction(right);

    positive = !positive;
    return *this;
  }

  return absolute_addition(right);
}

Bigint Bigint::operator+(int64_t value) const {
  Bigint c{ *this };
  c += value;

  return c;
}

Bigint &Bigint::operator+=(int64_t value) {
  if (value == 0) {
    return *this;
  }
  if (positive && value < 0) {
    return *this -= -value;
  }
  if (!positive && value > 0) {
    positive = !positive;

    *this -= value;

    positive = !positive;
    return *this;
  }
  if (!positive && value < 0) {
    value = -value;
  }

  int32_t *ptr{ number.begin() };

  while (value != 0) {
    if (ptr != number.end()) {
      *ptr += static_cast<int32_t>(value % BLOCK);
      value /= BLOCK;
      value += *ptr / BLOCK;
      *ptr %= BLOCK;
      ++ptr;
    } else {
      number.push_back(0);
      ptr = number.end() - 1;
    }
  }

  return *this;
}

Bigint Bigint::operator-(const Bigint &right) const {
  Bigint c{ *this };
  c -= right;

  return c;
}

Bigint &Bigint::operator-=(const Bigint &right) {
  if (right.is_zero()) {
    return *this;
  }
  if(positive != right.positive) {
    return absolute_addition(right);
  }

  return absolute_subtraction(right);
}

Bigint Bigint::operator-(int64_t value) const {
  Bigint c{ *this };
  c -= value;

  return c;
}

Bigint &Bigint::operator-=(int64_t value) {
  if (value == 0) {
    return *this;
  }
  if (!positive || value < 0) {
    return *this += -value;
  }

  int32_t *ptr{ number.begin() };
  int64_t diff{ 0 };

  while (ptr != number.end() || value != 0) {
    if (ptr != number.end()) {
      diff += *ptr;
      ++ptr;
    } else {
      number.push_back(0);
      ptr = number.end();
    }

    diff -= value % BLOCK;
    value /= BLOCK;

    if (diff < 0) {
      *(ptr - 1) = (diff + BLOCK) % BLOCK;
      diff = -1;
    } else {
      *(ptr - 1) = diff % BLOCK;
      diff /= BLOCK;
    }
  }
  if (diff < 0) {
    *this = Bigint(1).add_zeroes(number.size() * BLOCK_SIZE) - *this;
    positive = !positive;
  }
  while (!number.empty() && number.back() == 0) {
    number.pop_back();
  }

  return *this;
}

Bigint Bigint::operator-() noexcept {
  positive = !positive;
  return *this;
}

Bigint Bigint::operator*(const Bigint &right) const {
  return Bigint(*this) *= right;
}

Bigint &Bigint::operator*=(const Bigint &right) {
  if (right.number.size() == 1) {
    *this *= right.positive ? right.number[0] : -right.number[0];

    return *this;
  }

  SmallVector<int32_t> result{ 0 };
  int64_t sum{ 0 };

  result.reserve(number.size() + right.number.size());

  int32_t start{ 0 };

  for (int32_t val_left : number) {
    int32_t *ptr_result{ result.begin() + start };
    for (int32_t it2 : right.number) {
      sum += static_cast<int64_t>(val_left) * it2;
      if (ptr_result == result.end()) {
        result.push_back(sum % BLOCK);
        ptr_result = result.end();
      } else {
        sum += *ptr_result;
        *ptr_result = static_cast<int32_t>(sum % BLOCK);
        ++ptr_result;
      }
      sum /= BLOCK;
    }
    if (sum != 0) {
      result.push_back(sum % BLOCK);
      sum = 0;
    }
    ++start;
  }

  positive = positive == right.positive;
  number = std::move(result);

  return *this;
}

Bigint Bigint::operator*(int64_t value) const {
  Bigint c{ *this };
  c *= value;

  return c;
}

Bigint &Bigint::operator*=(int64_t value) {
  if (value == 0) {
    clear();

    return *this;
  }
  bool value_sign{ true };
  int64_t copy_value{ value };

  if (value < 0) {
    copy_value = -copy_value;
    value_sign = false;
  }

  if (copy_value >= BLOCK_SQ) {
    *this *= Bigint(value);

    return *this;
  }

  positive = value_sign == positive;

  SmallVector<int32_t> result{ number.size() };

  int32_t *ptr_left{ number.begin() };
  int64_t sum{ 0 };

  const int64_t lowPart{ copy_value % BLOCK };
  const int64_t highPart{ copy_value / BLOCK };

  while (ptr_left != number.end()) {
    sum += lowPart * (*ptr_left);
    result.push_back(static_cast<int32_t>(sum % BLOCK));
    sum /= BLOCK;
    ++ptr_left;
  }
  if (sum != 0) {
    result.push_back(static_cast<int32_t>(sum));
    sum = 0;
  }

  if (highPart != 0) {
    ptr_left = number.begin();

    int32_t *ptr_result{ result.begin() + 1 };

    while (ptr_left != number.end()) {
      sum += highPart * (*ptr_left);
      if (ptr_result == result.end()) {
        result.push_back(sum % BLOCK);
      } else {
        *ptr_result += static_cast<int32_t>(sum % BLOCK);
      }
      sum /= BLOCK;
      ++ptr_left;
      ++ptr_result;
    }
    if (sum != 0) {
      result.push_back(sum);
    }
  }

  number = std::move(result);

  return *this;
}

Bigint &Bigint::pow(uint32_t power) {
  std::map<uint32_t, Bigint> lookup;
  if (power % 2 == 0 && !positive) {
    positive = true;
  }
  *this = pow(power, &lookup);

  return *this;
}

int32_t Bigint::digits() const noexcept {
  const size_t segments{ number.size() };

  if (segments == 0) {
    return 1;
  }

  return BLOCK_SIZE * (segments - 1) + Details::segment_length(number.back());
}

bool Bigint::is_even() const noexcept {
  if (number.empty()) {
    return true;
  }

  return number[0] % 2 == 0;
}

bool Bigint::is_negative() const noexcept {
  return !positive;
}


void Bigint::get_fragment(const Bigint &bigint, Bigint &result, int32_t which) {
  int32_t *ptr{ bigint.number.end() - 1 };
  const int32_t size_at_end{ static_cast<int32_t>(std::log10(*ptr)) + 1 };

  result.number.clear();

  if (bigint.number.size() == 1 || size_at_end >= which) {
    result.number.push_back((*ptr / POW10.at(size_at_end - which)));
    return;
  }

  if (bigint.digits() <= which) {
    result.number.copy(bigint.number);
    return;
  }

  which -= size_at_end;
  --ptr;

  const int32_t idx{ which / BLOCK_SIZE };
  ptr -= idx;
  which -= idx * BLOCK_SIZE;

  const int32_t pow{ POW10.at(BLOCK_SIZE - which) };
  const int32_t pow2{ BLOCK / pow };

  result.number.reserve(std::distance(ptr, bigint.number.end()));

  for (; ptr < bigint.number.end() - 1;) {
    const int32_t div{ *ptr / pow };
    ptr++;
    const int32_t div2{ *ptr % pow * pow2 };
    result.number.push_back(div + div2);
  }
  result.number.push_back((*ptr) / pow);

  if (result.number.back() == 0) {
    result.number.pop_back();
  }
}

Bigint Bigint::operator/(const Bigint &right) const {
  if (right.is_zero()) {
    throw std::invalid_argument("Divisor must be non zero");
  }

  Bigint left{ *this };
  Bigint rhs_copy{ right };

  const bool new_positive{ positive == right.positive };
  left.positive = true;
  rhs_copy.positive = true;

  if (left < rhs_copy) {
    left.clear();
    return left;
  }
  if (number.size() == 1) {
    left.clear();
    left.positive = new_positive;
    left.number.push_back(number[0] / right.number[0]);

    return left;
  }

  Bigint sum_quotient;
  Bigint frag_left;
  Bigint temp;
  Bigint temp_quotient{ 1 };

  const std::array<Bigint, 4> look_up{ rhs_copy, rhs_copy * 2, rhs_copy * 4, rhs_copy * 8 };

  const std::array<int32_t, 4> look_up_digits{
    look_up[0].digits(),
    look_up[1].digits(),
    look_up[2].digits(),
    look_up[3].digits()
  };

  while (true) {
    const int32_t digits_left{ left.digits() };
    int32_t digits_right{ look_up_digits[3] };
    temp_quotient.clear();

    if (digits_left - look_up_digits[0] < 0) {
      sum_quotient.positive = new_positive;
      return sum_quotient;
    }

    if (digits_left - digits_right < 0) {
      get_fragment(left, frag_left, look_up_digits[0]);
    } else {
      get_fragment(left, frag_left, digits_right);
    }

    for (uint8_t i = 3;; i--) {
      if (frag_left >= look_up.at(i)) {
        temp.number.copy(look_up.at(i).number);
        digits_right = look_up_digits.at(i);
        temp_quotient.number.push_back(1U << i);
        break;
      }
      if (i == 0) {
        sum_quotient.positive = positive;
        return sum_quotient;
      }
    }

    const int32_t number_of_zeroes{ digits_left - look_up_digits[3] - look_up_digits[3] + digits_right };

    if (number_of_zeroes > 0) {
      sum_quotient += Bigint(temp_quotient).add_zeroes(number_of_zeroes);
      left -= temp.add_zeroes(number_of_zeroes);
    } else {
      sum_quotient += temp_quotient;
      left -= temp;
    }
  }
}

Bigint &Bigint::operator/=(const Bigint &right) {
  *this = *this / right;

  return *this;
}

int64_t Bigint::operator%(const int64_t &value) const {
  if (value == 0) {
    throw std::invalid_argument("Divisor must be non zero");
  }
  if (number.empty()) {
    return 0;
  }
  int64_t remains{ 0 };
  const size_t size{ number.size() - 1 };

  for (size_t idx{ 0 }; idx < number.size(); idx++) {
    remains = (remains + number[size - idx]) % value;
    if (idx != size) {
      remains *= BLOCK;
      remains %= value;
    }
  }

  return positive ? remains : -remains;
}

bool Bigint::operator<(const Bigint &other) const noexcept {
  return compare(other) == -1;
}

bool Bigint::operator>(const Bigint &other) const noexcept {
  return compare(other) == 1;
}

bool Bigint::operator<=(const Bigint &other) const noexcept {
  return compare(other) != 1;
}

bool Bigint::operator>=(const Bigint &other) const noexcept {
  return compare(other) != -1;
}

bool Bigint::operator==(const Bigint &other) const noexcept {
  return compare(other) == 0;
}

bool Bigint::operator!=(const Bigint &other) const noexcept {
  return compare(other) != 0;
}

Bigint &Bigint::operator=(const std::string &string) noexcept {
  Bigint temp{ string };
  number = std::move(temp.number);
  positive = temp.positive;

  return *this;
}

int32_t Bigint::operator()(const int32_t &which) const {
  const int32_t size{ digits() };

  if (which >= size) {
    throw std::out_of_range("Number out of bounds");
  }

  if (which == 0) {
    return number[0] % 10;
  }

  return number[which / BLOCK_SIZE] / POW10.at(which % BLOCK_SIZE) % 10;
}

int32_t Bigint::operator[](const int32_t &which) const {
  const int32_t size{ digits() };

  if (which >= size) {
    throw std::out_of_range("Number out of bounds");
  }

  if (which == 0 && number.back() < 10) {
    return number.back();
  }

  const int32_t size_at_back{ static_cast<int32_t>(std::log10(number.back())) + 1 };

  if (size_at_back > which) {
    return number.back() / POW10.at(size_at_back - which) % 10;
  }

  const int32_t segment{ which - size_at_back };

  const int32_t num{ number[number.size() - 2 - (segment / BLOCK_SIZE)] };
  const int32_t num_length{ Details::segment_length(num) };

  if (num_length < BLOCK_SIZE) {
    return num / POW10.at(segment % BLOCK_SIZE + BLOCK_SIZE - num_length) % 10;
  }

  return num / POW10.at(num_length - 1 - segment % BLOCK_SIZE) % 10;
}

void Bigint::clear() noexcept {
  number.clear();
  positive = true;
}

Bigint &Bigint::abs() noexcept {
  positive = true;

  return *this;
}

std::string Bigint::to_string() const {
  std::ostringstream stream;
  stream << *this;

  return stream.str();
}

Bigint Bigint::clone() const noexcept {
  return Bigint(*this);
}

std::istream &operator>>(std::istream &stream, Bigint &bigint) {
  std::string string;
  stream >> string;

  bigint = Bigint(string);

  return stream;
}

std::ostream &operator<<(std::ostream &stream, const Bigint &bigint) {
  if (bigint.is_zero()) {
    return stream << '0';
  }

  if (!bigint.positive) {
    stream << '-';
  }

  int32_t *ptr{ bigint.number.end() - 1 };

  stream << *ptr--;

  for (; ptr != bigint.number.begin() - 1; --ptr) {
    for (int32_t idx{ 0 }, segment{ Details::segment_length(*ptr) }; idx < BLOCK_SIZE - segment; ++idx) {
      stream << '0';
    }
    if (*ptr != 0) {
      stream << *ptr;
    }
  }

  return stream;
}

Bigint Bigint::pow(uint32_t power, std::map<uint32_t, Bigint> *lookup) {
  if (power == 1) {
    return *this;
  }
  if (power == 0) {
    clear();
    number.push_back(1);

    return *this;
  }

  if (lookup->count(power) != 0) {
    return lookup->at(power);
  }

  uint32_t closest_power{ 1 };
  while (closest_power < power) {
    closest_power <<= 1U;
  }
  closest_power >>= 1U;

  if (power == closest_power) {
    Bigint half{ pow(power / 2, lookup) };
    lookup->emplace(std::pair<uint32_t, Bigint>(power, half * half));
  } else {
    lookup->emplace(std::pair<uint32_t, Bigint>(power, pow(closest_power, lookup) * pow(power - closest_power, lookup)));
  }

  return lookup->at(power);
}

int8_t Bigint::compare(const Bigint &right) const noexcept {
  return compare(right, false);
}

int8_t Bigint::compare(const Bigint &right, bool absolute) const noexcept {
  if (is_zero() && right.is_zero()) {
    return 0;
  }

  if (!absolute && positive && !right.positive) {
    return 1;
  }
  if (!absolute && !positive && right.positive) {
    return -1;
  }

  int32_t signs{ 1 };
  if (!absolute && !positive) {
    signs = -1;
  }

  if (number.size() < right.number.size()) {
    return -1 * signs;
  }
  if (number.size() > right.number.size()) {
    return signs;
  }

  for (size_t idx{ number.size() }; idx > 0; --idx) {
    if (number[idx - 1] < right.number[idx - 1]) {
      return -1 * signs;
    }
    if (number[idx - 1] > right.number[idx - 1]) {
      return signs;
    }
  }

  return 0;
}

bool Bigint::is_zero() const noexcept {
  const size_t size{ number.size() };

  return size == 0 || (size == 1 && number[0] == 0);
}

Bigint &Bigint::add_zeroes(uint64_t amount) {
  if (amount == 0) {
    return *this;
  }

  const int64_t power_of_ten{ POW10.at(amount % BLOCK_SIZE) };

  if (power_of_ten != 1) {
    int32_t accumulator{ 0 };
    for (int32_t &ptr : number) {
      int64_t mul{ power_of_ten * ptr };
      ptr = static_cast<int32_t>((mul + accumulator) % BLOCK);
      accumulator = static_cast<int32_t>(mul / BLOCK);
    }
    if (accumulator != 0) {
      number.push_back(accumulator);
    }
  }

  if (amount / BLOCK_SIZE > 0) {
    std::reverse(number.begin(), number.end());

    number.resize(number.size() + amount / BLOCK_SIZE, 0);

    std::reverse(number.begin(), number.end());
  }

  return *this;
}

Bigint &Bigint::remove_trailing(uint64_t amount) {
  if (amount == 0) {
    return *this;
  }
  if (amount >= static_cast<uint32_t>(digits())) {
    clear();
    return *this;
  }

  std::reverse(number.begin(), number.end());

  const int32_t amount_mod{ static_cast<int32_t>(amount % BLOCK_SIZE) };
  const int32_t power_of_ten{ POW10.at(amount_mod) };
  const int32_t power_of_ten_complement{ POW10.at(BLOCK_SIZE - amount_mod) };

  if (power_of_ten != 1) {
    int32_t accumulator{ 0 };
    for (int32_t &ptr : number) {
      const int32_t temp{ ptr % power_of_ten };
      ptr = power_of_ten_complement * accumulator + ptr / power_of_ten;
      accumulator = temp;
    }
  }

  for (uint32_t to_remove{ 0 }; to_remove < amount / BLOCK_SIZE; to_remove++) {
    number.pop_back();
  }

  std::reverse(number.begin(), number.end());

  while (!number.empty() && number.back() == 0) {
    number.pop_back();
  }

  return *this;
}

std::string to_string(const Bigint &bigint) {
  std::ostringstream stream;
  stream << bigint;

  return stream.str();
}

}// namespace BigMath
