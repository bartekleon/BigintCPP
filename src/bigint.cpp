#include "bigint.h"

namespace BigMath {

constexpr static std::array<int32_t, 10> POW10 = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };

Bigint::Bigint(int64_t value) {
  if (value < 0) {
    positive = false;
    value *= -1;
  } else {
    positive = true;
  }

  while (value != 0) {
    number.push_back(value % 1000000000);
    value /= 1000000000;
  }
}

Bigint::Bigint(std::string_view string) {
  int32_t size = string.length();

  positive = string[0] != '-';

  while (true) {
    if (size <= 0) {
      break;
    }
    if (!positive && size <= 1) {
      break;
    }

    int32_t length = 0;
    int32_t num = 0;

    for (int32_t idx = size - 1; idx >= 0 && idx >= size - 9; --idx) {
      if (string[idx] >= '0' && string[idx] <= '9') {
        num += (string[idx] - '0') * POW10.at(length);
      }
      ++length;
    }
    number.push_back(num);
    size -= length;
  }
}

Bigint Bigint::operator+(const Bigint &right) const {
  Bigint c = *this;
  c += right;

  return c;
}

Bigint &Bigint::operator+=(const Bigint &right) {
  if (positive && !right.positive) {
    right.flip_sign();

    *this -= right;

    right.flip_sign();
    return *this;
  }
  if (!positive && right.positive) {
    flip_sign();

    *this -= right;

    flip_sign();
    return *this;
  }

  number.resize(std::max(number.size(), right.number.size()), 0);

  int32_t *ptr_left = number.begin();
  int32_t *ptr_right = right.number.begin();
  int32_t sum = 0;

  while (ptr_left != number.end()) {
    sum += *ptr_left;

    if (ptr_right != right.number.end()) {
      sum += *ptr_right;
      ++ptr_right;
    }
    *ptr_left = sum % 1000000000;
    ptr_left++;
    sum /= 1000000000;
  }
  if (sum != 0) {
    number.push_back(1);
  }

  return *this;
}

Bigint Bigint::operator+(const int64_t &value) const {
  Bigint c = *this;
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
    flip_sign();

    *this -= value;

    flip_sign();
    return *this;
  }
  if (!positive && value < 0) {
    value = -value;
  }

  int32_t *ptr = number.begin();

  while (value != 0) {
    if (ptr != number.end()) {
      *ptr += static_cast<int32_t>(value % 1000000000);
      value /= 1000000000;
      value += *ptr / 1000000000;
      *ptr %= 1000000000;
      ++ptr;
    } else {
      number.push_back(0);
      ptr = number.end() - 1;
    }
  }

  return *this;
}

Bigint Bigint::operator-(const Bigint &right) const {
  Bigint c = *this;
  c -= right;

  return c;
}

Bigint &Bigint::operator-=(const Bigint &right) {
  if (Bigint() == right) {
    return *this;
  }
  if (!positive || !right.positive) {
    right.flip_sign();

    *this += right;

    right.flip_sign();
    return *this;
  }

  int32_t *ptr_left = number.begin();
  int32_t *ptr_right = right.number.begin();
  int32_t diff = 0;

  while (ptr_left != number.end() || ptr_right != right.number.end()) {
    if (ptr_left != number.end()) {
      diff += *ptr_left;
      ++ptr_left;
    } else {
      number.push_back(0);
      ptr_left = number.end();
    }
    if (ptr_right != right.number.end()) {
      diff -= *ptr_right;
      ++ptr_right;
    }
    if (diff < 0) {
      *(ptr_left - 1) = (diff + 1000000000) % 1000000000;
      diff = -1;
    } else {
      *(ptr_left - 1) = diff % 1000000000;
      diff /= 1000000000;
    }
  }
  if (diff < 0) {
    *this = Bigint(1).add_zeroes(9 * number.size()) - *this;
    positive = false;
  }
  while (!number.empty() && number.back() == 0) {
    number.pop_back();
  }

  return *this;
}

Bigint Bigint::operator-(const int64_t &value) const {
  Bigint c = *this;
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

  int32_t *ptr = number.begin();
  int64_t diff = 0;

  while (ptr != number.end() || value != 0) {
    if (ptr != number.end()) {
      diff += *ptr;
      ++ptr;
    } else {
      number.push_back(0);
      ptr = number.end();
    }

    diff -= value % 1000000000;
    value /= 1000000000;

    if (diff < 0) {
      *(ptr - 1) = static_cast<int32_t>((diff + 1000000000) % 1000000000);
      diff = -1;
    } else {
      *(ptr - 1) = static_cast<int32_t>(diff % 1000000000);
      diff /= 1000000000;
    }
  }
  if (diff < 0) {
    *this = Bigint(1).add_zeroes(9 * number.size()) - *this;
    positive = false;
  }
  while (!number.empty() && number.back() == 0) {
    number.pop_back();
  }

  return *this;
}

