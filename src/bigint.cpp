#include "bigint.h"

Bigint::Bigint(long long value) {
	if (value < 0) {
		positive = false;
		value *= -1;
	} else {
		positive = true;
	}

	while (value != 0) {
		number.push_back((int)(value % 1000000000));
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
		int prefix = 1;

		for (int i = size - 1; i >= 0 && i >= size - 9; --i) {
			if (stringInteger[i] < '0' || stringInteger[i] > '9') {
				break;
			}
			num += (stringInteger[i] - '0') * prefix;
			prefix *= 10;
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

	auto it1 = this->number.begin();
	auto it2 = b.number.begin();
	int sum = 0;

	while (it1 != number.end() || it2 != b.number.end()) {
		if (it1 != number.end()) {
			sum += *it1;
		} else {
			number.push_back(0);
			it1 = number.end() - 1;
		}
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

Bigint Bigint::operator+(long long const& b) const {
	Bigint c = *this;
	c += b;

	return c;
}

Bigint& Bigint::operator+=(long long b) {
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
	if (b == 0) {
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
		*this = Bigint(1).addZeroes(9 * (int)number.size()) - *this;
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
		if (b.positive) {
			return (Bigint)*this *= b.number[0];
		}

		return (Bigint)*this *= (long long)b.number[0] * -1;
	}

	Bigint c(0);

	int i = 0;
	int j = 0;

	for (int it1 : number) {
		j = 0;
		for (int it2 : b.number) {
			c += Bigint((long long)it1 * it2).addZeroes(i + j);
			j += 9;
		}
		i += 9;
	}

	c.positive = positive == b.positive;

	return c;
}

Bigint& Bigint::operator*=(Bigint const& b) {
	*this = *this * b;

	return *this;
}

Bigint Bigint::operator*(long long b) const {
	Bigint c = *this;
	c *= b;

	return c;
}

Bigint& Bigint::operator*=(long long const& b1) {
	if (b1 == 0) {
		this->clear();

		return *this;
	}
	bool b1_sign = true;
	long long b = b1;

	if (b1 < 0) {
		b = -b;
		b1_sign = false;
	}

	positive = b1_sign == positive;

	auto it = number.begin();
	long long sum = 0;

	while (it != number.end()) {
		sum += (long long)(*it) * b;
		*it = (int)(sum % 1000000000);
		sum /= 1000000000;
		++it;
	}
	if (sum != 0) {
		number.push_back((int)sum);
	}

	return *this;
}

Bigint& Bigint::pow(int const& power) {
	std::map<int, Bigint> lookup;
	if (power % 2 == 0 && !positive) {
		positive = true;
	}
	*this = pow(power, lookup);

	return *this;
}

int Bigint::digits() const {
	int segments = number.size();

	if (segments == 0) {
		return 0;
	}

	return 9 * (segments - 1) + segmentLength(number.back());
}

bool Bigint::isEven() {
	if (number.empty()) {
		return true;
	}

	if (number.size() == 1) {
		return number[0] % 2 == 0;
	}

	return *(number.begin()) % 2 == 0;
}

bool Bigint::isNegative() const {
	return !this->positive;
}

Bigint Bigint::getFragment(Bigint& p, int size) {
	auto it = p.number.end() - 1;
	int log = (int)std::log10(*it) + 1;

	if (p.number.size() == 1 || log >= size) {
		return Bigint((int)(*it / std::pow(10, log - size)));
	}

	if (p.digits() <= size) {
		return Bigint(p);
	}

	size -= log;
	--it;

	int i = size / 9;
	it -= i;
	size -= i * 9;

	int pow = std::pow(10, 9 - size);
	int pow2 = 1000000000 / pow;

	Bigint ret((*it) / pow);

	++it;

	i = 0;

	for (; it < p.number.end(); ++it) {
		ret.number.push_back((*it) / pow);
		ret.number[i] += ((*it) % pow) * pow2;
		i++;
	}

	if (*(ret.number.end() - 1) == 0) {
		ret.number.pop_back();
	}

	return ret;
}

Bigint Bigint::operator/(Bigint q) {
	if (q == Bigint()) {
		throw std::exception("Divisor must be non zero");
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
		number[0] = (int)(number.at(0) / q.number.at(0));

		return *this;
	}

	Bigint sum_quotient, sub_p, tmpx1;
	int tmp_quotient = 0;

	Bigint look_up[4] = { q, q * 2, q * 4, q * 8 };

	int look_up_digits[4] = {
		look_up[0].digits(),
		look_up[1].digits(),
		look_up[2].digits(),
		look_up[3].digits()
	};

	while (true) {
		int digitsP = p.digits();
		int digitsQ = look_up_digits[3];

		if (digitsP - look_up_digits[0] < 0) {
			sum_quotient.positive = positive;
			return sum_quotient;
		}

		if (digitsP - digitsQ < 0) {
			sub_p = getFragment(p, look_up_digits[0]);
		}
		else {
			sub_p = getFragment(p, digitsQ);
		}

		for (int i = 3; i >= 0; i--) {
			if (sub_p >= look_up[i]) {
				tmpx1 = look_up[i];
				digitsQ = look_up_digits[i];
				tmp_quotient = 1 << i;
				break;
			}
			if (i == 0) {
				sum_quotient.positive = positive;
				return sum_quotient;
			}
		}

		int k = digitsP - look_up_digits[3] - (look_up_digits[3] - digitsQ);

		if (k > 0) {
			sum_quotient = sum_quotient + Bigint(tmp_quotient).addZeroes(k);
			p = p - tmpx1.addZeroes(k);
		}
		else {
			sum_quotient = sum_quotient + tmp_quotient;
			p = p - tmpx1;
		}
	}
}

Bigint& Bigint::operator/=(Bigint const& q) {
	*this = *this / q;

	return *this;
}

long long Bigint::operator%(long long const& divisor) {
	long long remains = 0;

	for (int i = 0; i < number.size(); i++) {
		remains = (remains + number[number.size() - i - 1]) % divisor;
		if (i != number.size() - 1) {
			remains *= 1000000000;
			remains %= divisor;
		}
	}

	if (positive) {
		return remains;
	}

	return -remains;
}

bool Bigint::operator<(const Bigint& b) const {
	return compare(b) == -1;
}

bool Bigint::operator>(const Bigint& b) const {
	return compare(b) == 1;
}

bool Bigint::operator<=(const Bigint& b) const {
	int compared = compare(b);

	return compared == 0 || compared == -1;
}

bool Bigint::operator>=(const Bigint& b) const {
	int compared = compare(b);

	return compared == 0 || compared == 1;
}

bool Bigint::operator==(const Bigint& b) const {
	return compare(b) == 0;
}

bool Bigint::operator!=(const Bigint& b) const {
	return compare(b) != 0;
}

Bigint& Bigint::operator=(const std::string& a) {
	Bigint tmp(a);
	number = tmp.number;
	positive = tmp.positive;

	return *this;
}

int Bigint::operator[](int const& b) {
	int size = this->digits();

	if (b >= size) {
		throw std::exception("Number out of bounds");
	}

	if (b == 0 && number.back() < 10) {
		return number.back();
	}

	int sizeAtBack = (int)std::log10(number.back()) + 1;

	if (sizeAtBack > b) {
		return std::to_string(number.back())[b] - '0';
	}

	int f = b - sizeAtBack;

	std::string part = std::to_string(number.at(number.size() - 2 - (f / 9)));

	if (part.size() < 9) {
		return part[f % 9 + (9 - part.size())] - '0';
	}

	return part[f % 9] - '0';
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

int Bigint::segmentLength(int segment) {
	if (segment == 0) {
		return 0;
	}

	return (int)std::log10(segment) + 1;
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

	int i = (int)a.number.size() - 1;
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

Bigint Bigint::pow(int const& power, std::map<int, Bigint>& lookup) {
	if (power < 2) {
		if (power == 1) {
			return *this;
		}
		if (power == 0) {
			this->clear();
			this->number.push_back(1);

			return *this;
		}

		throw std::exception("Exponent must be positive");
	}

	if (lookup.count(power) != 0) {
		return lookup[power];
	}

	int closestPower = 1;
	while (closestPower < power) {
		closestPower <<= 1;
	}
	closestPower >>= 1;

	if (power == closestPower) {
		Bigint doub = pow(power / 2, lookup);
		lookup[power] = doub * doub;
	} else {
		lookup[power] = pow(closestPower, lookup) * pow(power - closestPower, lookup);
	}

	return lookup[power];
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

void Bigint::flipPositive() const {
	this->positive = !this->positive;
}

Bigint& Bigint::addZeroes(int num) {
	if (num == 0) {
		return *this;
	}

	long long ten = std::pow(10, num % 9);

	if (ten != 1) {
		int acum = 0;
		int acum2 = 0;
		for (auto it = number.begin(); it != number.end(); ++it) {
			acum = (int)(ten * (*it) / 1000000000);
			*it = (int)((ten * (*it) + acum2) % 1000000000);
			acum2 = acum;
		}
		if (acum != 0) {
			this->number.push_back(acum);
		}
	}

	if (num / 9 > 0) {
		std::reverse(this->number.begin(), this->number.end());

		for (int i = 0; i < num / 9; i++) {
			this->number.push_back(0);
		}

		std::reverse(this->number.begin(), this->number.end());
	}

	return *this;
}

std::string toString(Bigint const& value) {
	std::ostringstream stream;
	stream << value;

	return stream.str();
}
