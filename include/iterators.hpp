#pragma once

#include "base_entities.hpp"

namespace stl
{
    template<typename Tp>
    struct RedBlackTree_const_iterator
    {
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef std::ptrdiff_t difference_type;

        typedef Tp value_type;
        typedef const Tp* pointer;
        typedef const Tp& reference;

        typedef Node<Tp>* _Base_ptr;
        typedef RedBlackTree_const_iterator<Tp> _Self;

        _Base_ptr node;

        RedBlackTree_const_iterator() : node() {}
        explicit RedBlackTree_const_iterator(const _Base_ptr other) : node(other) {}

        reference operator*() const
        {
            return node->key;
        }

        pointer operator->() const
        {
            return &(node->key);
        }

        _Self operator++()
        {
            node = node->nextNode();
            return *this;
        }

        _Self operator++(int)
        {
            _Self tmp = *this;
            node = node->nextNode();
            return tmp;
        }

        _Self operator--()
        {
            node = node->prevNode();
            return *this;
        }

        _Self operator--(int)
        {
            _Self tmp = *this;
            node = node->prevNode();
            return tmp;
        }

        friend bool operator==(const _Self& left, const _Self& right)
        {
            return left.node == right.node;
        }

        friend bool operator!=(const _Self& left, const _Self& right)
        {
            return left.node != right.node;
        }
    };
}
