#pragma once

#include <type_traits>

namespace Moth {

class Actor {
public:
	Actor() {};
	virtual ~Actor() {};

	virtual auto update() -> void = 0;
	virtual auto draw() -> void = 0;
};


using CreateActorFn = Actor* (*)();
using DestroyActorFn = void (*)(const Actor*);

}

#define ExportActorHppMacro(ACTOR_IMPL) \
	static_assert(std::is_base_of<Moth::Actor, ACTOR_IMPL>(), \
		"ExportActor requires that the class inherits from Actor"); \
	\
	extern "C" auto create_##ACTOR_IMPL() -> Moth::Actor*; \
	\
	extern "C" auto destroy_##ACTOR_IMPL(Moth::Actor* a) -> void;

#define ExportActorCppMacro(ACTOR_IMPL) \
	static_assert(std::is_base_of<Moth::Actor, ACTOR_IMPL>(), \
		"ExportActor requires that the class inherits from Actor"); \
	\
	extern "C" auto create_##ACTOR_IMPL() -> Moth::Actor* { \
		return new (ACTOR_IMPL)(); \
	} \
	\
	extern "C" auto destroy_##ACTOR_IMPL(Moth::Actor* a) -> void { \
		delete a; \
	}
