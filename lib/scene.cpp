#include "scene.hpp"

auto Moth::Scene::update() -> void {
	updateAllActors();
}

auto Moth::Scene::draw() -> void {
	drawAllActors();
}

auto Moth::Scene::updateAllActors() -> void {
	for(auto& actor : actors) {
		actor->update();
	}
}

auto Moth::Scene::drawAllActors() -> void {
	for(auto& actor : actors) {
		actor->draw();
	}
}
