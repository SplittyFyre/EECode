#include "monoqueue.h"
#include "lbt.h"
#include <iostream>
#include <functional>

struct subarray {
    int l, r;
    subarray() {}
    subarray(int l, int r) : l(l), r(r) {}
    bool operator>(const subarray &b) const {
        const subarray &a = *this;
        int lena = a.r - a.l + 1;
        int lenb = b.r - b.l + 1;
        return (lena > lenb) || ((lena == lenb) && (a.l < b.l));
    }
};

int N, K, Q;
std::vector<int> a;
std::vector<subarray> f, g;

struct compareSubarrays {
    bool operator()(const subarray &a, const subarray &b) const {
        return a > b;
    }
};

int main(void) {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cin >> N >> K;
    a.resize(N + 1); f.resize(N + 1); g.resize(N + 1);
    for (int i = 1; i <= N; i++) std::cin >> a[i];

    // two-pointers method to precompute the functions f and g
    monoqueue<std::less<int>> maxq;
    monoqueue<std::greater<int>> minq;

    int l = 1, r = 1;
    while (r <= N) {
        maxq.enqueue(a[r]);
        minq.enqueue(a[r]);

        while (maxq.extremum() - minq.extremum() > K) {
            f[l] = {l, r - 1};

            maxq.dequeue(a[l]);
            minq.dequeue(a[l]);
            l++;
        }
        g[r] = {l, r};
        r++;
    }

    while (l <= N) {
        f[l] = {l, N};
        l++;
    }

    // range maximum queries on f
    // subarray(0x3f3f3f3f, 0x3f3f3f3f) is a default value which is not greater than any other subarray
    lbt<subarray, compareSubarrays> rmq(N, f, subarray(0x3f3f3f3f, 0x3f3f3f3f));

    std::cin >> Q;
    while (Q--) {
        int l, r; std::cin >> l >> r;
        subarray s = {std::max(l, g[r].l), g[r].r};
        if (s.l == l) { // if s covers the whole query range
            std::cout << l << ' ' << r << '\n';
            continue;
        }
        subarray ans = rmq.query(l, s.l - 1);
        if (s > ans) ans = s;
        std::cout << ans.l << ' ' << ans.r << '\n';
    }

    return 0;
}