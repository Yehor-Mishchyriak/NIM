#pragma once
#include <stddef.h>

#define DEFINE_TUPLE_TYPE(T, N)  \
    typedef struct {            \
        T* elements;            \
        size_t size;            \
    } T##_##N##_tuple;

#define DEFINE_TUPLE_SIZE(T, N)                         \
    static inline size_t T##_##N##_tuple_len(           \
        T##_##N##_tuple t                               \
    ) {                                                 \
        return t.size;                                  \
    }

#define CTUPLE_GET(t, i) ((t).elements[(i)])

