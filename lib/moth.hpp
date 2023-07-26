#pragma once
#include "actor.hpp"
#include "color.hpp"
#include "rect.hpp"
#include "serialize.hpp"

namespace Moth {

auto init() -> void;

auto free() -> void;

auto lives() -> bool;

auto timeAlive() -> float;

auto update() -> void;

auto draw() -> void;

auto draw(Rect rect, Color color) -> void;

auto clear(Color color = Color{0, 0, 0, 0,}) -> void;

auto display() -> void;

}
