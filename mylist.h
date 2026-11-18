#pragma once
#include <cstddef>

template<typename T>
class MyList {
public:
    struct Node {
        T val;
        Node* next;
        Node(const T& v) : val(v), next(nullptr) {}
    };

    Node* head;
    size_t sz;

public:
    MyList() : head(nullptr), sz(0) {}
    ~MyList() { clear(); }

    void clear() {
        Node* cur = head;
        while (cur) { Node* n = cur->next; delete cur; cur = n; }
        head = nullptr;
        sz = 0;
    }

    void push_back(const T& val) {
        Node* newNode = new Node(val);
        if (!head) head = newNode;
        else {
            Node* cur = head;
            while (cur->next) cur = cur->next;
            cur->next = newNode;
        }
        sz++;
    }

    size_t size() const { return sz; }
    bool empty() const { return sz == 0; }
};