#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <sstream>
#include <set>
#include <gtest/gtest.h>

#include "redblacktree.hpp"
#include "Set.hpp"

namespace stl::unittests
{
    template<typename value_type>
    size_t rbtree_black_count(const Node<value_type>* node, const Node<value_type>* root)
    {
        size_t black_count(1);
        while (node != root) {
            if (node->color == Color::Black) {
                black_count++;
            }
            node = node->parent;
        }
        black_count++;
        return black_count;
    }

    enum class Report { WrongHeaderEmpty, WrongHeader, WrongRoot, WrongChild, WrongBlackLen };

    template<typename value_type>
    std::string report(Report status,
                       bool& debug,
                       const Node<value_type>* node = nullptr,
                       const Node<value_type>* child = nullptr)
    {
        std::stringstream sstream;
        if (debug) {
            switch (status) {
            case Report::WrongHeaderEmpty:
                sstream << "Tree is empty but header is wrong, check it!";
                break;
            case Report::WrongHeader:
                sstream << "Check leftmost and rightmost and also links in tree,"
                        << " may be maximum and minimum can't find right values";
                break;
            case Report::WrongRoot:
                sstream << "Root is wrong!";
                break;
            case Report::WrongChild:
                sstream << "Red node " << node->key << " has red child " << child->key;
                break;
            case Report::WrongBlackLen:
                sstream << "Wrong black len from leaf " << node->key << " to root";
                break;
            }
        }
        return sstream.str();
    }

    template<typename value_type>
    bool rbtree_verify(RedBlackTree<value_type>& rb_tree, bool debug = false)
    {
        bool is_correct(true);
        if (!rb_tree.size() || rb_tree.begin() == rb_tree.end()) {
            is_correct = !rb_tree.size() && (rb_tree.begin() == rb_tree.end()) &&
                (rb_tree.leftmost() == rb_tree.end().node) && (rb_tree.rightmost() == rb_tree.end().node);
        }

        EXPECT_TRUE(is_correct) << report<value_type>(Report::WrongHeaderEmpty, debug);

        auto root(rb_tree.root());
        if (root && (root->color == Color::Red || !is_header(root->parent))) {
            is_correct = false;
        }

        EXPECT_TRUE(is_correct) << report<value_type>(Report::WrongRoot, debug);

        if (root) {
            size_t black_len = rbtree_black_count(rb_tree.leftmost(), root);
            for (auto iter(rb_tree.begin()); iter != rb_tree.end(); iter++) {
                auto node(iter.node), lchild(iter.node->lchild), rchild(iter.node->rchild);

                if (node->color == Color::Red) {
                    if ((lchild && lchild->color == Color::Red) ||
                        (rchild && rchild->color == Color::Red)) {
                            is_correct = false;
                        }
                }

                EXPECT_TRUE(is_correct) <<
                    report(Report::WrongChild, debug, node, (lchild->color == Color::Red ? lchild : rchild));

                if (!lchild && !rchild && (rbtree_black_count(node, root) != black_len)) {
                    is_correct = false;
                }

                EXPECT_TRUE(is_correct) << report(Report::WrongBlackLen, debug, node);
            }

            if (rb_tree.minimum(root) != rb_tree.leftmost() || rb_tree.maximum(root) != rb_tree.rightmost()) {
                is_correct = false;
            }

            EXPECT_TRUE(is_correct) << report<value_type>(Report::WrongHeader, debug);
        }

        return true;
    }

    template<typename FirstContainer, typename SecondContainer>
    void check_container_equality(const FirstContainer& set1, const SecondContainer& set2)
    {
        EXPECT_EQ(set1.size(), set2.size());
        auto it1(set1.begin());
        auto it2(set2.begin());
        for (; it1 != set1.end(); it1++, it2++) {
            EXPECT_EQ(*it1, *it2);
        }
    }

    typedef std::chrono::_V2::steady_clock::time_point time_point;

    template<typename function, class... Args>
    int64_t timer(const function& op, Args... args)
    {
        time_point start, finish;
        start = std::chrono::steady_clock::now();
        op(args...);
        finish = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(finish - start).count();
        return duration;
    }

    template<typename function, class... Args>
    double timeit(const function& op, unsigned retry, const Args&... args)
    {
        double duration(0);
        for (unsigned i(0); i < retry; i++) {
            duration += timer(op, args...);
        }
        duration /= retry;
        return duration;
    }

    template<typename setfunc, typename rbtfunc, typename value_type, class... Args>
    void compare_operation_time(const setfunc& set_op,
                                const rbtfunc& rbt_op,
                                unsigned retry,
                                int64_t complete_count,
                                const std::set<value_type>& set,
                                const stl::Set<value_type>& rb_tree,
                                const Args&... args)
    {
        auto duration = timeit(rbt_op, retry, rb_tree, args...) / complete_count;
        std::cout << "\tstl::Set: " << duration << "s" << std::endl;

        duration = timeit(set_op, retry, set, args...) / complete_count;
        std::cout << "\tstd::set: " << duration << "s" << std::endl;
    }

    template<typename container, typename value_type>
    void insert_data(container& set, const std::vector<value_type>& data)
    {
        for (const auto& elem: data) {
            set.insert(elem);
        }
    }

    template<typename container, typename value_type>
    void erase_data(container& set, const std::vector<value_type>& data)
    {
        for (const auto& elem: data) {
            set.erase(elem);
        }
    }

    template<typename container>
    void iterator_inc(const container& set)
    {
        for (auto it(set.begin()); it != set.end(); it++) {}
    }

    template<typename container>
    void iterator_dec(const container& set)
    {
        for (auto it(set.rbegin()); it != set.rend(); it++) {}
    }

    template<typename container, typename value_type>
    void find(const container& set, const std::vector<value_type>& data)
    {
        for (auto& elem: data) {
            set.find(elem);
        }
    }

    template<typename value_type>
    void compare_with_stdset(const std::vector<value_type>& data)
    {
        std::cout << "Time comparison for different operations..." << std::endl;
        unsigned retry(10);
        stl::Set<value_type> rb_tree;
        std::set<value_type> set;

        std::cout << "Insert operation:" << std::endl;
        compare_operation_time(insert_data<std::set<value_type>, value_type>,
                               insert_data<stl::Set<value_type>, value_type>,
                               retry, data.size(), set, rb_tree, data);

        std::cout << "Erase operation:" << std::endl;
        rb_tree = stl::Set<value_type>(data.begin(), data.end());
        set = std::set<value_type>(data.begin(), data.end());
        compare_operation_time(erase_data<std::set<value_type>, value_type>,
                               erase_data<stl::Set<value_type>, value_type>,
                               retry, data.size(), set, rb_tree, data);

        std::cout << "Iterator++ operation:" << std::endl;
        compare_operation_time(iterator_inc<std::set<value_type>>,
                               iterator_inc<stl::Set<value_type>>,
                               retry, set.size(), set, rb_tree);
        std::cout << "Iterator-- operation:" << std::endl;
        compare_operation_time(iterator_dec<std::set<value_type>>,
                               iterator_dec<stl::Set<value_type>>,
                               retry, set.size(), set, rb_tree);

        std::cout << "Find operation:" << std::endl;
        compare_operation_time(find<std::set<value_type>, value_type>,
                               find<stl::Set<value_type>, value_type>,
                               retry, data.size(), set, rb_tree, data);
    }
}
