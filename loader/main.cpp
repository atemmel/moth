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

struct ActorDynlib {
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

auto getDynlibFnStr(const std::string& hppname, const std::string& prefix) -> std::string {
	size_t last = hppname.find_last_of('.');
	size_t first = hppname.find_last_of('/') + 1;
	auto base = hppname.substr(first, last - first);
	return prefix + base;
}

auto loadActorDynlib(const std::string& lib, const std::string& hpp) -> ActorDynlib {
	std::cerr << "Trying to load " <<  lib << '\n';
	auto handle = Moth::openLib(lib);
	if(!handle) {
		return ActorDynlib{};
	}

	auto createStr = getDynlibFnStr(hpp, "create_");
	auto destroyStr = getDynlibFnStr(hpp, "destroy_");

	auto create = (Moth::CreateActorFn)Moth::getAddrOfFn(handle, createStr.c_str());
	auto destroy = (Moth::DestroyActorFn)Moth::getAddrOfFn(handle, destroyStr.c_str());
	
	std::cerr << createStr << " : " << destroyStr <<  '\n';
	if(create == nullptr || destroy == nullptr) {
		Moth::closeLib(handle);
		return ActorDynlib{};
	}

	return ActorDynlib{
		.handle = handle,
		.create = create,
		.destroy = destroy,
	};
}

auto reload(ActorDynlib& lib, Moth::Actor** actor) -> bool {
	lib.destroy(*actor);
	lib.free();
	recompile();
	lib = loadActorDynlib(libPath, hppPath);
	if(!lib.ok()) {
		std::cerr << "Could not reload handle " << Moth::dynlibError() << '\n';
		return false;
	}
	*actor = lib.create();
	return true;
}

auto main() -> int {
	const auto tickRate = std::chrono::milliseconds(16);

	recompile();
	auto actorDynlib = loadActorDynlib(libPath, hppPath);
	if(!actorDynlib.ok()) {
		std::cerr << "Could not open handle " << Moth::dynlibError() << '\n';
		return 1;
	}

	auto watcher = FsWatcher{};
	watcher.watch(hppPath);

	auto actor = actorDynlib.create();

	Moth::init();
	while(Moth::lives()) {
		if(watcher.hasChanged() && !reload(actorDynlib, &actor)) {
			return 2;
		}
		actor->update();
		Moth::clear();
		actor->draw();
		Moth::display();
		std::this_thread::sleep_for(tickRate);
	}
	Moth::free();
	actorDynlib.destroy(actor);
	actorDynlib.free();
}
