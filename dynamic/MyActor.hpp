#pragma once
#include "../moth/moth.hpp"

#include <cmath>
#include <iostream>

class MyActor : public Moth::Actor {

	auto update() -> void override {
	}

	auto draw() -> void override {
		const float x = std::cos(Moth::timeAlive() * 0.02f) * 64;
		const float y = std::sin(Moth::timeAlive() * 0.02f) * 64;

		const auto rect = Moth::Rect{
			.x = 300.f + x,
			.y = 200.f + y,
			.w = 64.f,
			.h = 64.f,
		};

		const auto color = Moth::Color{
			.r = 255,
			.g = 0,
			.b = 0,
			.a = 255,
		};

		Moth::draw(rect, color);
	}
};

ExportActorHppMacro(MyActor);
