//
// Created by l4zy9uy on 03/05/2025.
//
// test_skiplist.cpp
// Catch2 test suite for SkipList<T,Compare>

#include <catch2/catch_test_macros.hpp>
#include "SkipList.h"
#include <vector>
#include <algorithm>
#include <string>

TEST_CASE("Default-constructed skip list is empty", "[skiplist][empty]") {
    SkipList<int> sl;
    REQUIRE(sl.empty());
    REQUIRE(sl.begin() == sl.end());
}

TEST_CASE("Insert and Contains", "[skiplist][insert][contains]") {
    SkipList<int> sl;
    REQUIRE(sl.empty());
    sl.insert(10);
    REQUIRE(!sl.empty());

    REQUIRE(sl.lower_bound(10) != sl.end());
    REQUIRE(*sl.lower_bound(10) == 10);
    REQUIRE(sl.lower_bound(5) == sl.begin()); // 10 is first >=5

    // Duplicates ignored
    sl.insert(10);
    size_t count = 0;
    for (auto it = sl.begin(); it != sl.end(); ++it) ++count;
    REQUIRE(count == 1);
}

TEST_CASE("Erase existing and non-existing values", "[skiplist][erase]") {
    SkipList<int> sl;
    std::vector<int> vals = {1,2,3,4,5};
    for (auto v: vals) sl.insert(v);
    REQUIRE(!sl.empty());
    // Erase middle
    REQUIRE(sl.erase(3));
    REQUIRE(sl.lower_bound(3) != sl.end());
    REQUIRE(*sl.lower_bound(3) == 4);
    // Erase head
    REQUIRE(sl.erase(1));
    REQUIRE(*sl.begin() == 2);
    // Erase tail
    REQUIRE(sl.erase(5));
    auto it = sl.begin();
    std::advance(it, 2);
    REQUIRE(it == sl.end()); // only 2,4 remain
    // Erase non-existent
    REQUIRE(!sl.erase(42));
}

TEST_CASE("Iteration yields sorted sequence", "[skiplist][iteration]") {
    SkipList<int> sl;
    std::vector<int> data{5,1,4,3,2};
    for (auto v: data) sl.insert(v);
    std::sort(data.begin(), data.end());
    std::vector<int> seq;
    for (auto v : sl) seq.push_back(v);
    REQUIRE(seq == data);
}

TEST_CASE("lower_bound and upper_bound semantics", "[skiplist][bounds]") {
    SkipList<int> sl;
    for (int i = 0; i < 10; i += 2) sl.insert(i);
    // Elements: 0,2,4,6,8
    auto lb = sl.lower_bound(5);
    REQUIRE(lb != sl.end());
    REQUIRE(*lb == 6);
    auto ub = sl.upper_bound(6);
    REQUIRE(ub != sl.end());
    REQUIRE(*ub == 8);
    // Out-of-range
    REQUIRE(sl.lower_bound(10) == sl.end());
    REQUIRE(sl.upper_bound(10) == sl.end());
}

struct Desc { bool operator()(int a, int b) const { return a > b; } };
TEST_CASE("Custom comparator (descending order)", "[skiplist][comparator]") {
    SkipList<int, Desc> sl(Desc{});
    sl.insert(1);
    sl.insert(3);
    sl.insert(2);
    std::vector<int> seq;
    for (auto v : sl) seq.push_back(v);
    REQUIRE(seq == std::vector<int>{3,2,1});
}

TEST_CASE("SkipList works with non-trivial type and comparator", "[skiplist][string]") {
    SkipList<std::string> sl;
    sl.insert("apple");
    sl.insert("banana");
    sl.insert("apricot");
    auto lb = sl.lower_bound("ap");  // first >= "ap"
    REQUIRE(lb != sl.end());
    REQUIRE(*lb == "apple");
    auto ub = sl.upper_bound("apple");
    REQUIRE(ub != sl.end());
    REQUIRE(*ub == "apricot");
}

TEST_CASE("STL algorithm compatibility", "[skiplist][algorithm]") {
    SkipList<int> sl;
    for (int i = 1; i <= 5; ++i) sl.insert(i);

    // std::find
    auto it = std::find(sl.begin(), sl.end(), 3);
    REQUIRE(it != sl.end());
    REQUIRE(*it == 3);

    // std::count
    REQUIRE(std::count(sl.begin(), sl.end(), 2) == 1);
    REQUIRE(std::count(sl.begin(), sl.end(), 10) == 0);

    // std::is_sorted
    REQUIRE(std::is_sorted(sl.begin(), sl.end()));

    // std::distance
    REQUIRE(std::distance(sl.begin(), sl.end()) == 5);

    // std::advance
    auto it2 = sl.begin();
    std::advance(it2, 4);
    REQUIRE(it2 != sl.end());
    REQUIRE(*it2 == 5);

    // std::copy
    std::vector<int> v;
    std::copy(sl.begin(), sl.end(), std::back_inserter(v));
    REQUIRE(v == std::vector<int>{1,2,3,4,5});

    // std::accumulate
    REQUIRE(std::accumulate(sl.begin(), sl.end(), 0) == 15);

    // std::any_of / std::none_of
    REQUIRE(std::any_of(sl.begin(), sl.end(), [](int x){ return x % 2 == 0; }));
    REQUIRE(std::none_of(sl.begin(), sl.end(), [](int x){ return x > 5; }));

    // std::mismatch
    auto mm = std::mismatch(sl.begin(), sl.end(), v.begin());
    REQUIRE(mm.first == sl.end());
    REQUIRE(mm.second == v.end());
}