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
#include <vector>

namespace BigMath {
	class Bigint {

	private:
		std::vector<int> number;
		mutable bool positive = true;

	public:
		// Constructors
		Bigint() = default;
		~Bigint() = default;
		Bigint(const Bigint&) = default;
		Bigint(Bigint&&) = default;

		Bigint(int64_t);
		Bigint(std::string_view);

		// Adding
		Bigint operator+(Bigint const&) const;
		Bigint& operator+=(Bigint const&);
		Bigint operator+(int64_t const&) const;
		Bigint& operator+=(int64_t);

		// Subtraction
		Bigint operator-(Bigint const&) const;
		Bigint& operator-=(Bigint const&);
		Bigint operator-(int64_t const&) const;
		Bigint& operator-=(int64_t);

		// Unary minus
		Bigint operator-();

		// Multiplication
		Bigint operator*(Bigint const&) const;
		Bigint& operator*=(Bigint const&);
		Bigint operator*(int64_t) const;
		Bigint& operator*=(int64_t const&);

		// Division
		Bigint operator/(Bigint q);
		Bigint& operator/=(Bigint const&);

		// Modulo
		[[nodiscard]] int64_t operator%(int64_t const&) const;

		// Comparison
		[[nodiscard]] bool operator<(const Bigint&) const;
		[[nodiscard]] bool operator>(const Bigint&) const;
		[[nodiscard]] bool operator<=(const Bigint&) const;
		[[nodiscard]] bool operator>=(const Bigint&) const;
		[[nodiscard]] bool operator==(const Bigint&) const;
		[[nodiscard]] bool operator!=(const Bigint&) const;

		// Allocation
		Bigint& operator=(const std::string&);
		Bigint& operator=(const Bigint&) = default;
		Bigint& operator=(Bigint&&) = default;

		// Access
		[[nodiscard]] int operator[](int const&) const;

		// Input & Output
		friend std::istream& operator>>(std::istream&, Bigint&);
		friend std::ostream& operator<<(std::ostream&, Bigint const&);

		// Helpers
		void clear();
		Bigint& abs();
		[[nodiscard]] std::string toString() const;
		[[nodiscard]] int digits() const;
		[[nodiscard]] bool isEven();
		[[nodiscard]] bool isNegative() const;
		Bigint clone();
		constexpr void flipPositive() const;

		Bigint& addZeroes(unsigned int);

		// Power
		Bigint& pow(unsigned int const&);

	private:
		[[nodiscard]] constexpr static int segmentLength(int);
		[[nodiscard]] Bigint pow(unsigned int const&, std::map<int, Bigint>*);
		[[nodiscard]] int compare(Bigint const&) const; // 0 a == b, -1 a < b, 1 a > b
		[[nodiscard]] static Bigint getFragment(const Bigint&, int);
	};

	std::string toString(Bigint const&);
}

#endif /* BIGINT_H_ */
