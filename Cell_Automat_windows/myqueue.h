#pragma once
#include "mylist.h"

template<typename T>
class MyQueue {
    MyList<T> list;
    

public:
    void push(const T& val) { list.push_back(val); }
    bool empty() const { return list.empty(); }
    size_t size() const { return list.size(); }

    T pop() {
        if (list.empty()) return T();
        T val = list.head->val;
        typename MyList<T>::Node* temp = list.head;
        list.head = list.head->next;
        delete temp;
        list.sz--;
        return val;
    }
};