#include "bigint.h"

namespace BigMath {
	
constexpr static std::array<int, 10> pow10 = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };

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

Bigint::Bigint(std::string_view stringInteger) {
	int size = stringInteger.length();

	positive = stringInteger[0] != '-';

	while (true) {
		if (size <= 0) {
			break;
		}
		if (!positive && size <= 1) {
			break;
		}

		int length = 0;
		int num = 0;

		for (int i = size - 1; i >= 0 && i >= size - 9; --i) {
			if (stringInteger[i] < '0' || stringInteger[i] > '9') {
				break;
			}
			num += (stringInteger[i] - '0') * pow10.at(length);
			++length;
		}
		number.push_back(num);
		size -= length;
	}
}

Bigint Bigint::operator+(Bigint const& b) const {
	Bigint c = *this;
	c += b;

	return c;
}

Bigint& Bigint::operator+=(Bigint const& b) {
	if (positive && !b.positive) {
		b.flipPositive();

		*this -= b;

		b.flipPositive();
		return *this;
	}
	if (!positive && b.positive) {
		flipPositive();

		*this -= b;

		flipPositive();
		return *this;
	}

	number.resize(std::max(number.size(), b.number.size()), 0);

	auto it1 = number.begin();
	auto it2 = b.number.begin();
	int sum = 0;

	while (it1 != number.end()) {
		sum += *it1;

		if (it2 != b.number.end()) {
			sum += *it2;
			++it2;
		}
		*it1 = sum % 1000000000;
		it1++;
		sum /= 1000000000;
	}
	if (sum != 0) {
		number.push_back(1);
	}

	return *this;
}

Bigint Bigint::operator+(int64_t const& b) const {
    Bigint c = *this;
    c += b;

	return c;
}

Bigint& Bigint::operator+=(int64_t b) {
	if (b == 0) {
		return *this;
	}
	if (positive && b < 0) {
		return *this -= -b;
	}
	if (!positive && b > 0) {
		flipPositive();

		*this -= b;

		flipPositive();
		return *this;
	}
	if (!positive && b < 0) {
		b = -b;
	}

	auto it = number.begin();

	while (b != 0) {
		if (it != number.end()) {
			*it += b % 1000000000;
			b /= 1000000000;
			b += *it / 1000000000;
			*it %= 1000000000;
			++it;
		} else {
			number.push_back(0);
			it = number.end() - 1;
		}
	}

	return *this;
}

Bigint Bigint::operator-(Bigint const& b) const {
	Bigint c = *this;
	c -= b;

	return c;
}

Bigint& Bigint::operator-=(Bigint const& b) {
	if (b == Bigint()) {
		return *this;
	}
	if (!positive || !b.positive) {
		b.flipPositive();

		*this += b;

		b.flipPositive();
		return *this;
	}

	auto it1 = number.begin();
	auto it2 = b.number.begin();
	int dif = 0;

	while (it1 != number.end() || it2 != b.number.end()) {
		if (it1 != number.end()) {
			dif += *it1;
			++it1;
		} else {
			number.push_back(0);
			it1 = number.end();
		}
		if (it2 != b.number.end()) {
			dif -= *it2;
			++it2;
		}
		if (dif < 0) {
			*(it1 - 1) = (dif + 1000000000) % 1000000000;
			dif = -1;
		} else {
			*(it1 - 1) = dif % 1000000000;
			dif /= 1000000000;
		}
	}
	if (dif < 0) {
		*this = Bigint(1).addZeroes(9 * number.size()) - *this;
		positive = false;
	}
	while (!number.empty() && number.back() == 0) {
		number.pop_back();
	}

	return *this;
}

Bigint Bigint::operator-(int64_t const& b) const {
	Bigint c = *this;
	c -= b;

	return c;
}

