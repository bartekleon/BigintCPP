#pragma once

#ifndef BIGINT_H_
#define BIGINT_H_

#include <algorithm>
#include <cmath>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

class Bigint {

private:
	std::vector<int> number;
	mutable bool positive = true;

public:
	// Constructors
	Bigint() = default;
	virtual ~Bigint() = default;
	Bigint(const Bigint&) = default;
	Bigint(Bigint&&) = default;

	Bigint(long long);
	Bigint(std::string_view);

	// Adding
	Bigint operator+(Bigint const&) const;
	Bigint& operator+=(Bigint const&);
	Bigint operator+(long long const&) const;
	Bigint& operator+=(long long);

	// Subtraction
	Bigint operator-(Bigint const&) const;
	Bigint& operator-=(Bigint const&);

	// Unary minus
	Bigint operator-();

	// Multiplication
	Bigint operator*(Bigint const&) const;
	Bigint& operator*=(Bigint const&);
	Bigint operator*(long long) const;
	Bigint& operator*=(long long const&);

	// Division
	Bigint operator/(Bigint q);
	Bigint& operator/=(Bigint const&);

	// Modulo
	long long operator%(long long const&);

	// Comparison
	bool operator<(const Bigint&) const;
	bool operator>(const Bigint&) const;
	bool operator<=(const Bigint&) const;
	bool operator>=(const Bigint&) const;
	bool operator==(const Bigint&) const;
	bool operator!=(const Bigint&) const;

	// Allocation
	Bigint& operator=(const std::string&);
	Bigint& operator=(const Bigint&) = default;
	Bigint& operator=(Bigint&&) = default;

	// Access
	int operator[](int const&);

	// Input & Output
	friend std::istream& operator>>(std::istream&, Bigint&);
	friend std::ostream& operator<<(std::ostream&, Bigint const&);

	// Helpers
	void clear();
	Bigint& abs();
	std::string toString() const;
	int digits() const;
	bool isEven();
	bool isNegative() const;
	Bigint clone();
	void flipPositive() const;

	Bigint& addZeroes(int);

	// Power
	Bigint& pow(int const&);

private:
	static int segmentLength(int);
	Bigint pow(int const&, std::map<int, Bigint>&);
	int compare(Bigint const&) const; // 0 a == b, -1 a < b, 1 a > b
	static Bigint getFragment(Bigint&, int);
};

std::string toString(Bigint const&);

#endif /* BIGINT_H_ */
