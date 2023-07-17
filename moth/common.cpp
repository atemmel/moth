#include "common.hpp"
#include <cassert>

auto unreachable() -> void {
	assert(false);
}

auto unimplemented() -> void {
	assert(false);
}
