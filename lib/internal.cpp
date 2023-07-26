#include "internal.hpp"
#include "moth.hpp"

#include <chrono>
#include <iostream>

auto moth::internal::init() -> void {
	ctx().startTimestamp = moth::now();

	SDL_Init(SDL_INIT_VIDEO);
	ctx().window = SDL_CreateWindow(
		"My Window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_SHOWN);
	ctx().renderer = SDL_CreateRenderer(
		ctx().window,
		-1,
		SDL_RENDERER_ACCELERATED);
}

auto moth::internal::free() -> void {
	SDL_DestroyRenderer(ctx().renderer);
	SDL_DestroyWindow(ctx().window);
	SDL_Quit();
}

auto moth::internal::lives() -> bool {
	SDL_Event e{};
	while(SDL_PollEvent(&e)) {
		if(e.type == SDL_QUIT) {
			return false;
		}
	}
	return true;
}

auto moth::internal::update() -> void {
	if(ctx().nextScene != nullptr) {
		ctx().currentScene = std::move(ctx().nextScene);
		ctx().nextScene = nullptr;
	}
	if(ctx().currentScene != nullptr) {
		ctx().currentScene->update();
	}
}

auto moth::internal::draw() -> void {
	ctx().currentScene->draw();
}

auto moth::internal::display() -> void {
	SDL_RenderPresent(ctx().renderer);
}
