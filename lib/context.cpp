#include "context.hpp"
#include "common.hpp"
#include <iostream>

Moth::Context Moth::context {};

auto Moth::ctx() -> Context& {
	return context;
}

auto Moth::changeScene(Scene* scene) -> void {
	context.nextScene = scene;
}
