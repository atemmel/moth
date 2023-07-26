#pragma once
#include "actor.hpp"
#include "color.hpp"
#include "common.hpp"
#include "context.hpp"
#include "rect.hpp"
#include "scene.hpp"
#include "serialize.hpp"
#include "types.hpp"
#include "vendor.hpp"
#include <cstdint>

namespace moth {

auto now() -> uint64_t;

auto timeAlive() -> float;

auto draw(Rect rect, Color color) -> void;

auto clear(Color color = Color{0, 0, 0, 0,}) -> void;

}
