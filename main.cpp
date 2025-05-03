#include <iostream>
#include <cassert>
#include <set>
#include "SkipList.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    const int N = 100000;   // number of inserts
    const int M = 50000;    // number of erases
    const int MAXV = 10000; // value range [0..MAXV)

    // prepare RNG
    std::mt19937_64 rng(12345);
    std::uniform_int_distribution<int> dist(0, MAXV - 1);

    SkipList<int> sl;
    std::set<int> ref;

    std::vector<int> data;
    data.reserve(N);
    for (int i = 0; i < N; ++i) {
        int v = dist(rng);
        data.push_back(v);
        sl.insert(v);
        ref.insert(v);
    }

    // 1) Verify after insert
    {
        auto it_sl = sl.begin();
        auto it_ref = ref.begin();
        while (it_ref != ref.end()) {
            assert(it_sl && "SkipList ended too early");
            if (it_sl->value != *it_ref) {
                std::cerr << "Mismatch after insert: skip="
                          << it_sl->value << " vs ref=" << *it_ref << "\n";
                return 1;
            }
            it_sl = it_sl->next[0];
            ++it_ref;
        }
        if (it_sl) {
            std::cerr << "SkipList has extra elements after insert\n";
            return 1;
        }
        std::cout << "[OK] Insert consistency check passed\n";
    }

    // 2) Random erasures
    for (int i = 0; i < M; ++i) {
        int v = data[rng() % N];
        sl.erase(v);
        auto it = ref.find(v);
        if (it != ref.end()) ref.erase(it);
    }

    // 3) Verify after erase
    {
        auto it_sl = sl.begin();
        auto it_ref = ref.begin();
        while (it_ref != ref.end()) {
            assert(it_sl && "SkipList ended too early after erase");
            if (it_sl->value != *it_ref) {
                std::cerr << "Mismatch after erase: skip="
                          << it_sl->value << " vs ref=" << *it_ref << "\n";
                return 1;
            }
            it_sl = it_sl->next[0];
            ++it_ref;
        }
        if (it_sl) {
            std::cerr << "SkipList has extra elements after erase\n";
            return 1;
        }
        std::cout << "[OK] Erase consistency check passed\n";
    }

    std::cout << "All tests passed!\n";
    return 0;
}