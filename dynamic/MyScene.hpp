#pragma once
#include "MyActor.hpp"
#include "../lib/context.hpp"
#include "../lib/scene.hpp"
#include "../lib/common.hpp"

struct MyScene : public Moth::Scene {
	MyActor* actorA;
	MyActor* actorB;

	MyScene() {
		actorA = spawn<MyActor>();
		actorB = spawn<MyActor>();

		check(actorA != nullptr);
		check(actorB != nullptr);

		actorA->phi = 0.f;
		actorB->phi = 2.f;
	}

};

ExportSceneHppMacro(MyScene)
