#pragma once
#include "vendor.hpp"

namespace Moth {

struct Ctx {
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	uint64_t startTimestamp = 0;
};

}
