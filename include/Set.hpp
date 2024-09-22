#pragma once

#include <initializer_list>
#include "redblacktree.hpp"

namespace stl
{
    template <typename TKey>
    class Set
    {
        RedBlackTree<TKey> rb_tree_;

     public:
        typedef TKey value_type;
        typedef typename RedBlackTree<value_type>::iterator iterator;
        typedef typename RedBlackTree<value_type>::reverse_iterator reverse_iterator;

        Set() = default;

        template <class _InputIterator>
        Set(_InputIterator first, _InputIterator last) : rb_tree_(first, last) { }

        explicit Set(const std::initializer_list<value_type>& l) : rb_tree_(l) { }

        Set(const Set& other) = default;

        ~Set() { }

        Set<value_type>& operator=(const Set<value_type>& other) = default;

        void clear()
        {
            rb_tree_.clear();
        }

        void insert(const value_type& val)
        {
            rb_tree_.insert(val);
        }

        void erase(const value_type& val)
        {
            rb_tree_.erase(val);
        }

        iterator begin() const
        {
            return rb_tree_.begin();
        }

        iterator end() const
        {
            return rb_tree_.end();
        }

        reverse_iterator rbegin() const
        {
            return rb_tree_.rbegin();
        }

        reverse_iterator rend() const
        {
            return rb_tree_.rend();
        }

        iterator find(const value_type& val) const
        {
            return rb_tree_.find(val);
        }

        iterator lower_bound(const value_type& val) const
        {
            return rb_tree_.lower_bound(val);
        }

        iterator upper_bound(const value_type& val) const
        {
            return rb_tree_.upper_bound(val);
        }

        size_t size() const
        {
            return rb_tree_.size();
        }

        bool empty() const
        {
            return rb_tree_.empty();
        }
    };
}