Bigint& Bigint::operator-=(int64_t b) {
	if (b == 0) {
		return *this;
	}
	if (!positive || b < 0) {
		return *this += -b;
	}

	auto it1 = number.begin();
	int64_t dif = 0;

	while (it1 != number.end() || b != 0) {
		if (it1 != number.end()) {
			dif += *it1;
			++it1;
		} else {
			number.push_back(0);
			it1 = number.end();
		}

		dif -= b % 1000000000;
		b /= 1000000000;

		if (dif < 0) {
			*(it1 - 1) = (dif + 1000000000) % 1000000000;
			dif = -1;
		} else {
			*(it1 - 1) = dif % 1000000000;
			dif /= 1000000000;
		}
	}
	if (dif < 0) {
		*this = Bigint(1).addZeroes(9 * number.size()) - *this;
		positive = false;
	}
	while (!number.empty() && number.back() == 0) {
		number.pop_back();
	}

	return *this;
}

Bigint Bigint::operator-() {
	flipPositive();
	return *this;
}

Bigint Bigint::operator*(Bigint const& b) const {
	if (b.number.size() == 1) {
		Bigint c(*this);
        c *= b.positive ? b.number[0] : -b.number[0];
        return c;
	}

	Bigint c(0);
	int64_t sum = 0;

	c.number.reserve(number.size() + b.number.size());

	int i = 0;

	for (int it1 : number) {
		auto ite = c.number.begin() + i;
		for (int it2 : b.number) {
			sum += static_cast<int64_t>(it1) * it2;
			if (ite == c.number.end()) {
				c.number.push_back(sum % 1000000000);
				ite = c.number.end();
			} else {
				sum += *ite;
				*ite = sum % 1000000000;
				++ite;
			}
			sum /= 1000000000;
		}
		if (sum != 0) {
			c.number.push_back(sum % 1000000000);
			sum = 0;
		}
		++i;
	}

	c.positive = positive == b.positive;

	return c;
}

Bigint& Bigint::operator*=(Bigint const& b) {
	*this = *this * b;

	return *this;
}

Bigint Bigint::operator*(int64_t b) const {
	Bigint c = *this;
	c *= b;

	return c;
}

Bigint& Bigint::operator*=(int64_t const& b1) {
	if (b1 == 0) {
		this->clear();

		return *this;
	}
	bool b1_sign = true;
	int64_t b = b1;

	if (b1 < 0) {
		b = -b;
		b1_sign = false;
	}

	if (b >= 1000000000000000000) {
		*this = *this * Bigint(b1);

		return *this;
	}

	positive = b1_sign == positive;

	std::vector<int> answer;
	answer.reserve(number.size());

	auto it = number.begin();
	int64_t sum = 0;

	int64_t lowB = b % 1000000000;
	int64_t highB = b / 1000000000;

	while (it != number.end()) {
		sum += lowB * (*it);
		answer.push_back(sum % 1000000000);
		sum /= 1000000000;
		++it;
	}
	if (sum != 0) {
		answer.push_back(sum);
		sum = 0;
	}

	if (highB != 0) {
		it = number.begin();

		auto ite = answer.begin() + 1;

		while (it != number.end()) {
			sum += highB * (*it);
			if (ite == answer.end()) {
				answer.push_back(sum % 1000000000);
			} else {
				*ite += sum % 1000000000;
			}
			sum /= 1000000000;
			++it;
			++ite;
		}
		if (sum != 0) {
			answer.push_back(sum);
		}
	}

	number = std::move(answer);

	return *this;
}

Bigint& Bigint::pow(unsigned int const& power) {
	std::map<int, Bigint> lookup;
	if (power % 2 == 0 && !positive) {
		positive = true;
	}
	*this = pow(power, &lookup);

	return *this;
}

int Bigint::digits() const {
	int segments = number.size();

	if (segments == 0) {
		return 1;
	}

	return 9 * (segments - 1) + segmentLength(number.back());
}

bool Bigint::isEven() {
	if (number.empty()) {
		return true;
	}

	return number[0] % 2 == 0;
}

bool Bigint::isNegative() const {
	return !this->positive;
}

Bigint Bigint::getFragment(const Bigint& p, int size) {
	auto it = p.number.end() - 1;
	int log = static_cast<int>(std::log10(*it)) + 1;

	if (p.number.size() == 1 || log >= size) {
		return Bigint((*it / pow10.at(log - size)));
	}

	if (p.digits() <= size) {
		return Bigint(p);
	}

	size -= log;
	--it;

	int i = size / 9;
	it -= i;
	size -= i * 9;

	int pow = pow10.at(9 - size);
	int pow2 = 1000000000 / pow;

	Bigint ret((*it) / pow);

	ret.number.reserve(std::distance(it, p.number.end()));

	++it;

	i = 0;

	for (; it < p.number.end(); ++it) {
		ret.number.push_back((*it) / pow);
		ret.number[i] += ((*it) % pow) * pow2;
		i++;
	}

	if (ret.number.back() == 0) {
		ret.number.pop_back();
	}

	return ret;
}

