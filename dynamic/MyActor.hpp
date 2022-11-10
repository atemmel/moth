#pragma once
#include "../common/actor.hpp"

#include <iostream>

class MyActor : public Actor {

	auto update() -> void override {
		std::cout << "Update from MyActor\n";
	}

	auto draw() -> void override {
		std::cout << "Draw from MyActor\n";
	}
};

ExportActorHppMacro(MyActor);
