#pragma once
#include "actor.hpp"
#include "color.hpp"
#include "rect.hpp"

namespace Moth {

auto init() -> void;

auto free() -> void;

auto lives() -> bool;

auto timeAlive() -> float;

auto draw(Rect rect, Color color) -> void;

auto clear() -> void;

auto display() -> void;


struct Ctx;

auto context() -> Ctx&;
	
}
