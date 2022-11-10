#include "../moth/moth.hpp"
#include "watcher.hpp"

#include <dlfcn.h>

#include <cmath>
#include <chrono>
#include <iostream>
#include <string>
#include <string_view>
#include <thread>

const std::string hppPath = "../dynamic/MyActor.hpp";
const std::string libPath = "../dynamic/moth_dynlib.so";

auto recompile() -> void {
	system("cd ../dynamic && ./make_dyn.sh moth_dynlib_tmp.so");
	system("cd ../dynamic && mv -f moth_dynlib_tmp.so moth_dynlib.so");
}

struct ActorDynLib {
	void* handle = nullptr;
	Moth::CreateActorFn create = nullptr;
	Moth::DestroyActorFn destroy = nullptr;

	auto ok() const -> bool {
		return handle != nullptr;
	}

	auto free() -> void {
		dlclose(handle);
		handle = nullptr;
		create = nullptr;
		destroy = nullptr;
	}
};

auto getDynLibFnStr(const std::string& hppname, const std::string& prefix) -> std::string {
	size_t last = hppname.find_last_of('.');
	size_t first = hppname.find_last_of('/') + 1;
	auto base = hppname.substr(first, last - first);
	return prefix + base;
}

auto loadActorDynLib(const std::string& lib, const std::string& hpp) -> ActorDynLib {
	std::cerr << "Trying to load " <<  lib << '\n';
	auto handle = dlopen(lib.data(), RTLD_NOW | RTLD_LOCAL);
	if(!handle) {
		return ActorDynLib{};
	}

	auto createStr = getDynLibFnStr(hpp, "create_");
	auto destroyStr = getDynLibFnStr(hpp, "destroy_");

	auto create = (Moth::CreateActorFn)dlsym(handle, createStr.c_str());
	auto destroy = (Moth::DestroyActorFn)dlsym(handle, destroyStr.c_str());
	
	std::cerr << createStr << " : " << destroyStr <<  '\n';
	if(create == nullptr || destroy == nullptr) {
		dlclose(handle);
		return ActorDynLib{};
	}

	return ActorDynLib{
		.handle = handle,
		.create = create,
		.destroy = destroy,
	};
}

auto reload(ActorDynLib& lib, Moth::Actor** actor) -> bool {
	lib.destroy(*actor);
	lib.free();
	recompile();
	lib = loadActorDynLib(libPath, hppPath);
	if(!lib.ok()) {
		std::cerr << "Could not reload handle " << dlerror() << '\n';
		return false;
	}
	*actor = lib.create();
	return true;
}

auto main() -> int {
	const auto tickRate = std::chrono::milliseconds(16);

	recompile();
	auto actorDynLib = loadActorDynLib(libPath, hppPath);
	if(!actorDynLib.ok()) {
		std::cerr << "Could not open handle " << dlerror() << '\n';
		return 1;
	}

	auto watcher = FsWatcher{};
	watcher.watch(hppPath);

	auto actor = actorDynLib.create();

	Moth::init();
	while(Moth::lives()) {
		if(watcher.hasChanged() && !reload(actorDynLib, &actor)) {
			return 2;
		}
		actor->update();
		Moth::clear();
		actor->draw();
		//Moth::draw(rect, color);
		Moth::display();
		std::this_thread::sleep_for(tickRate);
	}
	Moth::free();
	actorDynLib.destroy(actor);
	actorDynLib.free();
}

/*
auto main() -> int {
	const auto tickRate = std::chrono::milliseconds(16);
	Moth::init();

	while(Moth::lives()) {

		const float x = std::cos(Moth::timeAlive() * 0.002f) * 16;
		const float y = std::sin(Moth::timeAlive() * 0.002f) * 16;

		const auto rect = Moth::Rect{
			.x = 130.f + x,
			.y = 130.f + y,
			.w = 64.f,
			.h = 63.f,
		};

		const auto color = Moth::Color{
			.r = 255,
			.g = 0,
			.b = 0,
			.a = 255,
		};

		Moth::clear();
		Moth::draw(rect, color);
		Moth::display();
		std::this_thread::sleep_for(tickRate);
	}

	Moth::free();
}
*/

/*
auto main() -> int {
	Moth::init();
	recompile();
	auto actorDynLib = loadActorDynLib(libPath, hppPath);
	if(!actorDynLib.ok()) {
		std::cerr << "Could not open handle " << dlerror() << '\n';
		return 1;
	}

	const auto tickRate = std::chrono::milliseconds(1000);
	auto watcher = FsWatcher{};
	watcher.watch(hppPath);

	auto actor = actorDynLib.create();

	while(true) {
		if(watcher.hasChanged()) {

			actorDynLib.destroy(actor);
			actorDynLib.free();
			recompile();

			actorDynLib = loadActorDynLib(libPath, hppPath);
			if(!actorDynLib.ok()) {
				std::cerr << "Could not reload handle " << dlerror() << '\n';
				return 2;
			}
			actor = actorDynLib.create();
		}

		actor->update();
		actor->draw();
		std::this_thread::sleep_for(tickRate);
	}

	actorDynLib.destroy(actor);
	actorDynLib.free();
}
*/
