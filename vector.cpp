#include <iostream>
#include <sstream>
#include <iterator>
#include <utility>
#include <memory>
#include <deque>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

template<typename T>
struct Raw {
  T * data = nullptr;
  size_t capacity = 0;

  static T* alloc(size_t n) {
    return static_cast<T*>(operator new(n * sizeof(T)));
  }
  static void dealloc(T* buf) {
    operator delete(buf);
  }

  Raw() = default;
  Raw(size_t n) {
    data = alloc(n);
    capacity = n;
  }
  Raw(Raw&& other) {
    swap(other);
  }
  ~Raw() {
    dealloc(data);
  }

  Raw& operator = (Raw&& other) {
      swap(other);
      return *this;
  }

  T* operator + (size_t i) {
    return (data + i);
  }

  const T* operator + (size_t i) const {
    return (data + i);
  }

  T& operator[] (size_t i) {
    return data[i];
  }

  const T& operator[] (size_t i) const {
    return data[i];
  }

  void swap(Raw& other) {
    std::swap(data, other.data);
    std::swap(capacity, other.capacity);
  }
};

template<typename T>
class Vector {
private:
    Raw<T> data;
    size_t arraySize = 0;

    static void construct(void* buf, const T& el) {
      new (buf) T(el);
    }
    static void construct(void* buf, T&& el) { // NOLINT
      new (buf) T(std::move(el));
    }
    static void destroy(T* buf) {
      buf->~T();
    }

public:
    Vector() = default;

    Vector(size_t n)
    : data(n) {
        std::uninitialized_value_construct_n(data.data, n);
        arraySize = n;
    }

    Vector(const Vector& vec)
    : data(vec.arraySize) {
        std::uninitialized_copy_n(vec.data.data, vec.arraySize, data.data);
        arraySize = vec.arraySize;
    }

    Vector(Vector&& vec) {
        swap(vec);
    }

    void swap(Vector& other) {
      data.swap(other.data);
      std::swap(arraySize, other.arraySize);
    }

    void reserve(size_t n) {
      if (n > data.capacity) {
        Raw<T> data1(n);
        std::uninitialized_move_n(data.data, arraySize, data1.data);
        std::destroy_n(data.data, arraySize);
        data.swap(data1);
      }
    }

    Vector& operator = (Vector&& vec) {
        swap(vec);
        return *this;
    }

    Vector& operator = (const Vector& vec) {
        if (vec.arraySize > data.capacity) {
          Vector v(vec);
          swap(v);
        } else {
          for (size_t i = 0; i < arraySize && i < vec.arraySize; i++) {
            data[i] = vec[i];
          }
          if (arraySize < vec.arraySize) {
            std::uninitialized_copy_n(vec.data.data + arraySize,
              vec.arraySize - arraySize, data.data + arraySize);
          } else if (arraySize > vec.arraySize) {
            std::destroy_n(data.data + vec.arraySize, arraySize - vec.arraySize);
          }
          arraySize = vec.arraySize;
        }
        return *this;
    }

    void resize(size_t n) {
      reserve(n);
      if (arraySize < n) {
        std::uninitialized_value_construct_n(data + arraySize, n - arraySize);
      } else {
        std::destroy_n(data + n, arraySize - n);
      }
      arraySize = n;
    }

    void push_back(const T& x) {
        if (arraySize == data.capacity) {
          if (arraySize == 0) {
            reserve(1);
          } else {
            reserve(arraySize * 2);
          }
        }
        new (data + arraySize) T(x);
        arraySize++;
    }

    void push_back(T&& x) {
        if (arraySize == data.capacity) {
          if (arraySize == 0) {
            reserve(1);
          } else {
            reserve(arraySize * 2);
          }
        }
        new (data + arraySize) T(std::move(x));
        arraySize++;
    }

    void pop_back() {
      std::destroy_at(data + arraySize - 1);
      arraySize--;
    }

    void clear() {
      std::destroy_n(data.data, arraySize);
      arraySize = 0;
    }

    T* begin() {
      return (data.data);
    }

    const T* begin() const {
      return (data.data);
    }

    T* end() {
      return (data + arraySize);
    }

    const T* end() const {
      return (data + arraySize);
    }

    T& operator[] (size_t i) {
        return data[i];
    }

    const T& operator[] (size_t i) const {
        return data[i];
    }

    size_t size() {
        return arraySize;
    }

    const size_t size() const {
        return arraySize;
    }

    size_t capacity() {
        return data.capacity;
    }

    const size_t capacity() const {
        return data.capacity;
    }

    ~Vector() {
        std::destroy_n(data.data, arraySize);
    }
};
