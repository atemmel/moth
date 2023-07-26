#include "moth.hpp"
#include "color.hpp"
#include "context.hpp"

#include <chrono>
#include <iostream>
#include <SDL2/SDL.h>

auto moth::now() -> uint64_t {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()
		.time_since_epoch())
		.count();
}

auto moth::timeAlive() -> float {
	auto t = now();
	auto t2 = static_cast<float>(t - ctx().startTimestamp);
	return t2;
}

auto moth::draw(Rect rect, Color color) -> void {
	//TODO: rounding conversion
	auto rectImpl = SDL_Rect{
		.x = static_cast<int>(rect.x),
		.y = static_cast<int>(rect.y),
		.w = static_cast<int>(rect.w),
		.h = static_cast<int>(rect.h),
	};

	SDL_SetRenderDrawColor(ctx().renderer,
			color.r,
			color.g,
			color.b,
			color.a);

	SDL_RenderFillRect(ctx().renderer,
			&rectImpl);
}

auto moth::clear(moth::Color color) -> void {
    SDL_SetRenderDrawColor(ctx().renderer,
			color.r,
			color.g,
			color.b,
			color.a);
	SDL_RenderClear(ctx().renderer);
}