Bigint Bigint::operator-() {
  flip_sign();
  return *this;
}

Bigint Bigint::operator*(const Bigint &right) const {
  if (right.number.size() == 1) {
    Bigint left = *this;
    left *= right.positive ? right.number[0] : -right.number[0];
    return left;
  }

  Bigint result(0);
  int64_t sum = 0;

  result.number.reserve(number.size() + right.number.size());

  int32_t start = 0;

  for (int32_t val_left : number) {
    int32_t *ptr_result = result.number.begin() + start;
    for (int32_t it2 : right.number) {
      sum += static_cast<int64_t>(val_left) * it2;
      if (ptr_result == result.number.end()) {
        result.number.push_back(sum % 1000000000);
        ptr_result = result.number.end();
      } else {
        sum += *ptr_result;
        *ptr_result = static_cast<int32_t>(sum % 1000000000);
        ++ptr_result;
      }
      sum /= 1000000000;
    }
    if (sum != 0) {
      result.number.push_back(sum % 1000000000);
      sum = 0;
    }
    ++start;
  }

  result.positive = positive == right.positive;

  return result;
}

Bigint &Bigint::operator*=(const Bigint &right) {
  *this = *this * right;

  return *this;
}

Bigint Bigint::operator*(int64_t value) const {
  Bigint c = *this;
  c *= value;

  return c;
}

