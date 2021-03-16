#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <deque>
#include <cctype>
#include <set>
#include <map>
#include <iomanip>
#include <utility>
#include <cmath>
#include <numeric>
#include <typeinfo>
#include <exception>

using namespace std;

int sgn(int x) {
    return (x > 0) - (x < 0);
}

int gcd(int a, int b) {
    return (b ? gcd(b, a % b) : a);
}

class Rational;
Rational operator + (const Rational& a, const Rational& b);
Rational operator + (const int& a, const Rational& b);
Rational operator + (const Rational& a, const int& b);
Rational operator - (const Rational& a, const Rational& b);
Rational operator - (const int& a, const Rational& b);
Rational operator - (const Rational& a, const int& b);
Rational operator * (const Rational& a, const Rational& b);
Rational operator * (const int& a, const Rational& b);
Rational operator * (const Rational& a, const int& b);
Rational operator / (const Rational& a, const Rational& b);
Rational operator / (const int& a, const Rational& b);
Rational operator / (const Rational& a, const int& b);


class Rational {
  private:
    int x, y;

  public:
    Rational(int a = 0, int b = 1)
        : x(sgn(a) * sgn(b) * abs(a) / gcd(abs(a), abs(b)))
        , y(abs(b) / gcd(abs(a), abs(b))) {
        if (b == 0) {
            throw std::invalid_argument("zero");
        }
    }

    int numerator() const {
        return x;
    }

    int denominator() const {
        return abs(y);
    }

    Rational& operator += (Rational& other) {
        *this = *this + other;
        return *this;
    }

    Rational& operator -= (const Rational& other) {
        *this = *this - other;
        return *this;
    }

    Rational& operator *= (const Rational& other) {
        *this = *this * other;
        return *this;
    }

    Rational& operator /= (const Rational& other) {
        *this = *this / other;
        return *this;
    }

    Rational& operator ++() {
        *this = *this + 1;
        return *this;
    }

    Rational& operator --() {
        *this = *this - 1;
        return *this;
    }

    Rational operator ++(int) {
        Rational temp(x, y);
        ++(*this);
        return temp;
    }

    Rational operator --(int) {
        Rational temp(x, y);
        --(*this);
        return temp;
    }
};

std::ostream& operator << (std::ostream& out, const Rational& z) {
    out << z.numerator() << " / " << z.denominator();
    return out;
}

Rational operator - (const Rational& x) {
    return Rational(-x.numerator(), x.denominator());
}

Rational operator + (const Rational& x) {
    return Rational(x.numerator(), x.denominator());
}

Rational operator + (const Rational& a, const Rational& b) {
    int x = a.numerator() * b.denominator() + b.numerator() * a.denominator();
    int y = a.denominator() * b.denominator();
    return Rational(
        x,
        y);
}

Rational operator - (const Rational& a, const Rational& b) {
    int x = a.numerator() * b.denominator() - b.numerator() * a.denominator();
    int y = a.denominator() * b.denominator();
    return Rational(
        x,
        y);
}

Rational operator * (const Rational& a, const Rational& b) {
    int x = a.numerator() * b.numerator();
    int y = a.denominator() * b.denominator();
    return Rational(
        x,
        y);
}

Rational operator / (const Rational& a, const Rational& b) {
    int x = a.numerator() * b.denominator();
    int y = a.denominator() * b.numerator();
    return Rational(
        x,
        y);
}

Rational operator + (const Rational& a, const int& b) {
    Rational bb(b, 1);
    int x = a.numerator() * bb.denominator() + bb.numerator() * a.denominator();
    int y = a.denominator() * bb.denominator();
    return Rational(
        x,
        y);
}

Rational operator + (const int& a, const Rational& b) {
    Rational aa(a, 1);
    int x = aa.numerator() * b.denominator() + b.numerator() * aa.denominator();
    int y = aa.denominator() * b.denominator();
    return Rational(
        x,
        y);
}

Rational operator - (const Rational& a, const int& b) {
    Rational bb(b, 1);
    int x = a.numerator() * bb.denominator() - bb.numerator() * a.denominator();
    int y = a.denominator() * bb.denominator();
    return Rational(
        x,
        y);
}

Rational operator - (const int& a, const Rational& b) {
    Rational aa(a, 1);
    int x = aa.numerator() * b.denominator() - b.numerator() * aa.denominator();
    int y = aa.denominator() * b.denominator();
    return Rational(
        x,
        y);
}

Rational operator * (const int& a, const Rational& b) {
    Rational aa(a, 1);
    int x = aa.numerator() * b.numerator();
    int y = aa.denominator() * b.denominator();
    return Rational(
        x,
        y);
}

Rational operator * (const Rational& a, const int& b) {
    Rational bb(b, 1);
    int x = a.numerator() * bb.numerator();
    int y = a.denominator() * bb.denominator();
    return Rational(
        x,
        y);
}

Rational operator / (const Rational& a, const int& b) {
    Rational bb(b, 1);
    int x = a.numerator() * bb.denominator();
    int y = a.denominator() * bb.numerator();
    return Rational(
        x,
        y);
}

Rational operator / (const int& a, const Rational& b) {
    Rational aa(a, 1);
    int x = aa.numerator() * b.denominator();
    int y = aa.denominator() * b.numerator();
    return Rational(
        x,
        y);
}

bool operator == (const Rational& a, const Rational& b) {
    return a.denominator() == b.denominator() && a.numerator() == b.numerator();
}

bool operator != (const Rational& a, const Rational& b) {
    return !(a.denominator() == b.denominator() && a.numerator() == b.numerator());
}

