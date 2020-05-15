[![Build Status](https://travis-ci.com/kmdrGroch/BigintCPP.svg?branch=master)](https://travis-ci.com/kmdrGroch/BigintCPP) [![codecov](https://codecov.io/gh/kmdrGroch/BigintCPP/branch/master/graph/badge.svg)](https://codecov.io/gh/kmdrGroch/BigintCPP)

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
*  [isEven](#isEven)
*  [isNegative](#isNegative)
*  [toString](#tostring)
*  [clone](#clone)

[Functions](#functions)

*  [toString](#tostringbigint)

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

## isEven()
Returns true is the number is even.
```C++
Bigint a = 4558;
cout << a.isEven(); // true
```

## isNegative()
Returns true is the number is negative.
```C++
Bigint a = -4558;
cout << a.isNegative(); // true
```

## toString()
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

# Functions

## toString(Bigint)
Converts the big integer to a string.
```C++
string str;
Bigint a = 455897864531248;
str = toString(a);
```
