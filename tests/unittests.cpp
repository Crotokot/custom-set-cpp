#include "base_tests.h"
#include "tests.hpp"
#include "helpers.h"


namespace stl::unittests
{
    TEST(StlRedBlackTree, StructureVerify) {
        auto data = datagen::make_random_int_data(100, -200, 200);
        RedBlackTree<int> rb_tree(data.begin(), data.end());
        rbtree_verify(rb_tree);
        RedBlackTree<std::string> srbt{"a", "b", "c", "d", "t", "f", "g"};
        rbtree_verify(rb_tree);
    }

    TEST(StlRedBlackTree, CheckInsert) {
        auto data = datagen::make_random_int_data(100, -100, 100);
        RedBlackTree<int> rb_tree(data.begin(), data.end());
        rbtree_verify(rb_tree);
        std::mt19937 rng(42);
        std::uniform_int_distribution<int> uid(-200, 200);
        for (auto i(0); i < 200; i++) {
            rb_tree.insert(uid(rng));
            rbtree_verify(rb_tree);
        }
    }

    TEST(StlRedBlackTree, CheckErase) {
        std::initializer_list<char> list
            {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'g'};
        RedBlackTree<char> rb_tree(list);
        rbtree_verify(rb_tree);
        rb_tree.erase('b');
        rbtree_verify(rb_tree);
        for (auto& elem: list) {
            rb_tree.erase(elem);
            rbtree_verify(rb_tree);
        }
    }

    TEST(StlSet, CheckBase) {
        internal_tests::run_all();
    }

    TEST(StlSet, CheckEmpty) {
        stl::Set<int> set;
        EXPECT_TRUE(set.empty()) << "Set is not empty";
        EXPECT_EQ(set.size(), 0) << "Set size is not equal to zero";
        EXPECT_EQ(set.begin(), set.end()) << "begin iterator is not equal to end iterator";
        EXPECT_EQ(set.rbegin(), set.rend()) << "rbegin iterator is not equal to rend iterator";
    }

    TEST(StlSet, CheckBeginEndIteratorConstructor) {
        auto data = datagen::make_random_char_data(100);
        stl::Set<char> stlset(data.begin(), data.end());
        std::set<char> set(data.begin(), data.end());
        check_container_equality(set, stlset);
    }

