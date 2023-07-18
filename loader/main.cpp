#include "../moth/moth.hpp"
#include "../moth/dynlib.hpp"
#include "watcher.hpp"

#include <cmath>
#include <chrono>
#include <iostream>
#include <string>
#include <string_view>
#include <thread>

//const std::string hppPath = "../dynamic/MyActor.hpp";
//const std::string libPath = "../dynamic/moth_dynlib.so";
const std::string hppPath = "../dynamic/MyActor.hpp";
const std::string libPath = "MyActor.dll";

auto recompile() -> void {
	system("cd ../dynamic && ./make_dyn.sh moth_dynlib_tmp.so");
	system("cd ../dynamic && mv -f moth_dynlib_tmp.so moth_dynlib.so");
}

struct ActorDynLib {
	Moth::DynlibHandle handle = {0};
	Moth::CreateActorFn create = nullptr;
	Moth::DestroyActorFn destroy = nullptr;

	auto ok() const -> bool {
		return Moth::okHandle(handle);
	}

	auto free() -> void {
		Moth::closeLib(handle);
		handle = {0};
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
	auto handle = Moth::openLib(lib);
	if(!handle) {
		return ActorDynLib{};
	}

	auto createStr = getDynLibFnStr(hpp, "create_");
	auto destroyStr = getDynLibFnStr(hpp, "destroy_");

	auto create = (Moth::CreateActorFn)Moth::getAddrOfFn(handle, createStr.c_str());
	auto destroy = (Moth::DestroyActorFn)Moth::getAddrOfFn(handle, destroyStr.c_str());
	
	std::cerr << createStr << " : " << destroyStr <<  '\n';
	if(create == nullptr || destroy == nullptr) {
		Moth::closeLib(handle);
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
		//std::cerr << "Could not reload handle " << dlerror() << '\n';
		std::cerr << "Could not reload handle\n";
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
		//std::cerr << "Could not open handle " << dlerror() << '\n';
		std::cerr << "Could not open handle\n";
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
		Moth::display();
		std::this_thread::sleep_for(tickRate);
	}
	Moth::free();
	actorDynLib.destroy(actor);
	actorDynLib.free();
}