Bigint Bigint::operator/(Bigint q) {
	if (q == Bigint()) {
		throw std::invalid_argument("Divisor must be non zero");
	}

	Bigint p = *this;

	positive = positive == q.positive;

	p.positive = true;
	q.positive = true;

	if (p < q) {
		this->clear();

		return *this;
	}
	if (number.size() == 1) {
		number[0] = number.at(0) / q.number.at(0);

		return *this;
	}

	Bigint sum_quotient;
	Bigint sub_p;
	Bigint tmpx1;
	unsigned int tmp_quotient = 0;

	const std::array<Bigint, 4> look_up = { q, q * 2, q * 4, q * 8 };

	const std::array<int, 4> look_up_digits = {
		look_up[0].digits(),
		look_up[1].digits(),
		look_up[2].digits(),
		look_up[3].digits()
	};

	while (true) {
		const int digitsP = p.digits();
		int digitsQ = look_up_digits[3];

		if (digitsP - look_up_digits[0] < 0) {
			sum_quotient.positive = positive;
			return sum_quotient;
		}

		if (digitsP - digitsQ < 0) {
			sub_p = getFragment(p, look_up_digits[0]);
		} else {
			sub_p = getFragment(p, digitsQ);
		}

		for (unsigned int i = 3; i >= 0; i--) {
			if (sub_p >= look_up.at(i)) {
				tmpx1 = look_up.at(i);
				digitsQ = look_up_digits.at(i);
				tmp_quotient = 1U << i;
				break;
			}
			if (i == 0) {
				sum_quotient.positive = positive;
				return sum_quotient;
			}
		}

		const int k = digitsP - look_up_digits[3] - look_up_digits[3] + digitsQ;

		if (k > 0) {
			sum_quotient = sum_quotient + Bigint(tmp_quotient).addZeroes(k);
			p = p - tmpx1.addZeroes(k);
		} else {
			sum_quotient = sum_quotient + tmp_quotient;
			p = p - tmpx1;
		}
	}
}

Bigint& Bigint::operator/=(Bigint const& q) {
	*this = *this / q;

	return *this;
}

int64_t Bigint::operator%(int64_t const& divisor) const {
	if(number.empty()) {
		return 0;
	}
	int64_t remains = 0;
    const size_t s = number.size() - 1;

	for (size_t i = 0; i < number.size(); i++) {
		remains = (remains + number[s - i]) % divisor;
		if (i != s) {
			remains *= 1000000000;
			remains %= divisor;
		}
	}

	return positive ? remains : -remains;
}

bool Bigint::operator<(const Bigint& b) const {
	return compare(b) == -1;
}

bool Bigint::operator>(const Bigint& b) const {
	return compare(b) == 1;
}

bool Bigint::operator<=(const Bigint& b) const {
	return compare(b) != 1;
}

bool Bigint::operator>=(const Bigint& b) const {
	return compare(b) != -1;
}

bool Bigint::operator==(const Bigint& b) const {
	return compare(b) == 0;
}

bool Bigint::operator!=(const Bigint& b) const {
	return compare(b) != 0;
}

Bigint& Bigint::operator=(const std::string& a) {
	Bigint tmp(a);
	number = std::move(tmp.number);
	positive = tmp.positive;

	return *this;
}

int Bigint::operator[](int const& b) const {
	const int size = this->digits();

	if (b >= size) {
		throw std::out_of_range("Number out of bounds");
	}

	if (b == 0 && number.back() < 10) {
		return number.back();
	}

	const int sizeAtBack = static_cast<int>(std::log10(number.back())) + 1;

	if (sizeAtBack > b) {
		return number.back() / pow10.at(sizeAtBack - b) % 10;
	}

	const int f = b - sizeAtBack;

    const int num = number.at(number.size() - 2 - (f / 9));
    const int numSize = segmentLength(num);

    if (numSize < 9) {
        return num / pow10.at(f % 9 + (9 - numSize)) % 10;
    }

	return num / pow10.at(numSize - 1 - f % 9) % 10;
}

