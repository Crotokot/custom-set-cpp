#pragma once

#include <iterator>
#include <initializer_list>

#include "base_entities.hpp"
#include "iterators.hpp"

namespace stl
{
    ///////////////////////////////////////////////////////////////////////////////
    /// Template class RedBlackTree
    ///////////////////////////////////////////////////////////////////////////////

    template <typename TKey>
    class RedBlackTree
    {
     public:
        typedef TKey value_type;
        typedef RedBlackTree_const_iterator<value_type> iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef Node<TKey> _Base;
        typedef Node<TKey>* _Base_ptr;

     public:
        RedBlackTree();
        RedBlackTree(const RedBlackTree<value_type>& other);

        template <class _InputIterator>
        RedBlackTree(_InputIterator first, _InputIterator last);

        explicit RedBlackTree(const std::initializer_list<value_type>& l);

        ~RedBlackTree();

        void clear();
        inline void drop(_Base_ptr node);
        void insert(const value_type& val);
        void erase(const value_type& val);

        std::pair<_Base_ptr, bool> contains(const value_type& key) const;
        iterator find(const value_type& val) const;
        iterator lower_bound(const value_type& val) const;
        iterator upper_bound(const value_type& val) const;
        iterator begin() const;
        iterator end() const;
        reverse_iterator rbegin() const;
        reverse_iterator rend() const;
        size_t size() const;
        bool empty() const;
        _Base_ptr root() const;
        _Base_ptr leftmost() const;
        _Base_ptr rightmost() const;

        static _Base_ptr minimum(_Base_ptr node);
        static _Base_ptr maximum(_Base_ptr node);

        RedBlackTree<value_type>& operator=(const RedBlackTree<value_type>& other);

     private:
        RedBlackTreeHeader<value_type> header_;

     private:
        class Balancer
        {
            static _Base_ptr lrotate(_Base_ptr node);
            static _Base_ptr rrotate(_Base_ptr node);
            static inline bool is_black(_Base_ptr node);

            static inline void relink_parent(_Base_ptr& parent,
                                             _Base_ptr& prev,
                                             _Base_ptr& curr,
                                             bool prev_is_root = false);

            static void swap(_Base_ptr& node, _Base_ptr& other);

         public:
            static _Base_ptr insert_and_rebalance(_Base_ptr node,
                                                  _Base_ptr parent,
                                                  _Base& header_data);

            static void erase_and_rebalance(_Base_ptr node, _Base& header_data);
        };
    };


    ///////////////////////////////////////////////////////////////////////////////
    /// Implementation of template class RedBlackTree
    ///////////////////////////////////////////////////////////////////////////////

    template <typename TKey>
    RedBlackTree<TKey>::RedBlackTree() : header_() { }

    template <typename TKey>
    RedBlackTree<TKey>::RedBlackTree(const RedBlackTree<value_type>& other) : RedBlackTree()
    {
        for (const auto& val: other) {
            this->insert(val);
        }
    }

    template<typename TKey>
    template <class _InputIterator>
    RedBlackTree<TKey>::RedBlackTree(_InputIterator first,
                                     _InputIterator last)
                                     : RedBlackTree<TKey>::RedBlackTree()
    {
        for (auto iter(first); iter != last; iter++) {
            insert(*iter);
        }
    }

    template<typename TKey>
    RedBlackTree<TKey>::RedBlackTree(const std::initializer_list<value_type>& l)
                                     : RedBlackTree(l.begin(), l.end()) { }

    template<typename TKey>
    RedBlackTree<TKey>::~RedBlackTree()
    {
        clear();
    }

    template<typename TKey>
    RedBlackTree<TKey>& RedBlackTree<TKey>::operator=(const RedBlackTree<value_type>& other)
    {
        if (&other != this) {
            this->clear();
            for (auto& elem: other) {
                this->insert(elem);
            }
        }
        return *this;
    }

    template<typename TKey>
    void RedBlackTree<TKey>::clear()
    {
        auto node = header_.data.parent;
        while (node && node != end().node) {
            if (node->lchild) {
                node = node->lchild;
            } else if (node->rchild) {
                node = node->rchild;
            } else {
                auto tmp = node->parent;
                drop(node);
                node = tmp;
            }
        }
    }

