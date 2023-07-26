#include "common.hpp"
#include <cassert>

auto check(bool condition) -> void {
	assert(condition);
}

auto unreachable() -> void {
	assert(false);
}

auto unimplemented() -> void {
	assert(false);
}
