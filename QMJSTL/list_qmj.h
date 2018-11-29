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
}

#endif