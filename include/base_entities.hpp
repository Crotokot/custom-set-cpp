#pragma once

namespace stl
{
    enum class Color { Red = false, Black = true };

    template <typename TKey>
    struct Node
    {
        typedef Node<TKey> _Base;
        typedef Node<TKey>* _Base_ptr;


        TKey key;
        Color color;
        _Base_ptr parent, lchild, rchild;

        Node() : key(), color(Color::Red), parent(nullptr), lchild(nullptr), rchild(nullptr) { }

        explicit Node(const TKey& val,
                      Color c = Color::Red)
                      : key(val)
                      , color(c)
                      , parent(nullptr)
                      , lchild(nullptr)
                      , rchild(nullptr) { }

        void repaint()
        {
            color = (Color)(((int)color + 1) % 2);
        }

        void repaint(const Color& c)
        {
            color = c;
        }

        inline bool is_rchild(_Base_ptr child) const
        {
            return this->rchild == child;
        }

        inline bool is_lchild(_Base_ptr child) const
        {
            return this->lchild == child;
        }

        _Base_ptr nextNode()
        {
            auto iter = this;
            if (iter->rchild) {
                iter = iter->rchild;
                while (iter->lchild)
                    iter = iter->lchild;
            } else {
                _Base_ptr parent = iter->parent;
                while (parent->is_rchild(iter)) {
                    iter = parent;
                    parent = parent->parent;
                }
                if (!is_header(iter))
                    iter = parent;
            }

            return iter;
        }

        _Base_ptr prevNode()
        {
            auto iter = this;
            if (iter->lchild) {
                iter = iter->lchild;
                while (iter->rchild)
                    iter = iter->rchild;
            } else {
                _Base_ptr parent = iter->parent;
                while (parent->is_lchild(iter)) {
                    iter = parent;
                    parent = parent->parent;
                }
                if (!is_header(iter))
                    iter = parent;
            }

            return iter;
        }
    };

    // Helper type to manage header and nodes_count
    template <typename TKey>
    struct RedBlackTreeHeader
    {
        // header address is end() of the container
        // left child is begin(), i.e the most left element
        // right child is 'end() - 1', i.e. the most right element
        // parrent is root of the tree

        typedef Node<TKey>* _Base_ptr;

        Node<TKey> data;
        size_t nodes_count;

        RedBlackTreeHeader()
        {
            data.color = Color::Red;
            reset();
        }

        void reset()
        {
            nodes_count = 0;
            data.parent = nullptr;
            data.lchild = data.rchild = &data;
        }
    };


    template<typename TKey>
    inline bool is_root(const Node<TKey>* node)
    {
        return (node->parent->parent == node);
    }

    template<typename TKey>
    inline bool is_header(const Node<TKey>* node)
    {
        return !node->parent || (node->lchild && node->rchild && !(node->lchild->key < node->rchild->key));
    }
}
