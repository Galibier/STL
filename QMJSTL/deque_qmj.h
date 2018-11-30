#pragma once
#ifndef _DEQUE_QMJ_
#define _DEQUE_QMJ_

#include "allocator.h"
#include "iterator_qmj.h"
#include "vector_qmj.h"

namespace qmj {
    template<typename value_type, typename alloc, typename>
    class deque;

    template<typename value_type_>
    class deque_const_iterator {
    public:
        template<typename _value_type_, typename alloc, typename>
        friend
        class deque;

        typename std::random_access_iterator_tag iterator_category;
        typedef value_type_ value_type;
        typedef const value_type *pointer;
        typedef const value_type &reference;
        typedef ptrdiff_t difference_type;

        typedef const pointer *map_pointer;
        typedef deque_const_iterator<value_type> self;

        deque_const_iterator() : node(nullptr) {}

        deque_const_iterator(value_type **node) : node(node) {}

        deque_const_iterator(const self &x) : node(x.node) {}

        self &operator=(const self &x) {
            node = x.node;
            return (*this);
        }

        bool operator==(const self &x) const { return node == x.node; }

        bool operator!=(const self &x) const { return !operator==(x); }

        bool operator<(const self &x) const { return node < x.node; }

        bool operator<=(const self &x) const { return node <= x.node; }

        bool operator>(const self &x) const { return node > x.node; }

        bool operator>=(const self &x) const { return node >= x.node; }

        reference operator*() const { return (**node); }

        pointer operator->() const { return &(operator*()); }

        reference operator[](difference_type off) const { return (**(node + off)); }

        self &operator++() {
            ++node;
            return *this;
        }

        self operator++(int) {
            auto ret = *this;
            operator++();
            return ret;
        }

        self &operator--() {
            --node;
            return *this;
        }

        self operator--(int) {
            auto ret = *this;
            operator--();
            return ret;
        }

        self operator+(const difference_type n) const {
            return self(node + n);
        }

        self &operator+=(const difference_type n) {
            node += n;
            return *this;
        }

        self operator-(const difference_type n) const {
            return self(node - n);
        }

        difference_type operator-(const self &x) const {
            return node - x.node;
        }

        self &operator-=(const difference_type n) {
            node -= n;
            return *this;
        }

    private:
        value_type **get_node() { return node; }

    protected:
        value_type **node;
    };

    template<typename value_type_>
    class deque_iterator : public deque_const_iterator<value_type_> {
    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef value_type_ value_type;
        typedef value_type *pointer;
        typedef value_type &reference;
        typedef ptrdiff_t difference_type;

        typedef pointer *map_pointer;
        typedef deque_iterator<value_type> self;
        typedef deque_const_iterator<value_type> iterator_base;

        deque_iterator() : iterator_base() {}

        deque_iterator(value_type **node) : iterator_base(node) {}

        deque_iterator(const self &x) : iterator_base(x.node) {}

        self &operator=(const self &x) {
            this->node = x.node;
            return (*this);
        }

        void iter_swap(self &right) noexcept { std::iter_swap(*this->node, *(right.node)); }

        bool operator==(const self &x) const { return this->node == x.node; }

        bool operator!=(const self &x) const { return !operator==(x); }

        bool operator<(const self &x) const { return this->node < x.node; }

        bool operator<=(const self &x) const { return this->node <= x.node; }

        bool operator>(const self &x) const { return this->node > x.node; }

        bool operator>=(const self &x) const { return this->node >= x.node; }

        reference operator*() const { return (*(*this->node)); }

        pointer operator->() const { return &(operator*()); }

        reference operator[](difference_type off) const {
            return (**(this->node + off));
        }

        self &operator++() {
            ++this->node;
            return *this;
        }

        self operator++(int) {
            auto ret = *this;
            operator++();
            return ret;
        }

        self &operator--() {
            --this->node;
            return *this;
        }

        self operator--(int) {
            auto ret = *this;
            operator--();
            return ret;
        }

        self operator+(const difference_type n) const {
            return self(this->node + n);
        }

        self &operator+=(const difference_type n) {
            this->node += n;
            return *this;
        }

        self operator-(const difference_type n) const {
            return self(this->node - n);
        }

        difference_type operator-(const self &x) const {
            return this->node - x.node;
        }

        self &operator-=(const difference_type n) {
            this->node -= n;
            return *this;
        }
    };
}

#endif //_DEQUE_QMJ_
