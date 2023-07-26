#pragma once
#include "../lib/moth.hpp"

#include <cmath>

struct MyActor : public moth::Actor {

	float phi;
	const float dphi = 0.02f;

	SERIALIZE(MyActor,
		phi);

	MyActor() 
		: phi(-3.14565f / 2.f) {
	}

	auto update() -> void override {
		//std::cout << (*this) << '\n';
	}

	auto draw() -> void override {

		phi += dphi;

		const float x = std::cos(phi) * 64;
		const float y = std::sin(phi) * 64;

		const auto rect = moth::Rect{
			.x = 300.f + x,
			.y = 200.f + y,
			.w = 64.f,
			.h = 64.f,
		};

		const auto color = moth::Color{
			.r = 255,
			.g = 0,
			.b = 255,
			.a = 255,
		};

		moth::draw(rect, color);
	}
};

ExportActorHppMacro(MyActor);
