#pragma once
#include "../moth/moth.hpp"

#include <cmath>

struct MyActor : public Moth::Actor {

	int i, j;
	std::string name;

	SERIALIZE(MyActor, i, j, name);

	MyActor() 
		: i(1), j(2), name("gamers") {

	}

	auto update() -> void override {
		std::cout << (*this) << '\n';
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
