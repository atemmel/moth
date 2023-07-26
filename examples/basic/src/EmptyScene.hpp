#pragma once
#include <moth.hpp>

struct EmptyScene : public moth::Scene {
	EmptyScene() {
	}

	auto update() -> void override {
		moth::draw({64.f, 64.f, 64.f, 64.f}, {255, 0, 0, 255});
	}
};

ExportSceneHppMacro(EmptyScene)
