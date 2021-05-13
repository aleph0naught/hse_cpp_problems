#include <iostream>
#include <vector>
#include <cmath>
#include <execution>
#include <random>

using namespace std;

std::vector<long long> a, t;

long long f(long long x) {
    long long p = 1;
    while (1) {
        if (p >= x) {
            return p;
        }
        p <<= 1;
    }
    return 0;
}

struct Request {
    int left;
    int right;
};

long long compute(long long v, long long ql, long long qr, long long il, long long ir) {
    if (ir <= ql || qr <= il) {
        return 1e9;
    }
    if (ql <= il && ir <= qr) {
        return t[v];
    }
    long long im = (il + ir) / 2;
    return min(compute(2 * v + 1, ql, qr, il, im), compute(2 * v + 2, ql, qr, im, ir));
}

vector<int> ProcessRequests(const vector<int>& numbers, const vector<Request>& requests) {
    long long n;
    vector<int> a(numbers);
    for (long long i = numbers.size(); i < f(numbers.size()); i++) {
        a.push_back(1e9);
    }
    n = f(numbers.size());
    t.resize(2 * n + 1, 1e9);
    for (long long i = n - 1; i < 2 * n - 1; i++) {
        t[i] = a[i - n + 1];
    }
    for (long long i = n - 2; i >= 0; i--) {
        t[i] = min(t[2 * i + 1], t[2 * i + 2]);
    }

    vector<int> res;
    res.resize(requests.size());

    transform(
        execution::par,
        requests.begin(), requests.end(),
        res.begin(),
        [n](Request t) -> long long {
            return compute(0, t.left , t.right, 0, n);
        });
    return res;
}
