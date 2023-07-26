#pragma once
#include "scene.hpp"
#include "actor.hpp"
#include "vendor.hpp"
#include "types.hpp"

namespace Moth {

struct Context;

extern Context context;

struct Context {
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	Scene* currentScene = nullptr;
	Scene* nextScene = nullptr;
	uint64_t startTimestamp = 0;
};

auto ctx() -> Context&;

auto changeScene(Scene* scene) -> void;

}
