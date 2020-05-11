#pragma once
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <math.h>

class Bigint {

private:
    std::vector<int> number;
    mutable bool positive;
    int base;
    static const int default_base = 1000000000;

public:
    // Constructors
    Bigint();
    Bigint(long long);
    Bigint(std::string);
    Bigint(const Bigint&);

    // Adding
    Bigint operator+(Bigint const&) const;
    Bigint& operator+=(Bigint const&);
    Bigint operator+(long long const&) const;
    Bigint& operator+=(long long);

    // Subtraction
    Bigint operator-(Bigint const&) const;
    Bigint& operator-=(Bigint const&);

    // Multiplication
    Bigint operator*(Bigint const&) const;
    Bigint& operator*=(Bigint const&);
    Bigint operator*(long long) const;
    Bigint& operator*=(long long const&);

    Bigint operator/(Bigint q);

    // Modulo
    long long operator%(long long const&);

    // Compare
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
    void flipPositive() const;
    std::string toString(Bigint const&);
    int digits() const;
    bool isEven();

    // Power
    Bigint& pow(int const&);

private:
    int segmentLength(int) const;
    Bigint pow(int const&, std::map<int, Bigint>&);
    int compare(Bigint const&) const; // 0 a == b, -1 a < b, 1 a > b
};