    TEST(StlSet, CheckInitializerListConstructor) {
        std::initializer_list<unsigned> list{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        stl::Set<unsigned> stlset(list);
        std::set<unsigned> set(list);
        check_container_equality(set, stlset);
    }

    TEST(StlSet, CheckCopyCorrectness) {
        auto data = datagen::make_random_char_data(100);
        stl::Set<char> s1(data.begin(), data.end());
        auto s2(s1);
        check_container_equality(s1, s2);

        auto size = s2.size();
        s1.erase(*s1.begin());
        EXPECT_EQ(s2.size(), size);
    }

    TEST(StlSet, CheckAssigment) {
        auto data = datagen::make_random_char_data(100);
        stl::Set<char> s1(data.begin(), data.end());
        auto s2 = s1;
        check_container_equality(s1, s2);

        auto size = s2.size();
        s1.erase(*s1.begin());
        EXPECT_EQ(s2.size(), size);

        s1 = s2 = s1;
        check_container_equality(s1, s2);
    }

    TEST(StlSet, CheckInsert) {
        std::initializer_list<unsigned> list{4, 8, 15, 16, 23, 42};
        std::set<unsigned> set(list);
        stl::Set<unsigned> stlset(list);
        check_container_equality(set, stlset);

        auto data = datagen::make_random_int_data(100, 0, 100);
        for (auto& val: data) {
            set.insert(val);
            stlset.insert(val);
            check_container_equality(set, stlset);
        }
    }

    TEST(StlSet, CheckEraseIntData) {
        int nb_values(1000);
        auto data = datagen::make_random_int_data(nb_values, 0, nb_values);
        std::set<unsigned> set(data.begin(), data.end());
        stl::Set<unsigned> stlset(data.begin(), data.end());
        check_container_equality(set, stlset);

        std::shuffle(data.begin(), data.end(), std::default_random_engine());
        for (auto& val: data) {
            set.erase(val);
            stlset.erase(val);
            check_container_equality(set, stlset);
        }
    }

    TEST(StlSet, CheckEraseStringData) {
        int nb_values(1000);
        auto data = datagen::make_random_string_data(nb_values);
        std::set<std::string> set(data.begin(), data.end());
        stl::Set<std::string> stlset(data.begin(), data.end());
        check_container_equality(set, stlset);

        std::shuffle(data.begin(), data.end(), std::default_random_engine());
        for (auto& val: data) {
            set.erase(val);
            stlset.erase(val);
            check_container_equality(set, stlset);
        }
    }

    TEST(StlSet, CheckFind) {
        int nb_values(10000);
        auto data = datagen::make_random_int_data(nb_values, -nb_values, nb_values);
        std::set<unsigned> set(data.begin(), data.end());
        stl::Set<unsigned> stlset(data.begin(), data.end());
        check_container_equality(set, stlset);

        std::shuffle(data.begin(), data.end(), std::default_random_engine());  // to find in another order
        for (auto& val: data) {
            EXPECT_EQ(*set.find(val), *stlset.find(val));
        }
        EXPECT_EQ(stlset.find(nb_values * 2), stlset.end()) << "Is not equal to end()-iterator";
    }

    TEST(StlSet, CheckLowerBoundIntData) {
        auto data = datagen::make_random_int_data(500, -500, 500);
        std::set<int> set(data.begin(), data.end());
        stl::Set<int> stlset(data.begin(), data.end());
        check_container_equality(set, stlset);

        for (auto& elem: data) {
            auto val = rand() % 2 ? elem : elem - 1;
            EXPECT_EQ(*set.lower_bound(val), *stlset.lower_bound(val));
        }
        EXPECT_EQ(stlset.lower_bound(1000), stlset.end()) << "Is not equal to end()-iterator";
    }

    TEST(StlSet, CheckLowerBoundStringData) {
        auto data = datagen::make_random_string_data(100);
        std::set<std::string> set(data.begin(), data.end());
        stl::Set<std::string> stlset(data.begin(), data.end());
        check_container_equality(set, stlset);

        for (auto& val: data) {
            EXPECT_EQ(*set.lower_bound(val), *stlset.lower_bound(val));
        }
        auto ghost_val = *std::max_element(stlset.begin(), stlset.end()) + "a";
        EXPECT_EQ(stlset.lower_bound(ghost_val), stlset.end()) << "Is not equal to end()-iterator";
    }

    TEST(StlSet, CheckUpperBoundIntData) {
        auto data = datagen::make_random_int_data(500, -500, 500);
        std::set<int> set(data.begin(), data.end());
        stl::Set<int> stlset(data.begin(), data.end());
        check_container_equality(set, stlset);

        for (auto& elem: data) {
            auto val = rand() % 2 ? elem : elem - 1;
            EXPECT_EQ(*set.upper_bound(val), *stlset.upper_bound(val));
        }
        EXPECT_EQ(stlset.upper_bound(1000), stlset.end()) << "Is not equal to end()-iterator";
    }

    TEST(StlSet, CheckUpperBoundCharData) {
        auto data = datagen::make_random_char_data(100);
        std::set<char> set(data.begin(), data.end());
        stl::Set<char> stlset(data.begin(), data.end());
        check_container_equality(set, stlset);

        for (auto& elem: data) {
            auto val = elem ? elem - 1 : elem;
            EXPECT_EQ(*set.upper_bound(val), *stlset.upper_bound(val));
        }
        auto ghost_val = *std::max_element(stlset.begin(), stlset.end()) + 1;
        EXPECT_EQ(stlset.upper_bound(ghost_val), stlset.end()) << "Is not equal to end()-iterator";
    }

    TEST(StlSet, CheckClear) {
        int nb_values(1000);
        auto data = datagen::make_random_string_data(nb_values);
        stl::Set<std::string> stlset(data.begin(), data.end());
        stlset.clear();
        EXPECT_TRUE(stlset.empty()) << "Set is not empty";
        EXPECT_EQ(stlset.size(), 0) << "Set size is not equal to zero";
        EXPECT_EQ(stlset.begin(), stlset.end()) << "begin iterator is not equal to end iterator";
        EXPECT_EQ(stlset.rbegin(), stlset.rend()) << "rbegin iterator is not equal to rend iterator";
    }

    TEST(StlSet, CompareTime) {
        int nb_values(1000000);
        auto data = datagen::make_random_int_data(nb_values, 0, nb_values);
        compare_with_stdset(data);
    }
}
