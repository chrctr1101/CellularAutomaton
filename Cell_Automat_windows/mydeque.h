#pragma once
#include "myvector.h"
#include <cstddef>

template<typename T>
class MyDeque {
    MyVector<T> items;

public:
    void push_back(const T& val) { items.push_back(val); }
    bool empty() const { return items.empty(); }

    T pop_back() {
        if (items.empty()) return T();
        T val = items[items.size() - 1];
        // Упрощенная очистка для истории
        items.clear();
        return val;
    }

    size_t size() const { return items.size(); }
};