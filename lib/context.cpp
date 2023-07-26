#include "context.hpp"
#include "common.hpp"
#include <iostream>

moth::Context moth::context {};

auto moth::ctx() -> Context& {
	return context;
}

auto moth::changeScene(Scene* scene) -> void {
	context.nextScene = scene;
}
