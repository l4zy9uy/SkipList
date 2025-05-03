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

    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;

        explicit Iterator(Node *node = nullptr) : node_(node) {}

        reference operator*() const { return node_->value; }

        pointer operator->() const { return &(node_->value); }

        Iterator &operator++() {
            node_ = node_->next[0];
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++*this;
            return tmp;
        }

        friend bool operator==(const Iterator &a, const Iterator &b) {
            return a.node_ == b.node_;
        }

        friend bool operator!=(const Iterator &a, const Iterator &b) {
            return a.node_ != b.node_;
        }

    private:
        Node *node_;
    };

    class ConstIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T *;
        using reference = const T &;

        explicit ConstIterator(const Node *node = nullptr) : node_(node) {}

        reference operator*() const { return node_->value; }

        pointer operator->() const { return &(node_->value); }

        ConstIterator &operator++() {
            node_ = node_->next[0];
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator tmp = *this;
            ++*this;
            return tmp;
        }

        friend bool operator==(const ConstIterator &a, const ConstIterator &b) {
            return a.node_ == b.node_;
        }

        friend bool operator!=(const ConstIterator &a, const ConstIterator &b) {
            return a.node_ != b.node_;
        }

    private:
        const Node *node_;
    };

    using iterator = Iterator;
    using const_iterator = ConstIterator;

    explicit SkipList(const Compare &compare = Compare(),
                      int maxLevel = kMaxLevel,
                      float probability = kDefaultProbability);

    explicit SkipList(const std::vector<T> &v);

    ~SkipList();

    SkipList(const SkipList &) = delete;

    SkipList &operator=(const SkipList &) = delete;

    [[nodiscard]] bool empty() const;

    iterator lower_bound(const T &);

    iterator upper_bound(const T &);

    const_iterator lower_bound(const T &) const;

    const_iterator upper_bound(const T &) const;

    void insert(const T &);

    bool erase(const T &);

    iterator begin() { return iterator(head_->next[0]); }

    iterator end() { return iterator(nullptr); }

    const_iterator begin() const { return const_iterator(head_->next[0]); }

    const_iterator end() const { return const_iterator(nullptr); }

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

template<typename T, typename Compare>
SkipList<T, Compare>::SkipList(const std::vector<T> &v) : compare_(),
                                                          maxLevel_(kMaxLevel),
                                                          probability_(kDefaultProbability),
                                                          currentLevel_(1),
                                                          head_(new Node(T(), kMaxLevel)),
                                                          generator_(std::random_device{}()),
                                                          distribution_(0.0f, 1.0f) {
    for (auto &i: v) {
        insert(i);
    }
}

template<typename T, typename Compare>
SkipList<T, Compare>::~SkipList() {
    Node *node = head_;
    while (node) {
        Node *nxt = node->next[0];
        delete node;
        node = nxt;
    }
}

template<typename T, typename Compare>
SkipList<T, Compare>::SkipList(const Compare &compare,
                               int maxLevel,
                               float probability)
        : compare_(compare), maxLevel_(maxLevel), probability_(probability), currentLevel_(1),
          head_(new Node(T(), maxLevel_)), generator_(std::random_device{}()), distribution_(0.0f, 1.0f) {}


template<typename T, typename Compare>
bool SkipList<T, Compare>::empty() const {
    return head_->next[0] == nullptr;
}

template<typename T, typename Compare>
void SkipList<T, Compare>::insert(const T &value) {
    std::vector<Node *> update(maxLevel_, nullptr);
    Node *x = head_;

    // Find insertion points
    for (int i = currentLevel_ - 1; i >= 0; --i) {
        while (x->next[i] && compare_(x->next[i]->value, value)) {
            x = x->next[i];
        }
        update[i] = x;
    }
    x = x->next[0];

    if (x && !compare_(value, x->value) && !compare_(x->value, value)) {
        return;
    }

    // Randomly choose node height
    int lvl = random_level();
    if (lvl > currentLevel_) {
        for (int i = currentLevel_; i < lvl; ++i) {
            update[i] = head_;
        }
        currentLevel_ = lvl;
    }

    // Allocate and splice in
    Node *n = new Node(value, lvl);
    for (int i = 0; i < lvl; ++i) {
        n->next[i] = update[i]->next[i];
        update[i]->next[i] = n;
    }
}

template<typename T, typename Compare>
bool SkipList<T, Compare>::erase(const T &value) {
    std::vector<Node *> update(maxLevel_, nullptr);
    Node *x = head_;

    // Find nodes just before the target
    for (int i = currentLevel_ - 1; i >= 0; --i) {
        while (x->next[i] && compare_(x->next[i]->value, value)) {
            x = x->next[i];
        }
        update[i] = x;
    }
    x = x->next[0];
    if (!x || compare_(value, x->value) || compare_(x->value, value)) {
        return false;
    }

    // Unlink at each level
    for (int i = 0; i < currentLevel_; ++i) {
        if (update[i]->next[i] != x) break;
        update[i]->next[i] = x->next[i];
    }
    delete x;

    // 3) Decrease currentLevel_ if necessary
    while (currentLevel_ > 1 && head_->next[currentLevel_ - 1] == nullptr) {
        --currentLevel_;
    }
    return true;
}

template<typename T, typename Compare>
SkipList<T, Compare>::iterator SkipList<T, Compare>::lower_bound(const T &key) {
    Node *x = head_;
    for (int i = currentLevel_ - 1; i >= 0; --i) {
        while (x->next[i] && compare_(x->next[i]->value, key)) {
            x = x->next[i];
        }
    }
    return iterator(x->next[0]);
}

template<typename T, typename Compare>
SkipList<T, Compare>::iterator SkipList<T, Compare>::upper_bound(const T &key) {
    Node *x = head_;
    for (int i = currentLevel_ - 1; i >= 0; --i) {
        while (x->next[i] && !compare_(key, x->next[i]->value)) {
            x = x->next[i];
        }
    }
    return iterator(x->next[0]);
}


template<typename T, typename Compare>
SkipList<T, Compare>::const_iterator SkipList<T, Compare>::lower_bound(const T &key) const {
    const Node *x = head_;
    for (int i = currentLevel_ - 1; i >= 0; --i) {
        while (x->next[i] && compare_(x->next[i]->value, key)) {
            x = x->next[i];
        }
    }
    return const_iterator(x->next[0]);
}

template<typename T, typename Compare>
SkipList<T, Compare>::const_iterator SkipList<T, Compare>::upper_bound(const T &key) const {
    const Node *x = head_;
    for (int i = currentLevel_ - 1; i >= 0; --i) {
        while (x->next[i] && !compare_(key, x->next[i]->value)) {
            x = x->next[i];
        }
    }
    return const_iterator(x->next[0]);
}

#endif //SKIPLIST_SKIPLIST_H
