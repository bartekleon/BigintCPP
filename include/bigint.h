#pragma once

#ifndef BIGINT_H_
#define BIGINT_H_

#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <cmath>
#include <string_view>

class Bigint {

private:
	std::vector<int> number;
	mutable bool positive;

public:
	// Constructors
	Bigint();
	Bigint(long long);
	Bigint(std::string_view);
	Bigint(const Bigint&);

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
	Bigint operator=(const long long&);

	// Access
	int operator[](int const&);

	// Input & Output
	friend std::istream& operator>>(std::istream&, Bigint&);
	friend std::ostream& operator<<(std::ostream&, Bigint const&);

	// Helpers
	void clear();
	Bigint& abs();
	std::string toString();
	int digits() const;
	bool isEven();
	bool isNegative();
	Bigint clone();
	void flipPositive() const;

	Bigint& addZeroes(int);

	// Power
	Bigint& pow(int const&);

private:
	int segmentLength(int) const;
	Bigint pow(int const&, std::map<int, Bigint>&);
	int compare(Bigint const&) const; // 0 a == b, -1 a < b, 1 a > b
	Bigint getFragment(Bigint&, int);
};

std::string toString(Bigint const&);

#endif /* BIGINT_H_ */