[![Build Status](https://travis-ci.com/kmdrGroch/BigintCPP.svg?branch=master)](https://travis-ci.com/kmdrGroch/BigintCPP) [![codecov](https://codecov.io/gh/kmdrGroch/BigintCPP/branch/master/graph/badge.svg)](https://codecov.io/gh/kmdrGroch/BigintCPP)

[Requirements](#requirements)
[Description](#description)

[Operators](#operators)

*  [Addition](#addition)
*  [Subtraction](#subtraction)
*  [Unary minus](#unary-minus)
*  [Multiplication](#multiplication)
*  [Division](#division)
*  [Modulo](#modulo)
*  [Allocation](#allocation)
*  [Comparison](#comparison)
*  [Access](#access)
*  [Stream operators](#stream-operators)

[Methods](#methods)

*  [pow](#powint)
*  [clear](#clear)
*  [abs](#abs)
*  [digits](#digits)
*  [is_even](#is_even)
*  [is_negative](#is_negative)
*  [to_string](#to_string)
*  [clone](#clone)
*  [add_zeroes](#add_zeroes)

[Functions](#functions)

*  [toString](#tostringbigint)

# Requirements

To build this project you need: CMake, make, gcc/clang.
Also build.sh requires you to have installed and in path: clang-format, clang-tidy

For testing you need additionally lcov with it's tools: geninfo and gcov.

# Description

Bigint class provides math operations for arbitrarily large numbers.

# Operators

## Addition
```C++
Bigint a, b, c;
c = a + b;
c += a;
c = a + 6;
c += 6;
```

## Subtraction
```C++
Bigint a, b, c;
c = a - b;
c -= a;
```

## Unary minus
```C++
cout << -Bigint(10) // -10
```

## Multiplication
```C++
Bigint a, b, c;
c = a * b;
c *= a
c = a * 6;
c *= 6;
```

## Division
```C++
Bigint a, b;
Bigint c;
c = a / b;
```

## Modulo
```C++
Bigint a;
int b;
b = a % 31415;
```

## Comparison
```C++
Bigint a = 159753;
Bigint b = 1634687496;

if(a == b) cout << "A is the same as B";
if(a < b) cout << "A is less than B";
if(a > b) cout << "A is larger than B";
if(a >= b) cout << "A is larger than B or equal to it";
if(a <= b) cout << "A is smaller than B or equal to it";
```

## Allocation
```C++
Bigint a = 12345;
Bigint b;
b = 159753;
Bigint c = "12345";
Bigint d = b;
Bigint e;
e = "23498523524";
```

## Access
```C++
Bigint a = 159753;
a.pow(15); //a^15, 1126510743106482...
cout << a[3]; // 6 is the 4th digit
```

## Stream operators
```C++
Bigint a, b;
cin >> a >> b;
cout << a * b;
```

# Methods

## clear()
Clears the Bigint, making it equal to 0.
```C++
Bigint a = 4558;
cout << a.pow(486); // ~1.46 * 10^1778
a.clear();
cout << a; // 0
```

## abs()
Absolute value.
```C++
Bigint a = -4558;
cout << a.abs() // 4558
```

## pow(int)
Raises to the power of N.
```C++
Bigint a = 4558;
cout << a.pow(486); // ~1.46 * 10^1778
```

## digits()
Returns the number of digits.
```C++
Bigint a = 4558;
cout << a.pow(486).digits(); // 4558^486 = 1779 digit number
```

## is_even()
Returns true is the number is even.
```C++
Bigint a = 4558;
cout << a.isEven(); // true
```

## is_negative()
Returns true is the number is negative.
```C++
Bigint a = -4558;
cout << a.isNegative(); // true
```

## to_string()
Converts the big integer to a string.
```C++
string str;
Bigint a = 455897864531248;
str = a.toString();
```

## clone()
Clones Bigint with the same value
```C++
Bigint a = 455897864531248;
Bigint b = a.clone();
cout << a == b // true
```

## add_zeroes()
Multiplies number by pow(10, n) - faster than a*10^n
```C++
Bigint a = 455897864531248;
a.addZeroes(4);
cout << a // 4558978645312480000
cout << a.addZeroes(3) // 4558978645312480000000
```

## remove_trailing()
Divides number by pow(10, n) - faster than a/10^n
```C++
Bigint a = 3433123100000;
a.remove_trailing(4);
cout << a // 34331231

Bigint b = "3433123156364113314123"
cout << a.remove_trailing(11) // 34331231563
```

# Functions

## to_string(Bigint)
Converts the big integer to a string.
```C++
string str;
Bigint a = 455897864531248;
str = toString(a);
```
