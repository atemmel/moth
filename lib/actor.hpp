#pragma once

#include "dynlib.hpp"
#include <type_traits>

namespace moth {

struct Actor {
	Actor() {};
	virtual ~Actor() {};

	virtual auto update() -> void {};
	virtual auto draw() -> void {};

	Id kind;
};

using CreateActorFn = Actor* (*)();
using DestroyActorFn = void (*)(const Actor*);

}

#define ExportActorHppMacro(ACTOR_IMPL) \
	static_assert(std::is_base_of<moth::Actor, ACTOR_IMPL>(), \
		"ExportActor requires that the class inherits from Actor"); \
	\
	extern "C" MOTH_EXTERN auto create_##ACTOR_IMPL() -> moth::Actor*; \
	\
	extern "C" MOTH_EXTERN auto destroy_##ACTOR_IMPL(moth::Actor* a) -> void;

#define ExportActorCppMacro(ACTOR_IMPL) \
	static_assert(std::is_base_of<moth::Actor, ACTOR_IMPL>(), \
		"ExportActor requires that the class inherits from Actor"); \
	\
	extern "C" auto create_##ACTOR_IMPL() -> moth::Actor* { \
		return new (ACTOR_IMPL)(); \
	} \
	\
	extern "C" auto destroy_##ACTOR_IMPL(moth::Actor* a) -> void { \
		delete a; \
	}
