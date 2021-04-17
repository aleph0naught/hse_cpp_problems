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
#include <cassert>
#include <iterator>
#include <typeinfo>

using namespace std;


template <typename T>
class Polynomial {
 private:
    vector<T> data;

 public:
    Polynomial(const vector<T>& d) {
        data = d;
        if (data.size()) {
            auto it = data.end();
            --it;
            while ((*it) == T(0) && it != data.begin()) {
                data.erase(it);
                it = data.end();
                --it;
            }
        }
    }

    Polynomial(const T& x = T(0))
    : data(vector<T> {x}) {
    }

    template<typename RandomIt>
    Polynomial(RandomIt first, RandomIt last) {
        data = (vector<T> (first, last));
        auto it = data.end();
        --it;
        while ((*it) == T(0) && it != data.begin()) {
            data.erase(it);
            --it;
        }
    }

    vector<T> GetData() const {
        return data;
    }

    int Degree() const {
        if (data.size()) {
            if (data.size() == 1 && data[0] == T(0)) {
                return -1;
            } else {
                return (data.size() - 1);
            }
        } else {
            return -1;
        }
    }

    bool operator == (const Polynomial &other) const {
        return (data == other.data) ? 1 : 0;
    }

    bool operator == (const T& other) {
        if (other == T(0)) {
            for (size_t i = 0; i < data.size(); ++i) {
                if (data[i] != other) {
                    return 0;
                }
            }
            return 1;
        } else {
            return (data.size() == 1) && (data[0] == other);
        }
    }

    bool operator != (const Polynomial& other) const {
        return !((*this) == other);
    }

    friend Polynomial<T> operator- (const Polynomial<T>& l, const Polynomial<T>& r) {
        vector<T> v;
        Polynomial<T> res(v);
        size_t n = max(l.GetData().size(), r.GetData().size());
        for (size_t i = 0; i < n; ++i) {
            if (i >= l.GetData().size() || i >= r.GetData().size()) {
                if (i >= l.GetData().size()) {
                    res.data.push_back(-r.GetData()[i]);
                } else {
                    res.data.push_back(l.GetData()[i]);
                }
            } else {
                res.data.push_back(l.GetData()[i] - r.GetData()[i]);
            }
        }
        auto it = res.data.end();
        --it;
        while (it != res.data.begin()) {
            if ((*it) == T(0)) {
                res.data.erase(it);
                it = res.data.end();
                it--;
            } else {
                break;
            }
        }
        return res;
    }

    friend Polynomial<T> operator+ (const Polynomial& l, const Polynomial& r) {
        vector<T> v;
        Polynomial<T> res(v);
        size_t n = min(l.GetData().size(), r.GetData().size());
        for (size_t i = 0; i < n; ++i) {
            res.data.push_back(l.GetData()[i] + r.GetData()[i]);
        }
        if (l.GetData().size() > r.GetData().size()) {
            for (size_t i = n; i < l.GetData().size(); ++i) {
               res.data.push_back(l.GetData()[i]);
            }
        } else {
            for (size_t i = n; i < r.GetData().size(); ++i) {
                res.data.push_back(r.GetData()[i]);
            }
        }
        auto it = res.data.end();
        --it;
        while (it != res.data.begin()) {
            if ((*it) == T(0)) {
                res.data.erase(it);
                it = res.data.end();
                it--;
            } else {
                break;
            }
        }
        return res;
    }

    Polynomial<T>& operator += (const Polynomial<T>& other) {
        *this = *this + other;
        return *this;
    }

    Polynomial<T>& operator -= (const Polynomial<T>& other) {
        *this = *this - other;
        return *this;
    }

    Polynomial<T>& operator += (const T& ot) {
        *this = *this + ot;
        return *this;
    }

    Polynomial<T>& operator -= (const T& ot) {
        *this = *this - ot;
        return *this;
    }

    T operator[] (size_t i) const {
        if (i >= data.size()) {
            return T(0);
        }
        return data[i];
    }

    typename vector<T>::const_iterator end() const {
        return data.end();
    }

    typename vector<T>::const_iterator begin() const {
        return data.begin();
    }

    typename vector<T>::iterator end() {
        return data.end();
    }

    typename vector<T>::iterator begin() {
        return data.begin();
    }

    friend Polynomial<T> operator* (const Polynomial& l, const Polynomial& r) {
        std::vector<T> v;
        Polynomial<T> res(v);
        size_t n = l.GetData().size() + r.GetData().size() - 1;
        for (size_t i = 0; i < n; ++i) {
            T push = T(0);
            for (size_t j = 0; j <= i; ++j) {
                T x = T(0);
                if (j < l.GetData().size()) {
                    x = l.GetData()[j];
                }
                T y = T(0);
                if ((i - j) < r.GetData().size()) {
                    y = r.GetData()[i - j];
                }
                push += x * y;
            }

            res.data.push_back(push);
        }
        return res;
    }

    Polynomial& operator *= (const Polynomial &other) {
        *this = *this * other;
        return *this;
    }

    Polynomial& operator *= (const T &other) {
        *this = *this * other;
        return *this;
    }

