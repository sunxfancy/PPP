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

private:
    struct DataContainer {
        DataContainer* parent;
        T data;
    };
    DataContainer* top = nullptr;
    int size = 0;
};



#endif /* end of include guard: SHARED_STACK_HPP */