    template<typename TKey>
    void RedBlackTree<TKey>::drop(_Base_ptr node)
    {
        header_.nodes_count--;
        if (node->parent != end().node) {
            if (node->parent->is_lchild(node)) {
                node->parent->lchild = nullptr;
            } else if (node->parent->is_rchild(node)) {
                node->parent->rchild = nullptr;
            }
        } else {
            header_.data.rchild = header_.data.lchild = &header_.data;
            header_.data.parent = nullptr;
        }
        delete node;
    }

    template<typename TKey>
    void RedBlackTree<TKey>::insert(const value_type& val)
    {
        auto [node, is_exist] = contains(val);
        if (!is_exist) {
            if (node) {
                auto child = new Node(val);
                Balancer::insert_and_rebalance(child, node, header_.data);
            } else {
                node = new Node(val, Color::Black);
                node->parent = &header_.data;
                header_.data.rchild = header_.data.lchild = node;
                header_.data.parent = node;
            }
            header_.nodes_count++;
        }
    }

    template<typename TKey>
    void RedBlackTree<TKey>::erase(const value_type& val)
    {
        auto [node, is_exist] = contains(val);
        if (is_exist) {
            Balancer::erase_and_rebalance(node, header_.data);
            drop(node);
        }
    }

    template<typename TKey>
    std::pair<typename RedBlackTree<TKey>::_Base_ptr, bool>
    RedBlackTree<TKey>::contains(const value_type& key) const
    {
        _Base_ptr curr_it(header_.data.parent), prev_it(nullptr);
        while (curr_it) {
            prev_it = curr_it;
            if (key < curr_it->key) {
                curr_it = curr_it->lchild;
            } else if (curr_it->key < key) {
                curr_it = curr_it->rchild;
            } else {
                return { curr_it, true };
            }
        }
        return { prev_it, false };
    }

    template<typename TKey>
    typename RedBlackTree<TKey>::iterator RedBlackTree<TKey>::find(const value_type& val) const
    {
        auto [node, is_exist] = contains(val);
        if (is_exist) {
            return iterator(node);
        }
        return end();
    }

    template<typename TKey>
    typename RedBlackTree<TKey>::iterator RedBlackTree<TKey>::lower_bound(const value_type& val) const
    {
        _Base_ptr curr_it = header_.data.parent;
        _Base_ptr result = const_cast<Node<value_type>*>(&header_.data);
        while (curr_it) {
            if (!(curr_it->key < val)) {
                result = curr_it, curr_it = curr_it->lchild;
            } else {
                curr_it = curr_it->rchild;
            }
        }
        return iterator(result);
    }

    template<typename TKey>
    typename RedBlackTree<TKey>::iterator RedBlackTree<TKey>::upper_bound(const value_type& val) const
    {
        _Base_ptr curr_it = header_.data.parent;
        _Base_ptr result = const_cast<Node<value_type>*>(&header_.data);
        while (curr_it) {
            if (val < curr_it->key) {
                result = curr_it, curr_it = curr_it->lchild;
            } else {
                curr_it = curr_it->rchild;
            }
        }
        return iterator(result);
    }

    template<typename TKey>
    typename RedBlackTree<TKey>::iterator RedBlackTree<TKey>::begin() const
    {
        return iterator(header_.data.rchild);
    }

    template<typename TKey>
    typename RedBlackTree<TKey>::iterator RedBlackTree<TKey>::end() const
    {
        return iterator(const_cast<Node<value_type>*>(&header_.data));
    }

    template<typename TKey>
    typename RedBlackTree<TKey>::reverse_iterator RedBlackTree<TKey>::rbegin() const
    {
        return reverse_iterator(iterator(end()));
    }

    template<typename TKey>
    typename RedBlackTree<TKey>::reverse_iterator RedBlackTree<TKey>::rend() const
    {
        return reverse_iterator(begin());
    }

    template<typename TKey>
    size_t RedBlackTree<TKey>::size() const
    {
        return header_.nodes_count;
    }

    template<typename TKey>
    bool RedBlackTree<TKey>::empty() const
    {
        return header_.nodes_count == 0;
    }

    template<typename TKey>
    typename RedBlackTree<TKey>::_Base_ptr RedBlackTree<TKey>::root() const
    {
        return const_cast<_Base_ptr>(header_.data.parent);
    }

    template<typename TKey>
    typename RedBlackTree<TKey>::_Base_ptr RedBlackTree<TKey>::leftmost() const
    {
        return header_.data.rchild;
    }

