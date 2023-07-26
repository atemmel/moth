#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

template<typename T>
using Vector = std::vector<T>;

using String = std::string;

using StringView = std::string_view;

template<typename K, typename V>
using Map = std::unordered_map<K, V>;

template<typename T>
using Set = std::set<T>;

template<typename T>
using Ptr = std::unique_ptr<T>;

using Byte = uint8_t;
using ByteArray = Vector<Byte>;
using Index = std::size_t;
using Id = uint64_t;
