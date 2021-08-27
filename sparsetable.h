#pragma once

#include <vector>
#include <cmath>

template <typename T, typename Comp>
// T is the type of the element to be stored
// Comp is a functor where comp(a, b) should return true if a is better than b
class sparsetable {
public:
    sparsetable(int N, const std::vector<T> &data) : N(N), K(int(floor(log2(N)))), st(K + 1) {
        for (auto &v : st)
            v.resize(N + 1);

        // build sparse table:
        st[0] = data;
        for (int k = 1; k <= K; k++) {
            for (int i = 1; i + (1 << k) - 1 <= N; i++) {
                st[k][i] = func(st[k - 1][i], st[k - 1][i + (1 << (k - 1))]);
            }
        }
    }

    T query(int l, int r) {
        // fast computation of floor(log2(r - l + 1)):
        int k = 31 - __builtin_clz(r - l + 1);
        return func(st[k][l], st[k][r - (1 << k) + 1]);
    }
private:
    const int N, K;
    std::vector<std::vector<T>> st;
    Comp comp;
    inline T func(const T &a, const T &b) {
        return comp(a, b) ? a : b;
    }
};