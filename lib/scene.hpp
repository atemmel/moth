#pragma once
#include "actor.hpp"

namespace Moth {

struct Scene {
	Vector<Ptr<Actor>> actors;

	Scene() {};
	virtual ~Scene() {};

	virtual auto update() -> void;
	virtual auto draw() -> void;

	template<typename AnActor>
	auto spawn() -> AnActor* {
		auto actor = new AnActor();
		actors.emplace_back(actor);
		return actor;
	}

	auto updateAllActors() -> void;
	auto drawAllActors() -> void;
};

using CreateSceneFn = Scene* (*)();
using DestroySceneFn = void (*)(const Scene*);

}
#define ExportSceneHppMacro(SCENE_IMPL) \
	static_assert(std::is_base_of<Moth::Scene, SCENE_IMPL>(), \
		"ExportScene requires that the class inherits from Scene"); \
	\
	extern "C" MOTH_EXTERN auto create_##SCENE_IMPL() -> Moth::Scene*; \
	\
	extern "C" MOTH_EXTERN auto destroy_##SCENE_IMPL(Moth::Scene* a) -> void;

#define ExportSceneCppMacro(SCENE_IMPL) \
	static_assert(std::is_base_of<Moth::Scene, SCENE_IMPL>(), \
		"ExportScene requires that the class inherits from Scene"); \
	\
	extern "C" auto create_##SCENE_IMPL() -> Moth::Scene* { \
		return new (SCENE_IMPL)(); \
	} \
	\
	extern "C" auto destroy_##SCENE_IMPL(Moth::Scene* a) -> void { \
		delete a; \
	}
