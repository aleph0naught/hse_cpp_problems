#include <iostream>
#include <vector>
#include <list>
#include <stdexcept>
#include <utility>


template<class KeyType, class ValueType, class Hash = std::hash<KeyType> >
class HashMap {
public:
    using iterator = typename std::list<std::pair<const KeyType, ValueType>>::iterator;
    using const_iterator = typename std::list<std::pair<const KeyType, ValueType>>::const_iterator;

private:
    std::list<std::pair<const KeyType, ValueType>> data;
    std::vector<std::list<iterator>> table;
    Hash hasher;
    size_t number_of_elements = 0;
    size_t capacity;

    size_t Hash_F(const KeyType x) const {
        return (hasher(x) % capacity);
    }

    void Add(const std::pair<KeyType, ValueType> obj) {
        size_t hashed = Hash_F(obj.first);
        data.push_back(obj);
        auto it = prev(data.end());
        table[hashed].push_back(it);
    }

    void Initialize(const size_t cap = 32) {
        capacity = cap;
        table = std::vector<std::list<iterator>>(capacity);
    }

    void Rehash() {
        if (2 * number_of_elements < capacity) {
            return;
        }
        table.clear();
        capacity *= 2;
        Initialize(capacity);
        for (auto it = data.begin(); it != data.end(); ++it) {
            table[Hash_F(it->first)].push_back(it);
        }
    }

public:
    explicit HashMap(Hash hsh = Hash())
    : hasher(hsh) {
        Initialize();
    }

    template<typename Iter>
    HashMap (Iter begin, Iter end, Hash hsh = Hash()) 
    : HashMap(hsh) {
        for (auto it = begin; it != end; ++it) {
            insert(*it);
        }
    }

    HashMap(std::initializer_list<std::pair<KeyType, ValueType>> lst, Hash hsh = Hash()) 
    : HashMap(lst.begin(), lst.end(), hsh) {
    }

    iterator insert(std::pair<KeyType, ValueType> obj) {
        iterator it = find(obj.first);
        if (it != end()) {
            return it;
        }
        ++number_of_elements;
        Add(obj);
        Rehash();
        return prev(end());
    }

    void erase(KeyType to_delete) {
        size_t hsh = Hash_F(to_delete);
        for (auto it = table[hsh].begin(); it != table[hsh].end(); ++it) {
            if ((*it)->first == to_delete) {
                data.erase(*it);
                table[hsh].erase(it);
                --number_of_elements;
                return;
            }
        }
    }

    HashMap& operator= (const HashMap &other) {
        if (this == &other) {
            return *this;
        }
        clear();
        hasher = other.hash_function();
        Initialize(other.capacity);
        for (auto &i : other) {
            insert(i);
        }
        return *this;
    }

    iterator find(KeyType x) {
        size_t hsh = Hash_F(x);
        for (auto& i : table[hsh]) {
            if (i->first == x) {
                return i;
            }
        }
        return end();
    }

    const_iterator find(KeyType x) const {
        size_t hsh = Hash_F(x);
        for (auto& i : table[hsh]) {
            if (i->first == x) {
                return i;
            }
        }
        return end();
    }

    ValueType& operator[](KeyType x) {
        auto it = find(x);
        if (it == end()) {
            it = insert({x, ValueType()});
        }
        return it->second;
    }

    const ValueType& at(KeyType x) const {
        const_iterator it = find(x);
        if (it == end()) {
            throw std::out_of_range("not found");
        }
        return it->second;
    }

    void clear() {
        data.clear();
        table.clear();
        Initialize();
        number_of_elements = 0;
    }

    Hash hash_function() const {
        return hasher;
    }

    size_t size() const {
        return number_of_elements;
    }

    bool empty() const {
        return (number_of_elements == 0);
    }

    iterator begin() {
        return iterator(data.begin());
    }

    iterator end() {
        return iterator(data.end());
    }

    const_iterator begin() const {
        return data.begin();
    }

    const_iterator end() const {
        return data.end();
    }

    ~HashMap() {
        clear();
    }
};
