#pragma once
#include "../moth/moth.hpp"

#include <cmath>

struct MyActor : public Moth::Actor {

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
