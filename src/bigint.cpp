#include "bigint.h"

Bigint::Bigint() {
	this->positive = true;
	this->base = Bigint::default_base;
}

Bigint::Bigint(long long value) {
	base = Bigint::default_base;

	if (value < 0) {
		positive = false;
		value *= -1;
	} else {
		positive = true;
	}

	while (value) {
		number.push_back((int)(value % base));
		value /= base;
	}
}

Bigint::Bigint(std::string stringInteger) {
	int size = stringInteger.length();

	base = Bigint::default_base;
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

Bigint::Bigint(const Bigint& bigint) {
	this->number = bigint.number;
	this->base = bigint.base;
	this->positive = bigint.positive;
}

Bigint Bigint::operator+(Bigint const& b) const
{
	Bigint c = *this;
	c += b;

	return c;
}

Bigint& Bigint::operator+=(Bigint const& b) {
	if (positive && !b.positive) {
		b.positive = !b.positive;
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
	if (!positive && !b.positive) {
		this->flipPositive();
		b.flipPositive();

		*this += b;

		this->flipPositive();
		b.flipPositive();
		return *this;
	}

	std::vector<int>::iterator it1 = this->number.begin();
	std::vector<int>::const_iterator it2 = b.number.begin();
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
		*it1 = sum % base;
		it1++;
		sum /= base;
	}
	if (sum) {
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
		*this -= -b;

		return *this;
	}
	if (!positive && b > 0) {
		flipPositive();

		*this -= b;

		flipPositive();
		return *this;
	}
	if (!positive && b < 0) {
		this->flipPositive();

		*this += -b;

		this->flipPositive();
		return *this;
	}

	std::vector<int>::iterator it = number.begin();

	bool initial_flag = true;

	while (b || initial_flag) {
		initial_flag = false;
		if (it != number.end()) {
			*it += b % base;
			b /= base;
			b += *it / base;
			*it %= base;
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
	std::vector<int>::iterator it1 = number.begin();
	std::vector<int>::const_iterator it2 = b.number.begin();
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
			*(it1 - 1) = (dif + base) % base;
			dif = -1;
		} else {
			*(it1 - 1) = dif % base;
			dif /= base;
		}
	}
	if (dif < 0) {
		std::string newstr = "1";
		std::string str = "";

		for (int i = 1; i < base; i *= 10) {
			str += "0";
		}

		int c_seg = number.size();
		while (c_seg--) {
			newstr += str;
		}
			
		*this = Bigint(newstr) - *this;
		positive = false;
	}
	while (number.size() && !number.back()) {
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

	std::vector<int>::const_iterator it1;
	std::vector<int>::const_iterator it2;
	Bigint c = Bigint(0);

	for (it1 = number.begin(); it1 != number.end(); ++it1) {
		for (it2 = b.number.begin(); it2 != b.number.end(); ++it2) {
			c += (long long)(*it1) * (*it2);
		}
	}

	if (positive != b.positive) {
		c.positive = false;
	}

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
	int b = b1;

	bool old_positive = positive;

	positive = true;

	if (b1 < 0) {
		b *= -1;
		b1_sign = false;
	}

	std::vector<int>::iterator it = number.begin();
	long long sum = 0;

	while (it != number.end()) {
		sum += (long long)(*it) * b;
		*it = (int)(sum % base);
		sum /= base;
		++it;
	}
	if (sum) {
		number.push_back((int)sum);
	}

	if (b1_sign != old_positive) {
		positive = false;
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
	if (number.size() == 0) {
		return true;
	}

	if (number.size() == 1) {
		return number[0] % 2 == 0;
	}

	return *(number.begin()) % 2 == 0;
}

bool Bigint::isNegative() {
	return !this->positive;
}

//Division

Bigint Bigint::operator/(Bigint q) {
	if (q == Bigint()) {
		throw "Divisor must be non zero";
	}

	Bigint p = *this;
	Bigint answer;

	Bigint tmp_quotient, sum_quotient, sub_p, tmpx1;

	bool done_flag = false;

	bool this_sign = this->positive;
	bool q_sign = q.positive;

	p.positive = true;
	q.positive = true;

	std::vector<Bigint> look_up(4);
	look_up[0] = q;
	look_up[1] = q * 2;
	look_up[2] = q * 4;
	look_up[3] = q * 8;

	while (true) {
		sub_p = p;

		for (int i = 0; i < 4; i++) {
			if (sub_p < look_up[i] && i != 0) {
				tmpx1 = look_up[i - 1];
				tmp_quotient = (long long)1 << (i - 1);
				break;
			} else if (sub_p < look_up[i] && i == 0) {
				if (q.digits() >= p.digits()) {
					done_flag = true;
					break;
				}
			} else if (sub_p == look_up[i] || (sub_p > look_up[i] && i == 3)) {
				tmpx1 = look_up[i];
				tmp_quotient = (long long)1 << i;
				break;
			}
		}

		if (done_flag) {
			answer = sum_quotient;
			break;
		}

		std::string temppp(p.digits() - (sub_p.digits()), '0');
		temppp = "1" + temppp;
		
		sum_quotient = sum_quotient + (tmp_quotient * temppp);

		tmpx1 = tmpx1 * temppp;
		p = p - tmpx1;
	}

	answer.positive = this_sign == q_sign;

	return answer;
}

long long Bigint::operator%(long long const& divisor) {
	long long remains = 0;

	for (int i = 0; i < number.size(); i++) {
		remains = (remains + number[number.size() - i - 1]) % divisor;
		if (i != number.size() - 1)	{
			remains *= base;
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

Bigint Bigint::operator=(const long long& a) {
	number.clear();

	long long t = a;

	do {
		number.push_back((int)(t % base));
		t /= base;
	} while (t != 0);

	return *this;
}

int Bigint::operator[](int const& b) {
	return this->toString()[b] - '0';
}

void Bigint::clear() {
	number.clear();
	positive = true;
}

Bigint& Bigint::abs() {
	positive = true;

	return *this;
}

std::string Bigint::toString() {
	std::ostringstream stream;
	stream << *this;

	return stream.str();
}

Bigint Bigint::clone() {
	return Bigint(*this);
}

int Bigint::segmentLength(int segment) const {
	return (int)log10(segment);
}

std::istream& operator>>(std::istream& in, Bigint& a) {
	std::string str;
	in >> str;

	a = str;

	return in;
}

std::ostream& operator<<(std::ostream& out, Bigint const& a) {
	if (!a.number.size()) {
		return out << 0;
	}
	int i = a.number.size() - 1;
	for (; i >= 0 && a.number[i] == 0; --i);

	if (i == -1) {
		return out << 0;
	}
	if (!a.positive) {
		out << '-';
	}

	std::vector<int>::const_reverse_iterator it = a.number.rbegin() + (a.number.size() - i - 1);

	out << *it++;

	for (; it != a.number.rend(); ++it) {
		for (int i = 0, len = a.segmentLength(*it); i < 9 - len; ++i) {
			out << '0';
		}
		if (*it) {
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

		throw "Exponent must be positive";
	}

	if (lookup.count(power)) {
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
	if (a.number.size() == 0) {
		if (number.size() == 0) {
			return 0;
		}
		if (number.size() == 1 && number[0] == 0) {
			return 0;
		}
	}
	if (number.size() == 0) {
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

	for (size_t i(number.size()); i > 0; --i) {
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

std::string toString(Bigint const& value) {
	std::ostringstream stream;
	stream << value;

	return stream.str();
}
