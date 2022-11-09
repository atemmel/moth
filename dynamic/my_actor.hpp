#pragma once
#include "../common/actor.hpp"

#include <cstdio>

class MyActor : public Actor {
	auto update() -> void override {
		printf("Update from MyActor\n");
	}

	auto draw() -> void override {
		printf("Draw from MyActor\n");
	}
};

ExportActorMacro(MyActor)
