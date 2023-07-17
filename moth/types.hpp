#pragma once
#include <cstddef>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <cstdint>

template<typename T>
using Vector = std::vector<T>;

using String = std::string;

using StringView = std::string_view;

template<typename K, typename V>
using Map = std::unordered_map<K, V>;

template <typename T>
using Set = std::set<T>;

using Byte = uint8_t;
using Index = std::size_t;
