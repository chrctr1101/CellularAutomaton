#pragma once
#include <cstddef>

template<typename T>
class MyVector {
    T* data;
    size_t sz;
    size_t cap;

    void realloc(size_t newCap) {
        T* newData = new T[newCap];
        for (size_t i = 0; i < sz; ++i) newData[i] = data[i];
        delete[] data;
        data = newData;
        cap = newCap;
    }

public:
    MyVector() : data(nullptr), sz(0), cap(0) {}
    
    // Конструктор копирования
    MyVector(const MyVector& other) : data(nullptr), sz(0), cap(0) {
        if (other.sz > 0) {
            data = new T[other.sz];
            for (size_t i = 0; i < other.sz; ++i) data[i] = other.data[i];
            sz = other.sz;
            cap = other.sz;
        }
    }
    
    // Оператор присваивания
    MyVector& operator=(const MyVector& other) {
        if (this == &other) return *this;
        delete[] data;
        data = nullptr;
        sz = 0;
        cap = 0;
        if (other.sz > 0) {
            data = new T[other.sz];
            for (size_t i = 0; i < other.sz; ++i) data[i] = other.data[i];
            sz = other.sz;
            cap = other.sz;
        }
        return *this;
    }

    ~MyVector() { delete[] data; }

    void push_back(const T& val) {
        if (sz == cap) realloc(cap == 0 ? 4 : cap * 2);
        data[sz++] = val;
    }

    void pop_back() {
        if (sz > 0) sz--;
    }

    T& operator[](size_t i) { return data[i]; }
    const T& operator[](size_t i) const { return data[i]; }
    size_t size() const { return sz; }
    bool empty() const { return sz == 0; }
    void clear() { sz = 0; }
};