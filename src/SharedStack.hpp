#ifndef SHARED_STACK_HPP
#define SHARED_STACK_HPP
#include <deque>
#include "LexInterface.h"


template <typename T>
class SharedStack {
public:
    SharedStack(){}
    SharedStack(const SharedStack& other){
        top = other.top;
        size = other.size;
    }
    ~SharedStack(){}

    int push(const T data){
        if (top == nullptr) {
            top = new DataContainer();
            top->parent = nullptr;
            top->data = data;
        } else {
            DataContainer* p = new DataContainer();
            p->parent = top;
            p->data = data;
            top = p;
        }
    }
    T pop(){
        DataContainer* p = top;
        top = top->parent;
    }

    std::deque<T>& getData() const {
        std::deque<T> data;
        for (DataContainer* p = top; p != nullptr; p = p->parent) {
            data.push_back(p->data);
        }
    }
    
    bool operator< (const SharedStack<T> other) const { 
        return this->greater(other) == -1;
    }
    bool operator> (const SharedStack<T> other) const { 
        return this->greater(other) == 1;
    }
    bool operator== (const SharedStack<T> other) const { 
        return this->greater(other) == 0;
    }

private:

    int greater(const SharedStack<T> other) const {
        if (size != other.size) return size < other.size;
        DataContainer *p1, *p2;
        for ( p1 = this->top, p2 = other.top ; p1 != nullptr && p2 != nullptr;
              p1 = p1->parent, p2 = p2->parent ) {
            if (p1->data < p2->data) return -1;
            if (p1->data > p2->data) return 1;
        }
        if (p1 == nullptr && p2 == nullptr) return 0;
        if (p1 == nullptr) return -1;
        if (p2 == nullptr) return 1;
    }

    struct DataContainer {
        DataContainer* parent;
        T data;
    };
    DataContainer* top = nullptr;
    int size = 0;
};



#endif /* end of include guard: SHARED_STACK_HPP */
