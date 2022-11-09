#pragma once

#include <type_traits>

class Actor {
public:
	Actor() {};
	virtual ~Actor() {};

	virtual auto update() -> void = 0;
	virtual auto draw() -> void = 0;
};

using CreateActorFn = Actor* (*)();
using DestroyActorFn = void(*)(const Actor*);

#define ExportActorMacro(ACTOR_IMPL) \
	static_assert(std::is_base_of<Actor, ACTOR_IMPL>(), \
		"ExportActor requires that the class inherits from Actor"); \
	\
	extern "C" auto create() -> Actor* { \
		return new (ACTOR_IMPL)(); \
	} \
	\
	extern "C" auto destroy(const Actor* a) -> void { \
		delete a; \
	}