    T operator () (const T& i) const {
        T res = T(0);
        for (size_t k = 1; k < data.size(); ++k) {
            T ans = T(1);
            for (size_t j = 1; j <= k; ++j) {
                ans *= i;
            }
            ans *= data[k];
            res += ans;
        }
        if (data.size()) {
            res += data[0];
        }
        return res;
    }

    friend Polynomial<T> operator & (const Polynomial& l, const Polynomial& r) {
        Polynomial<T> res(0), ch(T(1)), chh(T(1));
        int i = 0;
        for (auto it = l.begin(); it != l.end(); it++) {
            ch = chh;
            for (int j = 0; j < i; j++) {
                ch = r * ch;
            }
            res += ((*it) * ch);
            ++i;
        }
        return res;
    }

    friend Polynomial<T> operator / (const Polynomial& l, const Polynomial& r) {
        std::vector<T> v;
        Polynomial<T> res(v);
        for (size_t i = 0; i <= l.GetData().size() - r.GetData().size(); i++) {
            res.data.push_back(T(0));
        }
        Polynomial<T> pol = l;
        while (pol.Degree() >= r.Degree() && pol != T(0)) {
            T x = pol.data[pol.data.size() - 1] / r.GetData()[r.GetData().size() - 1];
            res.data[pol.data.size() - r.GetData().size()] = x;
            for (size_t j = 0; j < r.GetData().size(); j++) {
                pol.data[pol.data.size() - j - 1] -= x * (r.GetData()[r.GetData().size() - j - 1]);
            }
            size_t t = pol.data.size() - 1;
            while (pol.data[t] == T(0) && t != 0) {
                pol.data.erase(pol.data.begin() + t);
                t--;
            }
        }
        return res;
    }

    friend Polynomial<T> operator % (const Polynomial& l, const Polynomial& r) {
        std::vector<T> v;
        Polynomial<T> res(v);
        res = l - (l / r) * r;
        return res;
    }

    friend Polynomial<T> operator , (const Polynomial& l, const Polynomial& r) {
        std::vector<T> v;
        Polynomial<T> res(v);
        Polynomial<T> p1, p2;
        p1 = l;
        p2 = r;
        while (p1.GetData().size() > 1 && p2.GetData().size() > 1) {
            if (p1.GetData().size() <= p2.GetData().size()) {
                p2 = p2 % p1;
            } else {
                p1 = p1 % p2;
            }
        }
        if (p1.GetData().size() < p2.GetData().size()) {
            if (p1.GetData()[p1.GetData().size() - 1] != T(0)) {
                if (p1.GetData()[p1.GetData().size() - 1] != T(1)) {
                    for (size_t i = 0; i < p1.GetData().size(); i++) {
                        p1.data[i] = p1.GetData()[i] / p1.GetData()[p1.GetData().size() - 1];
                    }
                }
                return p1;
            } else {
                if (p2.GetData()[p2.GetData().size() - 1] != T(1)) {
                    for (size_t i = 0; i < p2.GetData().size(); i++) {
                        p2.data[i] = p2.GetData()[i] / p2.GetData()[p2.GetData().size() - 1];
                    }
                }
                return p2;
            }
        } else {
            if (p2.GetData()[p2.GetData().size() - 1] != T(0)) {
                if (p2.GetData()[p2.GetData().size() - 1] != T(1)) {
                    for (size_t i = 0; i < p2.GetData().size(); i++) {
                        p2.data[i] = p2.GetData()[i] / p2.GetData()[p2.GetData().size() - 1];
                    }
                }
                return p2;
            } else {
                if (p1.GetData()[p1.GetData().size() - 1] != T(1)) {
                    for (size_t i = 0; i < p1.GetData().size(); i++) {
                        p1.data[i] = p1.GetData()[i] / p1.GetData()[p1.GetData().size() - 1];
                    }
                }
                return p1;
            }
        }
    }
};

template<typename T>
bool operator == (const T& other, const Polynomial<T>& pol) {
    if (other == T(0)) {
        for (size_t i = 0; i < pol.GetData().size(); ++i) {
            if (pol.GetData()[i] != other) {
                return 0;
            }
        }
        return 1;
    } else {
        return (pol.GetData().size() == 1) && (pol.GetData()[0] == other);
    }
}

template<typename T>
bool operator != (const T& other, const Polynomial<T>& pol) {
    return !(pol == other);
}

template<typename T>
std::ostream& operator << (std::ostream& out, const Polynomial<T>& pol) {
    if (!pol.GetData().size()) {
        out << "0";
        return out;
    }
    if (pol.Degree() == -1) {
        out << "0";
        return out;
    }
    for (int i = pol.Degree(); i >= 0; i--) {
        out << ((pol[i] != T(0)) ? ((pol[i] > T(0)) ? ((i != pol.Degree()) ? "+" : "") : "") : "");
        if (pol[i] != T(0)) {
            if (((pol[i] != T(-1)) && (pol[i] != T(1))) || (i == T(0))) {
                out << pol[i];
                if (i != 0) {
                    out << "*";
                }
            } else {
                if (pol[i] < T(0)) {
                    out << "-";
                }
            }
        }
        if (i != 0 && pol[i] != T(0)) {
            out << "x";
        }
        if (i != 0 && i != 1 && pol[i] != T(0)) {
            out << "^" << i;
        }
        if (pol.Degree() == 0 && pol[0] == T(0)) {
            out << "0";
        }
    }
    return out;
}
