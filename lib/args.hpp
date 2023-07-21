#pragma once
#include "types.hpp"

auto flag(bool* value, StringView flag);
auto flag(String* value, StringView flag);

auto parse(const Vector<String> &args) -> void;

auto rest() -> const Vector<String>&;
