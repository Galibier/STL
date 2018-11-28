#pragma once
#ifndef _HASH_TABLE_
#define _HASH_TABLE_

#include <utility>
#include "algorithm_qmj.h"
#include "hashfunction.h"
#include "type_traits_qmj.h"
#include "vector_qmj.h"


namespace qmj {
    template<typename value_type>
    struct hashtable_node {
    public:
        typedef hashtable_node<value_type> *link_type;

        hashtable_node(link_type next = nullptr) : value(), next(next) {}

        hashtable_node(const value_type &value, link_type next = nullptr)
                : value(value), next(next) {}

        value_type value;
        link_type next;
    };

    template<typename traits>
    class hashtable;

    template<typename traits>
    class hashtable_const_iterator {
    public:
        friend class hashtable<traits>;

        typedef std::forward_iterator_tag iterator_category;
        typedef typename traits::value_type value_type;
        typedef const value_type &reference;
        typedef const value_type *pointer;
        typedef std::ptrdiff_t difference_type;

        typedef size_t size_type;
        typedef hashtable_const_iterator<traits> self;
        typedef const hashtable<traits> *hashtable_link;
        typedef hashtable_node<value_type> *link_type;

        hashtable_const_iterator(link_type cur = nullptr, hashtable_link ht = nullptr)
                : cur(cur), ht(ht) {}

        hashtable_const_iterator(const self &x) : cur(x.cur), ht(x.ht) {}

        self &operator=(const self &x) {
            cur = x.cur;
            ht = x.ht;
            return (*this);
        }

        bool operator==(const self &it) const { return (cur == it.cur); }

        bool operator!=(const self &it) const { return (!(operator==(it))); }

        reference operator*() const { return (cur->value); }

        pointer operator->() const { return (&(operator*())); }

        self &operator++() {
            const link_type old = cur;
            cur = cur->next;
            if (cur == nullptr) {
                size_type index = ht->get_bucket_num(ht->get_key(old->value));
                for (size_t n = ht->buckets.size(); !cur && ++index != n;)
                    cur = ht->buckets[index];
            }
            return (*this);
        }

        self operator++(int) {
            self tmp = *this;
            ++*this;
            return tmp;
        }

    protected:
        link_type cur;
        hashtable_link ht;
    };

    template<typename traits>
    class hashtable_iterator : public hashtable_const_iterator<traits> {
    public:
        typedef hashtable_const_iterator<traits> base_type;

        typedef std::forward_iterator_tag iterator_category;
        typedef typename traits::value_type value_type;
        typedef value_type &reference;
        typedef value_type *pointer;
        typedef std::ptrdiff_t difference_type;

        typedef size_t size_type;
        typedef hashtable_iterator<traits> self;
        typedef const hashtable<traits> *hashtable_link;
        typedef hashtable_node<value_type> *link_type;

        hashtable_iterator(const link_type cur = nullptr, const hashtable_link ht = nullptr)
                : base_type(cur, ht) {}

        hashtable_iterator(const self &x) : base_type(x.cur, x.ht) {}

        self &operator=(const self &x) {
            this->cur = x.cur;
            this->ht = x.ht;
        }

        bool operator==(const self &it) const { return (this->cur == it.cur); }

        bool operator!=(const self &it) const { return (!(operator==(it))); }

        reference operator*() const { return (this->cur->value); }

        pointer operator->() const { return (&(operator*())); }

        self &operator++() {
            const link_type old = this->cur;
            this->cur = this->cur->next;
            if (!this->cur) {
                size_type index = this->ht->get_bucket_num(this->ht->get_key(old->value));
                for (size_t n = this->ht->buckets.size(); !(this->cur) && ++index != n;)
                    this->cur = this->ht->buckets[index];
            }
            return (*this);
        }

        self operator++(int) {
            self tmp = *this;
            ++*this;
            return (tmp);
        }
    };

    template<typename traits>
    class hashtable_const_local_iterator {
    public:
        friend class hashtable<traits>;

        typedef std::forward_iterator_tag iterator_category;
        typedef typename traits::value_type value_type;
        typedef const value_type &reference;
        typedef const value_type *pointer;
        typedef std::ptrdiff_t difference_type;

        typedef size_t size_type;
        typedef hashtable_const_local_iterator<traits> self;
        typedef hashtable_node<value_type> *link_type;

        hashtable_const_local_iterator(link_type cur = nullptr) : cur(cur) {}

        hashtable_const_local_iterator(const self &x) : cur(x.cur) {}

        bool operator==(const self &it) const { return (cur == it.cur); }

        bool operator!=(const self &it) const { return (!(operator==(it))); }

        reference operator*() const { return (cur->value); }

        pointer operator->() const { return (&(operator*())); }

        self &operator++() {
            cur = cur->next;
            return (*this);
        }

        self operator++(int) {
            self tmp = *this;
            ++*this;
            return (tmp);
        }

    protected:
        link_type cur;
    };

    template<typename traits>
    class hashtable_local_iterator : public hashtable_const_local_iterator<traits> {
    public:
        friend class hashtable<traits>;

        typedef std::forward_iterator_tag iterator_category;
        typedef typename traits::value_type value_type;
        typedef value_type &reference;
        typedef value_type *pointer;
        typedef std::ptrdiff_t difference_type;

        typedef size_t size_type;
        typedef hashtable_const_local_iterator<traits> base_type;
        typedef hashtable_local_iterator<traits> self;
        typedef hashtable_node<value_type> *link_type;

        hashtable_local_iterator(link_type cur = nullptr) : base_type(cur) {}

        hashtable_local_iterator(const self &x) : base_type(x) {}

        bool operator==(const self &it) const { return (this->cur == it.cur); }

        bool operator!=(const self &it) const { return (!(operator==(it))); }

        reference operator*() const { return (this->cur->value); }

        pointer operator->() const { return (&(operator*())); }

        self &operator++() {
            this->cur = this->cur->next;
            return (*this);
        }

        self operator++(int) {
            self tmp = *this;
            ++*this;
            return (tmp);
        }
    };

    static const int num_primes = 31;
    static const std::size_t prime_list[num_primes] = {
            7, 19, 37, 53, 97, 193,
            389, 769, 1543, 3079, 6151, 12289,
            24593, 49157, 98317, 196613, 393241, 786433,
            1572869, 3145739, 6291469, 12582917, 25165843, 50331653,
            100663319, 201326611, 402653189, 805306457, 1610612741, 3221225437ul,
            4294967291ul};

    inline std::size_t next_prime(std::size_t n) {
        const size_t *first = prime_list;
        auto last = first + num_primes;
        auto pos = qmj::lower_bound(first, last, n);
        return pos == last ? *(last - 1) : *pos;
    }
}

#endif //_HASH_TABLE_