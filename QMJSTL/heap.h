#pragma once
#ifndef _HEAP_
#define _HEAP_

#include <cmath>
#include "vector_qmj.h"

namespace qmj {
    template<typename value_type_, typename Container = _QMJ vector<value_type_>,
            typename Compare = std::less<value_type_>>
    class binary_heap {
    public:
        typedef Container container_type;
        typedef value_type_ value_type;
        typedef typename container_type::size_type size_type;
        typedef typename container_type::reference reference;
        typedef typename container_type::const_reference const_reference;
        typedef binary_heap<value_type, Container, Compare> self;

        binary_heap() : heap_v(), comp() {}

        template<typename Iter>
        binary_heap(Iter first, Iter last):heap_v(first, last) {
            build_heap();
        }

        template<typename Iter>
        binary_heap(Iter first, Iter last, const Container &cont, const Compare &comp)
                :heap_v(cont), comp(comp) {
            heap_v.insert(this->end(), first, last);
            build_heap();
        }

        binary_heap(const Container &cont, const Compare &comp = Compare())
                : heap_v(cont), comp(comp) {
            build_heap();
        }

        explicit binary_heap(const Compare &cmp) : heap_v(), comp(cmp) {}

        binary_heap(const self &x) : heap_v(x.heap_v), comp(x.comp) {}

        binary_heap(self &&x) : heap_v(std::move(x.heap_v)), comp(x.comp) {}

        size_type size() const { return heap_v.size(); }

        self &operator=(const self &x) {
            heap_v = x.heap_v;
            comp = x.comp;
            return (*this);
        }

        self &operator=(self &&x) {
            heap_v = std::move(x.heap_v);
            comp = std::move(x.comp);
            return (*this);
        }

        bool empty() const { return (heap_v.empty()); }

        const_reference top() const { return (heap_v.front()); }

        void push(const value_type &val) {
            heap_v.push_back(val);
            change_key_imple(heap_v.size() - 1, val);
        }

        void pop() {
            heap_v[0] = std::move(heap_v.back());
            heap_v.pop_back();
            heapify(0);
        }

        value_type pop_top() {
            value_type top = heap_v[0];
            heap_v[0] = heap_v.back();
            heap_v.pop_back();
            heapify(0);
            return (top);
        }

        void change_key(const size_t index, const value_type &val) {
            if (comp(heap_v[index], val))
                return;
            change_key_imple(index, val);
        }

        void swap(self &x) noexcept {
            std::swap(comp, x.comp);
            std::swap(heap_v, x.heap_v);
        }

    protected:
        void change_key_imple(size_t index, const value_type &val) {
            for (size_t par_index = parent(index); index && comp(heap_v[par_index], val); par_index = parent(index)) {
                heap_v[index] = std::move(heap_v[par_index]);
                index = par_index;
            }
            heap_v[index] = val;
        }

        void heapify(size_t index) {
            const value_type val = heap_v[index];
            size_t right_index = right(index);
            const size_t len = heap_v.size();
            for (; right_index < len;) {
                if (comp(heap_v[right_index], heap_v[right_index - 1]))
                    --right_index;
                if (comp(val, heap_v[right_index]))
                    heap_v[index] = std::move(heap_v[right_index]);
                else {
                    heap_v[index] = val;
                    return;
                }
                index = right_index;
                right_index = right(index);
            }
            if (right_index == len && comp(val, heap_v[--right_index])) {
                heap_v[index] = std::move(heap_v[right_index]);
                index = right_index;
            }
            heap_v[index] = std::move(val);
        }

        void build_heap() {
            size_t index = heap_v.size();
            if (index < 2)
                return;
            for (index = parent(index) + 1; index != 0;)
                heapify(--index);
        }

        size_t left(const size_t index) { return (index << 1) + 1; }

        size_t right(const size_t index) { return (index << 1) + 2; }

        size_t parent(const size_t index) { return (index - 1) >> 1; }

    private:
        Container heap_v;
        Compare comp;
    };

    template<typename value_type, typename Container, typename Compare>
    inline void swap(binary_heap<value_type, Container, Compare> &left,
                     binary_heap<value_type, Container, Compare> &right) noexcept {
        left.swap(right);
    }
}



#endif //_HEAP_
