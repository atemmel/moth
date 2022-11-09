#include "../common/actor.hpp"
#include "watcher.hpp"

#include <dlfcn.h>

#include <chrono>
#include <iostream>
#include <string_view>
#include <thread>

constexpr auto headerPath = std::string_view("../dynamic/my_actor.hpp");
constexpr auto libPath = std::string_view("../dynamic/my_actor.so");

auto recompile() -> void {
	system("cd ../dynamic && ./make_dyn.sh my_actor_tmp.so");
	system("cd ../dynamic && mv -f my_actor_tmp.so my_actor.so");
}

struct ActorDynLib {
	void* handle = nullptr;
	CreateActorFn create = nullptr;
	DestroyActorFn destroy = nullptr;

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

auto loadActorDynLib(std::string_view path) -> ActorDynLib {
	auto handle = dlopen(path.data(), RTLD_NOW | RTLD_LOCAL);
	if(!handle) {
		return ActorDynLib{};
	}
	auto create = (CreateActorFn)dlsym(handle, "create");
	auto destroy = (DestroyActorFn)dlsym(handle, "destroy");
	
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

auto main() -> int {

	auto actorDynLib = loadActorDynLib(libPath);
	if(!actorDynLib.ok()) {
		std::cerr << "Could not open handle " << dlerror() << '\n';
		return 1;
	}

	const auto tickRate = std::chrono::milliseconds(1000);
	auto watcher = FsWatcher{};
	watcher.watch(headerPath);

	auto actor = actorDynLib.create();

	while(true) {
		if(watcher.hasChanged()) {

			actorDynLib.destroy(actor);
			actorDynLib.free();
			recompile();

			actorDynLib = loadActorDynLib(libPath);
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
