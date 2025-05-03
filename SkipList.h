//
// Created by l4zy9uy on 01/05/2025.
//

#ifndef SKIPLIST_SKIPLIST_H
#define SKIPLIST_SKIPLIST_H

#include <vector>
#include <random>

template<typename T, typename Compare = std::less<T>>
class SkipList {
public:
    struct Node {
        T value;
        std::vector<Node *> next;

        Node(const T &v, int h) : value(v), next(h, nullptr) {}
    };

    explicit SkipList(const Compare &compare = Compare(),
                      int maxLevel = kMaxLevel,
                      float probability = kDefaultProbability);

    explicit SkipList(const std::vector<T> &v);

    ~SkipList();

    SkipList(const SkipList &) = delete;

    SkipList &operator=(const SkipList &) = delete;

    [[nodiscard]] bool empty() const;

    Node *lower_bound(const T &);

    Node *upper_bound(const T &);

    void insert(const T &);

    bool erase(const T &);

    Node *begin() const;

    Node *end() const;

private:
    static constexpr int kMaxLevel = 12;
    static constexpr float kDefaultProbability = 0.5f;

    int random_level() {
        int lvl = 1;
        while (distribution_(generator_) < probability_ && lvl < maxLevel_) {
            ++lvl;
        }
        return lvl;
    }

    Compare compare_;
    const int maxLevel_;
    const float probability_;
    int currentLevel_;
    Node *head_;

    std::mt19937 generator_;
    std::uniform_real_distribution<float> distribution_;
};


#endif //SKIPLIST_SKIPLIST_H
