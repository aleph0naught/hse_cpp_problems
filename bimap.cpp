#include <stdexcept>
#include <optional>
#include <map>
#include <iostream>
#include <string>

using namespace std;

template <typename Key1, typename Key2, typename Value>
class BiMap {
private:
    std::map<std::pair<std::optional<Key1>, std::optional<Key2>>, Value> mp;
    std::map<Key1, std::optional<Key2>> v1;
    std::map<Key2, std::optional<Key1>> v2;

public:
    // Вставить значение, указав один или оба ключа.
    // Генерирует исключение std::invalid_argument("") в случае,
    // если оба ключа пусты, либо один из ключей уже имеется в хранилище.
    void Insert(const std::optional<Key1>& key1, const std::optional<Key2>& key2,
        const Value& value) {
        if (!key1.has_value() && !key2.has_value()) {
            throw std::invalid_argument("");
            return;
        }
        if (key1.has_value()) {
            if (key2.has_value()) {
                if (mp.find({key1.value(), key2.value()}) != mp.end()) {
                    throw std::invalid_argument("");
                    return;
                }
                if (v1.find(key1.value()) != v1.end()) {
                    throw std::invalid_argument("");
                    return;
                }
                if (v2.find(key2.value()) != v2.end()) {
                    throw std::invalid_argument("");
                    return;
                }
            } else {
                if (v1.find(key1.value()) != v1.end()) {
                    throw std::invalid_argument("");
                    return;
                }
            }
        } else {
            if (key2.has_value()) {
                if (v2.find(key2.value()) != v2.end()) {
                    throw std::invalid_argument("");
                    return;
                }
            }
        }
        if (key1.has_value()) {
            if (key2.has_value()) {
                v1[key1.value()] = key2.value();
                v2[key2.value()] = key1.value();
                mp[{key1.value(), key2.value()}] = value;
            } else {
                v1[key1.value()] = std::nullopt;
                mp[{key1.value(), std::nullopt}] = value;
            }
        } else {
            if (key2.has_value()) {
                v2[key2.value()] = std::nullopt;
                mp[{std::nullopt, key2.value()}] = value;
            }
        }
    }

    // Получить значение по ключу первого типа.
    // Генерирует исключение std::out_of_range("")
    // в случае отсутствия ключа (как и функция at в std::map).
    Value& GetByPrimaryKey(const Key1& key) {
        auto it = v1.find(key);
        if (it == v1.end()) {
            throw std::out_of_range("");
        } else {
            if (it->second.has_value()) {
                return mp.at({key, it->second.value()});
            } else {
                return mp.at({key, std::nullopt});
            }
        }
    }
    const Value& GetByPrimaryKey(const Key1& key) const {
        auto it = v1.find(key);
        if (it == v1.end()) {
            throw std::out_of_range("");
        } else {
            if (it->second.has_value()) {
                return mp.at({key, it->second.value()});
            } else {
                return mp.at({key, std::nullopt});
            }
        }
    }

    // Аналогичная функция для ключа второго типа.
    Value& GetBySecondaryKey(const Key2& key) {
        auto it = v2.find(key);
        if (it == v2.end()) {
            throw std::out_of_range("");
        } else {
            if (it->second.has_value()) {
                return mp.at({it->second.value(), key});
            } else {
                return mp.at({std::nullopt, key});
            }
        }
    }
    const Value& GetBySecondaryKey(const Key2& key) const {
        auto it = v2.find(key);
        if (it == v2.end()) {
            throw std::out_of_range("");
        } else {
            if (it->second.has_value()) {
                return mp.at({it->second.value(), key});
            } else {
                return mp.at({std::nullopt, key});
            }
        }
    }
};