    template<typename TKey>
    typename RedBlackTree<TKey>::_Base_ptr RedBlackTree<TKey>::rightmost() const
    {
        return header_.data.lchild;
    }

    template<typename TKey>
    typename RedBlackTree<TKey>::_Base_ptr RedBlackTree<TKey>::minimum(_Base_ptr node)
    {
        while (node->lchild) {
            node = node->lchild;
        }
        return node;
    }

    template<typename TKey>
    typename RedBlackTree<TKey>::_Base_ptr RedBlackTree<TKey>::maximum(_Base_ptr node)
    {
        while (node->rchild) {
            node = node->rchild;
        }
        return node;
    }


    ///////////////////////////////////////////////////////////////////////////////
    /// Implementation of private template class RedBlackTree::Balancer
    ///////////////////////////////////////////////////////////////////////////////

    template<typename TKey>
    typename RedBlackTree<TKey>::_Base_ptr RedBlackTree<TKey>::Balancer::lrotate(_Base_ptr node)
    {
        auto isroot = is_root(node);
        auto rchild = node->rchild;
        auto parent = node->parent;
        rchild->parent = parent;
        node->parent = rchild;
        node->rchild = rchild->lchild;
        if (node->rchild) {
            node->rchild->parent = node;
        }
        rchild->lchild = node;
        if (!isroot) {
            if (parent->is_lchild(node)) {
                parent->lchild = rchild;
            } else {
                parent->rchild = rchild;
            }
        } else {
            parent->parent = rchild;
        }

        return rchild;
    }

    template<typename TKey>
    typename RedBlackTree<TKey>::_Base_ptr RedBlackTree<TKey>::Balancer::rrotate(_Base_ptr node)
    {
        auto isroot = is_root(node);
        auto lchild = node->lchild;
        auto parent = node->parent;
        lchild->parent = parent;
        node->parent = lchild;
        node->lchild = lchild->rchild;
        if (node->lchild) {
            node->lchild->parent = node;
        }
        lchild->rchild = node;
        if (!isroot) {
            if (parent->is_lchild(node)) {
                parent->lchild = lchild;
            } else {
                parent->rchild = lchild;
            }
        } else {
            parent->parent = lchild;
        }

        return lchild;
    }

    template<typename TKey>
    bool RedBlackTree<TKey>::Balancer::is_black(_Base_ptr node)
    {
        return (!node || (node->color == Color::Black));
    }

    template<typename TKey>
    void RedBlackTree<TKey>::Balancer::relink_parent(_Base_ptr& parent,
                                        _Base_ptr& prev,
                                        _Base_ptr& curr,
                                        bool prev_is_root)
    {
        if (curr) {
            curr->parent = parent;
        }
        if (!prev || !prev_is_root) {
            if (parent->is_lchild(prev)) {
                parent->lchild = curr;
            } else {
                parent->rchild = curr;
            }
        } else {
            parent->parent = curr;
        }
    }

    template<typename TKey>
    void RedBlackTree<TKey>::Balancer::swap(_Base_ptr& node, _Base_ptr& other)
    {
        auto node_is_root(is_root(node));
        auto other_is_root(is_root(other));
        auto other_is_child = other->parent == node;
        if (other_is_child || node->parent == other) {
            auto [parent, child] = other_is_child ?
                std::pair<_Base_ptr, _Base_ptr>({ node, other }) :
                std::pair<_Base_ptr, _Base_ptr>({ other, node });
            relink_parent(parent->parent, parent, child, is_root(parent));
            parent->parent = child;
            parent->rchild = child->rchild;
            if (child->rchild) {
                child->rchild->parent = parent;
            }
            child->rchild = parent;
            child->lchild = parent->lchild;
            if (parent->lchild) {
                parent->lchild->parent = child;
            }
            parent->lchild = nullptr;
        } else {
            auto other_parent = other->parent;
            relink_parent(node->parent, node, other, node_is_root);
            relink_parent(other_parent, other, node, other_is_root);
            auto other_lchild = other->lchild;
            relink_parent(other, other_lchild, node->lchild);
            relink_parent(node, node->lchild, other_lchild);
            auto other_rchild = other->rchild;
            relink_parent(other, other_rchild, node->rchild);
            relink_parent(node, node->rchild, other_rchild);
        }
        if (node->color != other->color) {
            node->repaint(), other->repaint();
        }
    }

