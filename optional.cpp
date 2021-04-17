#include <iostream>
#include <string>

using namespace std;

struct BadOptionalAccess {
};

template <typename T>
class Optional {
private:
    // alignas нужен для правильного выравнивания блока памяти
    alignas(T) unsigned char data[sizeof(T)];
    bool defined = false;

public:
    Optional() {
    }

    Optional(const T& elem) {
        new (data) T(elem);
        defined = true;
    }

    Optional(Optional &&other) {
        *this = std::move(other);
        defined = true;
    }

    Optional(T && elem) {
        new (data) T(std::move(elem));
        defined = true;
    }

    Optional(const Optional& other) {
        if (other.defined) {
            new (data) T(*other.data);
            defined = true;
        } else {
            defined = false;
        }
    }

    Optional& operator=(const Optional& other) {
        if (!other.defined && !defined) {
            return *this;
        }
        if (other.defined) {
            if (!defined) {
                new (data) T(*(reinterpret_cast<T*>(data)));
                defined = true;
                return *this;
            } else {
                *(reinterpret_cast<T*>(data)) = *(reinterpret_cast<const T*>(other.data));
                defined = true;
                return *this;
            }
        } else {
            if (defined) {
                reinterpret_cast<T*>(data)->~T();
            }
            defined = false;
            return *this;
        }
    }

    Optional& operator=(const T& elem) {
        if (defined) {
            reinterpret_cast<T*>(data)->~T();
        }
        *(reinterpret_cast<T*>(data)) = elem;
        defined = true;
        return *this;
    }

    Optional& operator=(T&& elem) {
            if (defined) {
                *(reinterpret_cast<T*>(data)) = std::move(elem);
                defined = true;
                return *this;
            } else {
                new (data) T(std::move(elem));
                defined = true;
                return *this;
            }
    }

    bool has_value() const {
        return defined;
    }

    T& operator*() {
        return *(reinterpret_cast<T*>(data));
    }

    const T& operator*() const {
        return *(reinterpret_cast<const T*>(data));
    }

    T* operator->() {
        return reinterpret_cast<T*>(&data);
    }

    const T* operator->() const {
        return reinterpret_cast<const T*>(&data);
    }

    T& value() {
        if (defined) {
            return *(reinterpret_cast<T*>(data));
        } else {
            throw BadOptionalAccess();
        }
    }

    const T& value() const {
        if (defined) {
            return *(reinterpret_cast<const T*>(data));
        } else {
            throw BadOptionalAccess();
        }
    }

    void reset() {
        if (defined) {
            reinterpret_cast<T*>(data)->~T();
        }
        defined = false;
    }

    ~Optional() {
        if (defined) {
            reinterpret_cast<T*>(data)->~T();
        }
        defined = false;
    }
};