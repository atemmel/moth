#pragma once
#include "moth.hpp"
#include "dynlib.hpp"

namespace moth::internal {

auto init() -> void;

auto free() -> void;

auto lives() -> bool;

auto timeAlive() -> float;

auto update() -> void;

auto draw() -> void;

auto display() -> void;

}