Bigint &Bigint::operator*=(const int64_t &value) {
  if (value == 0) {
    clear();

    return *this;
  }
  bool value_sign = true;
  int64_t copy_value = value;

  if (value < 0) {
    copy_value = -copy_value;
    value_sign = false;
  }

  if (copy_value >= 1000000000000000000) {
    *this = *this * Bigint(value);

    return *this;
  }

  positive = value_sign == positive;

  SmallVector<int32_t> result;
  result.reserve(number.size());

  int32_t *ptr_left = number.begin();
  int64_t sum = 0;

  int64_t lowPart = copy_value % 1000000000;
  int64_t highPart = copy_value / 1000000000;

  while (ptr_left != number.end()) {
    sum += lowPart * (*ptr_left);
    result.push_back(sum % 1000000000);
    sum /= 1000000000;
    ++ptr_left;
  }
  if (sum != 0) {
    result.push_back(sum);
    sum = 0;
  }

  if (highPart != 0) {
    ptr_left = number.begin();

    int32_t *ptr_result = result.begin() + 1;

    while (ptr_left != number.end()) {
      sum += highPart * (*ptr_left);
      if (ptr_result == result.end()) {
        result.push_back(sum % 1000000000);
      } else {
        *ptr_result += static_cast<int32_t>(sum % 1000000000);
      }
      sum /= 1000000000;
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

Bigint &Bigint::pow(const uint32_t &power) {
  std::map<int32_t, Bigint> lookup;
  if (power % 2 == 0 && !positive) {
    positive = true;
  }
  *this = pow(power, &lookup);

  return *this;
}

int32_t Bigint::digits() const {
  int32_t segments = number.size();

  if (segments == 0) {
    return 1;
  }

  return 9 * (segments - 1) + segment_length(number.back());
}

// TODO: Change to const
bool Bigint::is_even() const {
  if (number.empty()) {
    return true;
  }

  return number[0] % 2 == 0;
}

bool Bigint::is_negative() const {
  return !positive;
}

Bigint Bigint::get_fragment(const Bigint &bigint, int32_t which) {
  int32_t *ptr = bigint.number.end() - 1;
  const int32_t size_at_end = static_cast<int32_t>(std::log10(*ptr)) + 1;

  if (bigint.number.size() == 1 || size_at_end >= which) {
    return Bigint((*ptr / POW10.at(size_at_end - which)));
  }

  if (bigint.digits() <= which) {
    return Bigint(bigint);
  }

  which -= size_at_end;
  --ptr;

  int32_t idx = which / 9;
  ptr -= idx;
  which -= idx * 9;

  int32_t pow = POW10.at(9 - which);
  int32_t pow2 = 1000000000 / pow;

  Bigint result((*ptr) / pow);

  result.number.reserve(std::distance(ptr, bigint.number.end()));

  ++ptr;

  idx = 0;

  for (; ptr < bigint.number.end(); ++ptr) {
    result.number.push_back((*ptr) / pow);
    result.number[idx] += ((*ptr) % pow) * pow2;
    idx++;
  }

  if (result.number.back() == 0) {
    result.number.pop_back();
  }

  return result;
}

Bigint Bigint::operator/(Bigint right) {
  if (right == Bigint()) {
    throw std::invalid_argument("Divisor must be non zero");
  }

  Bigint left = *this;

  positive = positive == right.positive;

  left.positive = true;
  right.positive = true;

  if (left < right) {
    clear();

    return *this;
  }
  if (number.size() == 1) {
    number[0] = number[0] / right.number[0];

    return *this;
  }

  Bigint sum_quotient;
  Bigint frag_left;
  Bigint temp;
  uint32_t temp_quotient = 0;

  const std::array<Bigint, 4> look_up = { right, right * 2, right * 4, right * 8 };

  const std::array<int32_t, 4> look_up_digits = {
    look_up[0].digits(),
    look_up[1].digits(),
    look_up[2].digits(),
    look_up[3].digits()
  };

  while (true) {
    const int32_t digits_left = left.digits();
    int32_t digits_right = look_up_digits[3];

    if (digits_left - look_up_digits[0] < 0) {
      sum_quotient.positive = positive;
      return sum_quotient;
    }

    if (digits_left - digits_right < 0) {
      frag_left = get_fragment(left, look_up_digits[0]);
    } else {
      frag_left = get_fragment(left, digits_right);
    }

    for (uint8_t i = 3; i >= 0; i--) {
      if (frag_left >= look_up.at(i)) {
        temp = look_up.at(i);
        digits_right = look_up_digits.at(i);
        temp_quotient = 1U << i;
        break;
      }
      if (i == 0) {
        sum_quotient.positive = positive;
        return sum_quotient;
      }
    }

    const int32_t number_of_zeroes = digits_left - look_up_digits[3] - look_up_digits[3] + digits_right;

    if (number_of_zeroes > 0) {
      sum_quotient = sum_quotient + Bigint(temp_quotient).add_zeroes(number_of_zeroes);
      left = left - temp.add_zeroes(number_of_zeroes);
    } else {
      sum_quotient = sum_quotient + temp_quotient;
      left = left - temp;
    }
  }
}

Bigint &Bigint::operator/=(const Bigint &right) {
  *this = *this / right;

  return *this;
}

int64_t Bigint::operator%(const int64_t &value) const {
  if (number.empty()) {
    return 0;
  }
  int64_t remains = 0;
  const size_t s = number.size() - 1;

  for (size_t i = 0; i < number.size(); i++) {
    remains = (remains + number[s - i]) % value;
    if (i != s) {
      remains *= 1000000000;
      remains %= value;
    }
  }

  return positive ? remains : -remains;
}

bool Bigint::operator<(const Bigint &other) const {
  return compare(other) == -1;
}

bool Bigint::operator>(const Bigint &other) const {
  return compare(other) == 1;
}

bool Bigint::operator<=(const Bigint &other) const {
  return compare(other) != 1;
}

bool Bigint::operator>=(const Bigint &other) const {
  return compare(other) != -1;
}

bool Bigint::operator==(const Bigint &other) const {
  return compare(other) == 0;
}

bool Bigint::operator!=(const Bigint &other) const {
  return compare(other) != 0;
}

Bigint &Bigint::operator=(const std::string &string) {
  Bigint temp(string);
  number = std::move(temp.number);
  positive = temp.positive;

  return *this;
}

int32_t Bigint::operator()(const int32_t &which) const {
  const int32_t size = digits();

  if (which >= size) {
    throw std::out_of_range("Number out of bounds");
  }

  if (which == 0) {
    return number[0] % 10;
  }

  return number[which / 9] / POW10.at(which % 9) % 10;
}

int32_t Bigint::operator[](const int32_t &which) const {
  const int32_t size = digits();

  if (which >= size) {
    throw std::out_of_range("Number out of bounds");
  }

  if (which == 0 && number.back() < 10) {
    return number.back();
  }

  const int32_t size_at_back = static_cast<int32_t>(std::log10(number.back())) + 1;

  if (size_at_back > which) {
    return number.back() / POW10.at(size_at_back - which) % 10;
  }

  const int32_t segment = which - size_at_back;

  const int32_t num = number[number.size() - 2 - (segment / 9)];
  const int32_t num_length = segment_length(num);

  if (num_length < 9) {
    return num / POW10.at(segment % 9 + (9 - num_length)) % 10;
  }

  return num / POW10.at(num_length - 1 - segment % 9) % 10;
}

void Bigint::clear() {
  number.clear();
  positive = true;
}

Bigint &Bigint::abs() {
  positive = true;

  return *this;
}

std::string Bigint::to_string() const {
  std::ostringstream stream;
  stream << *this;

  return stream.str();
}

Bigint Bigint::clone() const {
  return Bigint(*this);
}

constexpr int32_t Bigint::segment_length(int32_t segment) {
  if (segment == 0) {
    return 1;
  }

  return static_cast<int32_t>(std::log10(segment)) + 1;
}

std::istream &operator>>(std::istream &stream, Bigint &bigint) {
  std::string string;
  stream >> string;

  bigint = Bigint(string);

  return stream;
}

std::ostream &operator<<(std::ostream &stream, const Bigint &bigint) {
  if (bigint.number.empty()) {
    return stream << '0';
  }

  int64_t length = bigint.number.size() - 1;
  for (; length >= 0 && bigint.number[length] == 0; --length) {};

  if (length == -1) {
    return stream << '0';
  }
  if (!bigint.positive) {
    stream << '-';
  }

  int32_t *ptr = bigint.number.end() - bigint.number.size() + length;

  stream << *ptr--;

  for (; ptr != bigint.number.begin() - 1; --ptr) {
    for (int32_t idx = 0, segment = bigint.segment_length(*ptr); idx < 9 - segment; ++idx) {
      stream << '0';
    }
    if (*ptr != 0) {
      stream << *ptr;
    }
  }

  return stream;
}

Bigint Bigint::pow(const uint32_t &power, std::map<int32_t, Bigint> *lookup) {
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

  uint32_t closest_power = 1;
  while (closest_power < power) {
    closest_power <<= 1U;
  }
  closest_power >>= 1U;

  if (power == closest_power) {
    Bigint half = pow(power / 2, lookup);
    lookup->emplace(std::pair<int32_t, Bigint>(power, half * half));
  } else {
    lookup->emplace(std::pair<int32_t, Bigint>(power, pow(closest_power, lookup) * pow(power - closest_power, lookup)));
  }

  return lookup->at(power);
}

int8_t Bigint::compare(const Bigint &right) const {
  if (right.number.empty()) {
    if (number.empty()) {
      return 0;
    }
    if (number.size() == 1 && number[0] == 0) {
      return 0;
    }
  }
  if (number.empty()) {
    if (right.number.size() == 1 && right.number[0] == 0) {
      return 0;
    }
  }

  if (positive && !right.positive) {
    return 1;
  }
  if (!positive && right.positive) {
    return -1;
  }

  int32_t signs = 1;
  if (!positive && !right.positive) {
    signs = -1;
  }

  if (number.size() < right.number.size()) {
    return -1 * signs;
  }
  if (number.size() > right.number.size()) {
    return signs;
  }

  for (int32_t idx = number.size(); idx > 0; --idx) {
    if (number[idx - 1] < right.number[idx - 1]) {
      return -1 * signs;
    }
    if (number[idx - 1] > right.number[idx - 1]) {
      return signs;
    }
  }

  return 0;
}

void Bigint::flip_sign() const {
  positive = !positive;
}

Bigint &Bigint::add_zeroes(uint32_t amount) {
  if (amount == 0) {
    return *this;
  }

  const int64_t power_of_ten = POW10.at(amount % 9);

  if (power_of_ten != 1) {
    int32_t accumulator = 0;
    for (int32_t &ptr : number) {
      int64_t mul = power_of_ten * ptr;
      ptr = static_cast<int32_t>((mul + accumulator) % 1000000000);
      accumulator = static_cast<int32_t>(mul / 1000000000);
    }
    if (accumulator != 0) {
      number.push_back(accumulator);
    }
  }

  if (amount / 9 > 0) {
    std::reverse(number.begin(), number.end());

    number.resize(number.size() + amount / 9, 0);

    std::reverse(number.begin(), number.end());
  }

  return *this;
}

Bigint &Bigint::remove_trailing(uint32_t amount) {
  if (amount == 0) {
    return *this;
  }
  if (amount >= static_cast<uint32_t>(digits())) {
    clear();
    return *this;
  }

  std::reverse(number.begin(), number.end());

  const int32_t power_of_ten = POW10.at(amount % 9);
  const int32_t power_of_ten_complement = POW10.at(9 - amount % 9);

  if (power_of_ten != 1) {
    int32_t accumulator = 0;
    for (int32_t &ptr : number) {
      int32_t temp = ptr % power_of_ten;
      ptr = power_of_ten_complement * accumulator + ptr / power_of_ten;
      accumulator = temp;
    }
  }

  for (uint32_t to_remove = 0; to_remove < amount / 9; to_remove++) {
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
