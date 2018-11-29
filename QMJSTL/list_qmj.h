#pragma once
#ifndef _LIST_QMJ_
#define _LIST_QMJ_

#include <initializer_list>
#include "allocator.h"

namespace qmj {
    template<typename value_type>
    struct list_node;

    template<typename value_type>
    struct list_base_node {
        typedef list_node<value_type> *link_type;

        list_base_node(link_type prev = nullptr, link_type next = nullptr)
                : prev(prev), next(next) {}

        link_type prev;
        link_type next;
    };

    template<typename value_type>
    struct list_node : public list_base_node<value_type> {
        typedef list_node<value_type> *link_type;
        typedef list_base_node<value_type> base_type;

        list_node(link_type prev = nullptr, link_type next = nullptr)
                : base_type(prev, next), data() {}

        list_node(link_type prev, link_type next, const value_type &data)
                : base_type(prev, next), data(data) {}

        value_type data;
    };

    template<typename value_type_>
    class list_const_iterator {
    public:
        template<typename value_type, typename alloc>
        friend
        class slist;

        typedef std::bidirectional_iterator_tag iterator_category;
        typedef value_type_ value_type;
        typedef const value_type *pointer;
        typedef const value_type &reference;
        typedef ptrdiff_t difference_type;

        typedef list_const_iterator<value_type> self;
        typedef list_node<value_type> *link_type;
        typedef size_t size_type;

        list_const_iterator(link_type node = nullptr) : node(node) {}

        list_const_iterator(const self &x) : node(x.node) {}

        self &operator=(const self &x) {
            node = x.node;
            return (*this);
        }

        bool operator==(const self &x) const { return (node == x.node); }

        bool operator!=(const self &x) const { return !operator==(x); }

        reference operator*() const { return node->data; }

        pointer operator->() const { return &(operator*()); }

        self &operator++() {
            node = node->next;
            return *this;
        }

        self operator++(int) {
            auto ret = *this;
            ++*this;
            return ret;
        }

        self &operator--() {
            node = node->prev;
            return *this;
        }

        self operator--(int) {
            auto ret = *this;
            ++*this;
            return ret;
        }

    protected:
        link_type get_node() { return node; }

    protected:
        link_type node;
    };

    template<typename value_type_>
    class list_iterator : public list_const_iterator<value_type_> {
    public:
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef value_type_ value_type;
        typedef value_type *pointer;
        typedef value_type &reference;
        typedef ptrdiff_t difference_type;

        typedef list_iterator<value_type> self;
        typedef list_const_iterator<value_type> iterator_base;
        typedef list_node<value_type> *link_type;
        typedef size_t size_type;

        list_iterator(const link_type node = nullptr) : iterator_base(node) {}

        list_iterator(const self &x) : iterator_base(x.node) {}

        self &operator=(const self &x) {
            this->node = x.node;
            return (*this);
        }

        bool operator==(const self &x) const { return this->node == x.node; }

        bool operator!=(const self &x) const { return !operator==(x); }

        reference operator*() const { return this->node->data; }

        pointer operator->() const { return &(operator*()); }

        self &operator++() {
            this->node = this->node->next;
            return *this;
        }

        self operator++(int) {
            auto ret = *this;
            ++*this;
            return ret;
        }

        self &operator--() {
            this->node = this->node->prev;
            return *this;
        }

        self operator--(int) {
            auto ret = *this;
            ++*this;
            return ret;
        }
    };

    template<typename value_type_,
            typename Alloc = _QMJ allocator<value_type_>>
    class slist {
    public:
        typedef slist<value_type_, Alloc> self;
        typedef Alloc allocator_type;
        typedef list_node<value_type_> *link_type;
        typedef typename allocator_type::template rebind<list_node<value_type_>>::other
                alloc;
        typedef list_base_node<value_type_> base_node_type;
        typedef base_node_type *base_link_type;
        typedef typename allocator_type::template rebind<base_node_type>::other
                alloc_type;

        typedef value_type_ value_type;
        typedef value_type *pointer;
        typedef const value_type *const_pointer;
        typedef value_type &reference;
        typedef const value_type &const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t differene_type;

        typedef list_iterator<value_type> iterator;
        typedef list_const_iterator<value_type> const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;


    private:
        link_type node;
    };
}

#endif