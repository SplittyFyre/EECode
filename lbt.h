#pragma once

#include "sparsetable.h"
#include <algorithm>

template <typename T, typename Comp>
// Comp is a functor, comp(a, b) should return true if a is better than b
class lbt {
public:
    lbt(int N, const std::vector<T> &v, const T &defval) : N(N), b(int(floor(log2(N)))), data(v), masks(N + 1) {
        // compute masks for all indices
        masks[N] = int16_t(1) << 15;
        for (int i = N - 1; i >= 1; i--) {
            int16_t q = masks[i + 1];
            q >>= 1;
            q &= ~((1 << (16 - (b - 1))) - 1); // clear all except the b-1 most significant bits
            while (q && !comp(v[i + 15 - mssb(q)], v[i])) {
                q &= ~(1 << mssb(q));
            }
            q |= (1 << 15);
            masks[i] = q;
        }
        
        std::vector<T> blocks(ceildiv(N, b) + 1, defval);
        for (int i = 1; i <= N; i++) {
            int bi = ceildiv(i, b);
            blocks[bi] = func(blocks[bi], v[i]);
        }
        broad = new sparsetable<T, Comp>(ceildiv(N, b), blocks);
    }
    ~lbt() { delete broad; }

    T query(int l, int r) {
        if (r - l + 1 < b) return preciseQuery(l, r);

        // query b-1 sized blocks from the ends inwards
        T ans = func(preciseQuery(l, l + (b - 1) - 1), preciseQuery(r - (b - 1) + 1, r));

        int bl = ceildiv(l, b), br = ceildiv(r, b);
        if (l != (bl - 1) * b + 1) bl++;
        if (r != std::min(N, br * b)) br--;
        if (bl <= br) {
            ans = func(ans, broad->query(bl, br));
        }

        return ans;
    }

private:
    const int N, b;
    std::vector<T> data;
    std::vector<int16_t> masks;
    sparsetable<T, Comp> *broad;
    Comp comp;
    inline T func(const T &a, const T &b) {
        return comp(a, b) ? a : b;
    }

    // compute the index of the most significant set bit of mask
    inline int mssb(int16_t mask) {
        return 31 - __builtin_clz(uint16_t(mask));
    }
    // compute the index of the least significant set bit of mask
    inline int lssb(int16_t mask) {
        return mssb(mask & -mask);
    }
    // compute ceil(x / y)
    inline int ceildiv(int x, int y) {
        return (x - 1) / y + 1;
    }

    T preciseQuery(int l, int r) {
        int16_t mask = masks[l];
        mask &= ~((1 << (15 - r + l)) - 1);
        int idx = lssb(mask);
        return data[l + (15 - idx)];
    }
};