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

    template<typename traits>
    class hashtable {
    public:
        friend class hashtable_const_iterator<traits>;

        friend class hashtable_iterator<traits>;

        typedef typename traits::allocator_type allocator_type;
        //typedef typename traits::HashFunction hashfunction;
        typedef typename traits::HashFunction hasher;
        typedef typename traits::EqualKey equalkey;
        typedef typename traits::EqualKey key_equal;
        typedef typename traits::key_type key_type;
        typedef typename traits::value_type value_type;
        typedef value_type *pointer;
        typedef const value_type *const_pointer;
        typedef value_type &reference;
        typedef const value_type &const_reference;
        typedef size_t size_type;
        typedef std::ptrdiff_t difference_type;

        typedef typename allocator_type::template rebind<hashtable_node<value_type>>::other alloc;
        typedef hashtable_const_iterator<traits> const_iterator;
        typedef hashtable_const_local_iterator<traits> const_local_iterator;
        typedef typename If<is_same<key_type, value_type>::value, const_iterator,
                hashtable_iterator<traits>>::type iterator;
        typedef typename If<is_same<key_type, value_type>::value, const_local_iterator,
                hashtable_local_iterator<traits>>::type local_iterator;
        enum {
            is_multi = traits::is_multi
        };

        typedef hashtable_node<value_type> *link_type;
        typedef hashtable<traits> self;
        typedef qmj::vector<link_type> container;

        typedef std::pair<iterator, bool> PairIB;
        typedef std::pair<local_iterator, local_iterator> PairII;
        typedef std::pair<const_local_iterator, const_local_iterator> PairCC;
        typedef std::pair<link_type, link_type> PairLL;

        hashtable() : hash(), equals(), num_elements(0) { init_buckets(0); }

        explicit hashtable(size_t n) : hash(), equals(), num_elements(0) {
            init_buckets(n);
        }

        hashtable(size_t n, const hasher &hash)
                : hash(hash), equals(), num_elements(0) {
            init_buckets(n);
        }

        hashtable(const hasher &hash, const equalkey &equals)
                : hash(hash), equals(equals), num_elements(0) {
            init_buckets(0);
        }

        hashtable(const size_t n, const hasher &hash, const equalkey &equals)
                : hash(hash), equals(equals), num_elements(0) {
            init_buckets(n);
        }

        hashtable(const self &x)
                : buckets(x.buckets.size(), nullptr), hash(x.hash),
                  equals(x.equals), num_elements(x.num_elements) {
            for (size_t i = 0; i != buckets.size(); ++i)
                for (link_type cur = x.buckets[i]; cur; cur = cur->next)
                    buckets[i] = create_node(cur->value, buckets[i]);
        }

        hashtable(self &&x)
                : num_elements(x.num_elements), hash(std::move(x.hash)),
                  equals(std::move(x.equals)), buckets(std::move(x.buckets)) {}

        self &operator=(self x) {
            hash = x.hash;
            equals = x.equals;
            num_elements = x.num_elements;
            buckets.swap(x.buckets);
            return (*this);
        }

        ~hashtable() { clear(); }

        void swap(self &x) noexcept {
            swap(hash, x.hash);
            swap(equals, x.equals);
            std::swap(num_elements, x.num_elements);
            buckets.swap(x.buckets);
        }

        iterator begin() {
            if (empty())
                return (end());
            size_t i = 0;
            for (const size_t len = buckets.size(); i != len && (!buckets[i]);)
                ++i;
            return (iterator(buckets[i], this));
        }


    private:
        void init_buckets(const size_type n) {
            const size_type n_buckets = next_prime(n);
            buckets.resize(n_buckets, nullptr);
        }

        void clear() {
            const size_t len = buckets.size();
            for (size_t i = 0; i != len; ++i)
                for (link_type cur = buckets[i], next; cur; cur = next) {
                    next = cur->next;
                    destroy_and_free_node(cur);
                }

            memset(&buckets[0], 0, len);
            num_elements = 0;
        }

        void destroy_and_free_node(link_type node) {
            alloc::destroy(node);
            alloc::deallocate(node);
        }

    private:
        hasher hash;
        key_equal equals;
        container buckets;
        size_type num_elements;
    };
}

#endif //_HASH_TABLE_