#pragma once

#include <deque>

template <typename Comp>
// Comp is a functor used to compare elements
// comp(a, b) returns true if b should pop out a at the end of the monoqueue
class monoqueue {
public:
    void enqueue(int x) {
        while (!dq.empty() && comp(dq.back(), x)) dq.pop_back();
        dq.push_back(x);
    }
    inline void dequeue(int x) {
        if (!dq.empty() && dq.front() == x)
            dq.pop_front();
    }
    inline int extremum() { return dq.front(); }
private:
    Comp comp;
    std::deque<int> dq;
};