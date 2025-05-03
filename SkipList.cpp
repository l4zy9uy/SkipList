////
//// Created by l4zy9uy on 01/05/2025.
////
//
//#include "SkipList.h"
//
//template
//class SkipList<int, std::less<int>>;
//
//template<typename T, typename Compare>
//SkipList<T, Compare>::SkipList(const std::vector<T> &v) : compare_(),
//                                                          maxLevel_(kMaxLevel),
//                                                          probability_(kDefaultProbability),
//                                                          currentLevel_(1),
//                                                          head_(new Node(T(), kMaxLevel)),
//                                                          generator_(std::random_device{}()),
//                                                          distribution_(0.0f, 1.0f) {
//    for (auto &i: v) {
//        insert(i);
//    }
//}
//
//template<typename T, typename Compare>
//SkipList<T, Compare>::~SkipList() {
//    Node *node = head_;
//    while (node) {
//        Node *nxt = node->next[0];
//        delete node;
//        node = nxt;
//    }
//}
//
//template<typename T, typename Compare>
//SkipList<T, Compare>::SkipList(const Compare &compare,
//                               int maxLevel,
//                               float probability)
//        : compare_(compare), maxLevel_(maxLevel), probability_(probability), currentLevel_(1),
//          head_(new Node(T(), maxLevel_)), generator_(std::random_device{}()), distribution_(0.0f, 1.0f) {}
//
//
//template<typename T, typename Compare>
//bool SkipList<T, Compare>::empty() const {
//    return head_->next[0] == nullptr;
//}
//
//template<typename T, typename Compare>
//void SkipList<T, Compare>::insert(const T &value) {
//    std::vector<Node *> update(maxLevel_, nullptr);
//    Node *x = head_;
//
//    // Find insertion points
//    for (int i = currentLevel_ - 1; i >= 0; --i) {
//        while (x->next[i] && compare_(x->next[i]->value, value)) {
//            x = x->next[i];
//        }
//        update[i] = x;
//    }
//    x = x->next[0];
//
//    if (x && !compare_(value, x->value) && !compare_(x->value, value)) {
//        return;
//    }
//
//    // Randomly choose node height
//    int lvl = random_level();
//    if (lvl > currentLevel_) {
//        for (int i = currentLevel_; i < lvl; ++i) {
//            update[i] = head_;
//        }
//        currentLevel_ = lvl;
//    }
//
//    // Allocate and splice in
//    Node *n = new Node(value, lvl);
//    for (int i = 0; i < lvl; ++i) {
//        n->next[i] = update[i]->next[i];
//        update[i]->next[i] = n;
//    }
//}
//
//template<typename T, typename Compare>
//bool SkipList<T, Compare>::erase(const T &value) {
//    std::vector<Node *> update(maxLevel_, nullptr);
//    Node *x = head_;
//
//    // Find nodes just before the target
//    for (int i = currentLevel_ - 1; i >= 0; --i) {
//        while (x->next[i] && compare_(x->next[i]->value, value)) {
//            x = x->next[i];
//        }
//        update[i] = x;
//    }
//    x = x->next[0];
//    if (!x || compare_(value, x->value) || compare_(x->value, value)) {
//        return false;
//    }
//
//    // Unlink at each level
//    for (int i = 0; i < currentLevel_; ++i) {
//        if (update[i]->next[i] != x) break;
//        update[i]->next[i] = x->next[i];
//    }
//    delete x;
//
//    // 3) Decrease currentLevel_ if necessary
//    while (currentLevel_ > 1 && head_->next[currentLevel_ - 1] == nullptr) {
//        --currentLevel_;
//    }
//    return true;
//}
//
//template<typename T, typename Compare>
//SkipList<T, Compare>::iterator SkipList<T, Compare>::lower_bound(const T &key) {
//    Node *x = head_;
//    for (int i = currentLevel_ - 1; i >= 0; --i) {
//        while (x->next[i] && compare_(x->next[i]->value, key)) {
//            x = x->next[i];
//        }
//    }
//    return iterator(x->next[0]);
//}
//
//template<typename T, typename Compare>
//SkipList<T, Compare>::iterator SkipList<T, Compare>::upper_bound(const T &key) {
//    Node *x = head_;
//    for (int i = currentLevel_ - 1; i >= 0; --i) {
//        while (x->next[i] && !compare_(key, x->next[i]->value)) {
//            x = x->next[i];
//        }
//    }
//    return iterator(x->next[0]);
//}
//
//
//template<typename T, typename Compare>
//SkipList<T, Compare>::const_iterator SkipList<T, Compare>::lower_bound(const T &key) const {
//    const Node *x = head_;
//    for (int i = currentLevel_ - 1; i >= 0; --i) {
//        while (x->next[i] && compare_(x->next[i]->value, key)) {
//            x = x->next[i];
//        }
//    }
//    return const_iterator(x->next[0]);
//}
//
//template<typename T, typename Compare>
//SkipList<T, Compare>::const_iterator SkipList<T, Compare>::upper_bound(const T &key) const {
//    const Node *x = head_;
//    for (int i = currentLevel_ - 1; i >= 0; --i) {
//        while (x->next[i] && !compare_(key, x->next[i]->value)) {
//            x = x->next[i];
//        }
//    }
//    return const_iterator(x->next[0]);
//}