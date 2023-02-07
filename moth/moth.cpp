#include "moth.hpp"
#include "color.hpp"
#include "context.hpp"

#include <chrono>
#include <iostream>

static Moth::Ctx ctx;

static auto now() -> uint64_t {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()
		.time_since_epoch())
		.count();
}

auto Moth::init() -> void {
	ctx.startTimestamp = now();

	SDL_Init(SDL_INIT_VIDEO);
	ctx.window = SDL_CreateWindow(
		"My Window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_SHOWN);
	ctx.renderer = SDL_CreateRenderer(
		ctx.window,
		-1,
		SDL_RENDERER_ACCELERATED);
}

auto Moth::free() -> void {
	SDL_DestroyRenderer(ctx.renderer);
	SDL_DestroyWindow(ctx.window);
	SDL_Quit();
}

auto Moth::lives() -> bool {
	SDL_Event e{};
	while(SDL_PollEvent(&e)) {
		if(e.type == SDL_QUIT) {
			return false;
		}
	}
	return true;
}

auto Moth::timeAlive() -> float {
	auto t = now();
	auto t2 = static_cast<float>(t - ctx.startTimestamp);
	return t2;
}

auto Moth::draw(Rect rect, Color color) -> void {
	//TODO: rounding conversion
	auto rectImpl = SDL_Rect{
		.x = rect.x,
		.y = rect.y,
		.w = rect.w,
		.h = rect.h,
	};

	SDL_SetRenderDrawColor(ctx.renderer,
			color.r,
			color.g,
			color.b,
			color.a);

	SDL_RenderFillRect(ctx.renderer,
			&rectImpl);
}

auto Moth::clear(Moth::Color color) -> void {
    SDL_SetRenderDrawColor(ctx.renderer,
			color.r,
			color.g,
			color.b,
			color.a);
	SDL_RenderClear(ctx.renderer);
}

auto Moth::display() -> void {
	SDL_RenderPresent(ctx.renderer);
}

auto Moth::context() -> Ctx& {
	return ctx;
}
