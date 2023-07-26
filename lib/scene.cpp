#include "scene.hpp"

auto moth::Scene::update() -> void {
	updateAllActors();
}

auto moth::Scene::draw() -> void {
	drawAllActors();
}

auto moth::Scene::updateAllActors() -> void {
	for(auto& actor : actors) {
		actor->update();
	}
}

auto moth::Scene::drawAllActors() -> void {
	for(auto& actor : actors) {
		actor->draw();
	}
}