void Bigint::clear() {
	number.clear();
	positive = true;
}

Bigint& Bigint::abs() {
	positive = true;

	return *this;
}

std::string Bigint::toString() const {
	std::ostringstream stream;
	stream << *this;

	return stream.str();
}

Bigint Bigint::clone() {
	return Bigint(*this);
}

constexpr int Bigint::segmentLength(int segment) {
	if (segment == 0) {
		return 1;
	}

	return static_cast<int>(std::log10(segment)) + 1;
}

std::istream& operator>>(std::istream& in, Bigint& a) {
	std::string str;
	in >> str;

	a = Bigint(str);

	return in;
}

std::ostream& operator<<(std::ostream& out, Bigint const& a) {
	if (a.number.empty()) {
		return out << '0';
	}

	int64_t i = a.number.size() - 1;
	for (; i >= 0 && a.number[i] == 0; --i) {};

	if (i == -1) {
		return out << '0';
	}
	if (!a.positive) {
		out << '-';
	}

	auto it = a.number.rbegin() + (a.number.size() - i - 1);

	out << *it++;

	for (; it != a.number.rend(); ++it) {
		for (int i = 0, len = a.segmentLength(*it); i < 9 - len; ++i) {
			out << '0';
		}
		if (*it != 0) {
			out << *it;
		}
	}

	return out;
}

Bigint Bigint::pow(unsigned int const& power, std::map<int, Bigint>* lookup) {
	if (power == 1) {
		return *this;
	}
	if (power == 0) {
		this->clear();
		number.push_back(1);

		return *this;
	}

	if (lookup->count(power) != 0) {
		return lookup->at(power);
	}

	unsigned int closestPower = 1;
	while (closestPower < power) {
		closestPower <<= 1U;
	}
	closestPower >>= 1U;

	if (power == closestPower) {
		Bigint doub = pow(power / 2, lookup);
		lookup->emplace(std::pair<int, Bigint>(power, doub * doub));
	} else {
		lookup->emplace(std::pair<int, Bigint>(power, pow(closestPower, lookup) * pow(power - closestPower, lookup)));
	}

	return lookup->at(power);
}

int Bigint::compare(const Bigint& a) const {
	if (a.number.empty()) {
		if (number.empty()) {
			return 0;
		}
		if (number.size() == 1 && number[0] == 0) {
			return 0;
		}
	}
	if (number.empty()) {
		if (a.number.size() == 1 && a.number[0] == 0) {
			return 0;
		}
	}

	if (positive && !a.positive) {
		return 1;
	}
	if (!positive && a.positive) {
		return -1;
	}

	int check = 1;
	if (!positive && !a.positive) {
		check = -1;
	}

	if (number.size() < a.number.size()) {
		return -1 * check;
	}
	if (number.size() > a.number.size()) {
		return check;
	}

	for (int i = number.size(); i > 0; --i) {
		if (number[i - 1] < a.number[i - 1]) {
			return -1 * check;
		}
		if (number[i - 1] > a.number[i - 1]) {
			return check;
		}
	}

	return 0;
}

constexpr void Bigint::flipPositive() const {
	this->positive = !this->positive;
}

Bigint& Bigint::addZeroes(unsigned int num) {
	if (num == 0) {
		return *this;
	}

	const int64_t ten = pow10.at(num % 9);

	if (ten != 1) {
		int acum = 0;
		for (auto& it : number) {
			int64_t mul = ten * it;
			it = static_cast<int>((mul + acum) % 1000000000);
			acum = static_cast<int>(mul / 1000000000);
		}
		if (acum != 0) {
			number.push_back(acum);
		}
	}

	if (num / 9 > 0) {
		std::reverse(number.begin(), number.end());

		number.resize(number.size() + num / 9, 0);

		std::reverse(number.begin(), number.end());
	}

	return *this;
}

std::string toString(Bigint const& value) {
	std::ostringstream stream;
	stream << value;

	return stream.str();
}

}