    template<typename TKey>
    typename RedBlackTree<TKey>::_Base_ptr
    RedBlackTree<TKey>::Balancer::insert_and_rebalance(_Base_ptr node,
                                                       _Base_ptr parent,
                                                       _Base& header_data)
    {
        if (node->key < parent->key) {
            parent->lchild = node;
            if (parent == header_data.rchild) {
                header_data.rchild = node;
            }
            node->parent = parent;
        } else {
            parent->rchild = node;
            if (parent == header_data.lchild) {
                header_data.lchild = node;
            }
            node->parent = parent;
        }

        while (node->color == Color::Red && node->parent->color == Color::Red) {
            parent = node->parent;
            auto grandpa = parent->parent;
            bool node_is_left(parent->is_lchild(node));
            bool parent_is_left = (grandpa->lchild == parent);
            auto uncle = parent_is_left ? grandpa->rchild : grandpa->lchild;

            if (uncle && uncle->color == Color::Red) {
                uncle->repaint(), parent->repaint(), grandpa->repaint();
                node = grandpa;
            } else {
                if (node_is_left == parent_is_left) {
                    node = node_is_left ? rrotate(grandpa) : lrotate(grandpa);
                    parent->repaint(), grandpa->repaint();
                } else {
                    if (parent_is_left) {
                        node = lrotate(parent)->lchild;
                    } else {
                        node = rrotate(parent)->rchild;
                    }
                }
            }

            if (node->parent == &header_data) {
                node->repaint(Color::Black);
            }
        }
        return node;
    }

    template<typename TKey>
    void RedBlackTree<TKey>::Balancer::erase_and_rebalance(_Base_ptr node, _Base& header_data)
    {
        auto isroot(is_root(node));
        if (isroot && header_data.rchild == node && header_data.lchild == node) {
            header_data.rchild = header_data.lchild = &header_data;
            header_data.parent = nullptr;
            return;
        }

        if (node->lchild && node->rchild) {
            auto upbound = node->rchild;
            while (upbound->lchild) {
                upbound = upbound->lchild;
            }
            swap(node, upbound);
            isroot = false;
        }

        _Base_ptr child(nullptr);
        if ((child = node->lchild ? node->lchild : node->rchild)) {
            child->parent = node->parent;
            node->parent = child;
            if (!isroot) {
                if (child->parent->is_lchild(node)) {
                    child->parent->lchild = child;
                } else {
                    child->parent->rchild = child;
                }
            } else {
                child->parent->parent = child;
            }
            if (node->color == Color::Black && child->color == Color::Red) {
                node->repaint(), child->repaint();
            }
        }

        if (header_data.rchild == node) {
            header_data.rchild = node->parent;
        }
        if (header_data.lchild == node) {
            header_data.lchild = node->parent;
        }

        if (node->color == Color::Black) {
            bool rebalance(true);

            while (rebalance) {
                _Base_ptr bug_node = node->parent;
                bool brother_left = bug_node->is_rchild(node);
                _Base_ptr brother = brother_left ? bug_node->lchild : bug_node->rchild;
                if (brother->color == Color::Black) {
                    if (is_black(brother->lchild) && is_black(brother->rchild)) {
                        brother->repaint();
                        if (is_black(bug_node)) {
                            node = bug_node;
                            rebalance = !is_root(node);
                        } else {
                            bug_node->repaint();
                            rebalance = false;
                        }
                    } else if (!is_black(brother->rchild)) {
                        if (brother_left) {
                            lrotate(brother);
                            brother->repaint();
                            brother->parent->repaint();
                        } else {
                            lrotate(bug_node);
                            brother->rchild->repaint(Color::Black);
                            brother->repaint(bug_node->color);
                            bug_node->repaint(Color::Black);
                            rebalance = false;
                        }
                    } else {
                        if (brother_left) {
                            rrotate(bug_node);
                            brother->lchild->repaint(Color::Black);
                            brother->repaint(bug_node->color);
                            bug_node->repaint(Color::Black);
                            rebalance = false;
                        } else {
                            rrotate(brother);
                            brother->repaint();
                            brother->parent->repaint();
                        }
                    }
                } else {
                    if (brother_left) {
                        rrotate(bug_node);
                    } else {
                        lrotate(bug_node);
                    }
                    bug_node->repaint();
                    brother->repaint();
                }
            }
        }
    }
}
