#pragma once
#include "types.hpp"

template<typename P>
auto as(void* ptr) -> P* {
	return static_cast<P*>(ptr);
}

template<typename K, typename  V>
auto has(Map<K, V>& map, const K& key) -> V* {
	auto it = map.find(key);
	if(it == map.end()) {
		return nullptr;
	}
	return &(it->second);
}

auto unreachable() -> void;

auto unimplemented() -> void;
