#include <iostream>
#include <string>
#include <cstddef>
#include <iostream>
#include <memory>
#include <type_traits>

using namespace std;

template<typename T>
class SharedPtr {
private:
    T * pt;
    int * n;

public:
    SharedPtr()
    : pt(nullptr)
    , n(new int(1)) {
    }

    SharedPtr(T* ptr)
    : pt(ptr)
    , n(new int(1)) {
    }

    SharedPtr(const SharedPtr& other)
    : pt(other.pt) {
        n = other.n;
        (*n)++;
    }

    SharedPtr(SharedPtr&& other) {
        pt = other.pt;
        n = other.n;
        other.pt = nullptr;
        other.n = new int(1);
    }

    SharedPtr& operator= (T* ptr) {
        (*n)--;
        n = new int(1);
        pt = ptr;
        return *this;
    }

    SharedPtr& operator= (const SharedPtr& other) {
        if (this != &other) {
            if (*n == 1) {
                delete pt;
                delete n;
                pt = other.pt;
                n = other.n;
                (*n)++;
                return *this;
            } else {
                (*n)--;
                pt = other.pt;
                n = other.n;
                (*n)++;
                return *this;
            }
        } else {
            return *this;
        }
    }

    SharedPtr& operator= (SharedPtr&& other) {
        if (this != &other) {
            if (*n == 1) {
                delete pt;
                delete n;
                pt = other.pt;
                n = other.n;
                other.pt = nullptr;
                other.n = new int(1);
                return *this;
            } else {
                (*n)--;
                pt = other.pt;
                n = other.n;
                other.pt = nullptr;
                other.n = new int(1);
                return *this;
            }
        } else {
            return *this;
        }
    }

    const T& operator * () const {
        return *pt;
    }

    T& operator * () {
        return *pt;
    }

    T * operator -> () const {
        return pt;
    }

    void reset(T* ptr) {
        if (*n == 1) {
            delete pt;
            delete n;
            n = new int(1);
            pt = ptr;
        } else {
            (*n)--;
            n = new int(1);
            pt = ptr;
        }
    }

    void swap(SharedPtr& other) {
        std::swap(pt, other.pt);
        std::swap(n, other.n);
    }

    T * get() const {
        return pt;
    }

    explicit operator bool() const {
        return !(pt == nullptr);
    }

    ~SharedPtr() {
        if (n != nullptr && (*n) < 2) {
            delete pt;
            delete n;
        } else {
            if (n != nullptr) {
                (*n)--;
            }
        }
    }
};