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

#define ExportActorHppMacro(ACTOR_IMPL) \
	static_assert(std::is_base_of<Actor, ACTOR_IMPL>(), \
		"ExportActor requires that the class inherits from Actor"); \
	\
	extern "C" auto create_##ACTOR_IMPL() -> Actor*; \
	\
	extern "C" auto destroy_##ACTOR_IMPL(Actor* a) -> void;

#define ExportActorCppMacro(ACTOR_IMPL) \
	static_assert(std::is_base_of<Actor, ACTOR_IMPL>(), \
		"ExportActor requires that the class inherits from Actor"); \
	\
	extern "C" auto create_##ACTOR_IMPL() -> Actor* { \
		return new (ACTOR_IMPL)(); \
	} \
	\
	extern "C" auto destroy_##ACTOR_IMPL(Actor* a) -> void { \
		delete a; \
	}
