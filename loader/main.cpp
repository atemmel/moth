#include "../lib/moth.hpp"
#include "../lib/dynlib.hpp"
#include "watcher.hpp"

#include <cstdio>
#include <cmath>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>
#include <thread>
#include <unistd.h>

//const std::string hppPath = "../dynamic/MyActor.hpp";
//const std::string libPath = "../dynamic/moth_dynlib.so";
const std::string hppPath = "../dynamic/MyActor.hpp";
const std::string libPath = (std::filesystem::current_path() / "MyActor.dll").string();

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
	std::cout << "Trying to load " <<  lib << '\n';
	auto handle = Moth::openLib(lib);
	if(!handle) {
		return ActorDynlib{};
	}

	auto createStr = getDynlibFnStr(hpp, "create_");
	auto destroyStr = getDynlibFnStr(hpp, "destroy_");

	auto create = (Moth::CreateActorFn)Moth::getAddrOfFn(handle, createStr.c_str());
	auto destroy = (Moth::DestroyActorFn)Moth::getAddrOfFn(handle, destroyStr.c_str());
	
	std::cout << createStr << " : " << destroyStr <<  '\n';
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
		std::cout << "Could not reload handle " << Moth::dynlibError() << '\n';
		return false;
	}
	*actor = lib.create();
	return true;
}

bool reloading = false;
bool badThread = false;

auto main() -> int {
	if(isatty(fileno(stdin)) != 0) {
		std::cout << "Loader run as a standalone process, exiting...\n";
		return 4;
	}
	const auto tickRate = std::chrono::milliseconds(16);

	recompile();
	auto actorDynlib = loadActorDynlib(libPath, hppPath);
	if(!actorDynlib.ok()) {
		std::cout << "Could not open handle " << Moth::dynlibError() << '\n';
		return 1;
	}

	auto watcher = FsWatcher{};
	watcher.watch(hppPath);

	auto actor = actorDynlib.create();

	std::cout << "Waiting to read...\n";

	Vector<char> buffer;
	buffer.resize(2048);
	auto thread = std::thread([&](){
READ_STDIN:
		//std::cout << "Reading stdin..." << std::endl;
		/*
		auto addr = ::fgets(buffer.data(), buffer.size(), stdin);
		if(addr == nullptr) {
			std::cout << "Whoopdiedoo\n";
			return;
		}
		*/
		String msg;
		std::getline(std::cin, msg, '\n');
		//auto msg = StringView(addr);
		std::cout << msg << std::endl;
		if(msg == "reload\n") {
			reloading = true;
			if(!reload(actorDynlib, &actor)) {
				badThread = true;
				return;
			} else {
				reloading = false;
			}
		}
		std::this_thread::sleep_for(tickRate);
		goto READ_STDIN;
	});
	thread.detach();

	std::cout << "Moth init" << std::endl;
	Moth::init();
	while(Moth::lives() && !badThread) {
		std::cout << "New iter" << std::endl;
		/*
		if(watcher.hasChanged() && !reload(actorDynlib, &actor)) {
			return 2;
		}
		*/
		if(reloading) {
			std::cout << "Skipped..." << std::endl;
		} else {
			actor->update();
			Moth::clear();
			actor->draw();
			Moth::display();
		}
		std::this_thread::sleep_for(tickRate);
	}
	std::cout << "Exiting..." << std::endl;
	Moth::free();
	actorDynlib.destroy(actor);
	actorDynlib.free();
	//thread.join();
}
