#include "../lib/moth.hpp"
#include "../lib/context.hpp"
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

constexpr auto tickRate = std::chrono::milliseconds(16);

const std::string hppPath = "../dynamic/MyScene.hpp";

std::string libPath;

auto recompile() -> void {
	std::cout << "Attempt to recompile" << std::endl;
	//system("cd ../dynamic && ./make_dyn.sh moth_dynlib_tmp.so");
	//system("cd ../dynamic && mv -f moth_dynlib_tmp.so moth_dynlib.so");
}

struct SceneDynlib {
	Moth::DynlibHandle handle = {0};
	Moth::CreateSceneFn create = nullptr;
	Moth::DestroySceneFn destroy = nullptr;

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

SceneDynlib dynlib;
Moth::Scene* scene;

auto getDynlibFnStr(const std::string& hppname, const std::string& prefix) -> std::string {
	size_t last = hppname.find_last_of('.');
	size_t first = hppname.find_last_of('/') + 1;
	auto base = hppname.substr(first, last - first);
	return prefix + base;
}

auto loadSceneDynlib(const std::string& lib, const std::string& hpp) -> SceneDynlib {
	std::cerr << "Trying to load " <<  lib << '\n';
	auto handle = Moth::openLib(lib);
	if(!handle) {
		return SceneDynlib{};
	}

	auto createStr = getDynlibFnStr(hpp, "create_");
	auto destroyStr = getDynlibFnStr(hpp, "destroy_");

	auto create = (Moth::CreateSceneFn)Moth::getAddrOfFn(handle, createStr.c_str());
	auto destroy = (Moth::DestroySceneFn)Moth::getAddrOfFn(handle, destroyStr.c_str());
	
	std::cout << createStr << " : " << destroyStr <<  '\n';
	if(create == nullptr || destroy == nullptr) {
		Moth::closeLib(handle);
		return SceneDynlib{};
	}

	return SceneDynlib{
		.handle = handle,
		.create = create,
		.destroy = destroy,
	};
}

auto reload() -> bool {
	dynlib.destroy(scene);
	dynlib.free();
	recompile();
	dynlib = loadSceneDynlib(libPath, hppPath);
	if(!dynlib.ok()) {
		std::cerr << "Could not reload handle " << Moth::dynlibError() << '\n';
		return false;
	}
	scene = dynlib.create();
	return true;
}

bool reloading = false;
bool shouldDie = false;

auto ipcThread() -> void {
		String msg;
READ_STDIN:
		std::getline(std::cin, msg, '\n');
		std::cout << msg << std::endl;
		if(msg == "reload\n") {
			reloading = true;
			if(!reload()) {
				shouldDie = true;
				return;
			} else {
				reloading = false;
			}
		}
		goto READ_STDIN;
}

auto main(int argc, char** argv) -> int {
	if(argc < 2) {
		std::cerr << "DLL to load not specified, exiting..." << std::endl;
		return 4;
	}

	libPath = argv[1];
	std::cout << "libPath: '" << libPath << '\'' << std::endl;

	recompile();
	auto sceneDynlib = loadSceneDynlib(libPath, hppPath);
	if(!sceneDynlib.ok()) {
		std::cerr << "Could not open handle " << Moth::dynlibError() << '\n';
		return 1;
	}

	auto watcher = FsWatcher{};
	watcher.watch(hppPath);

	scene =sceneDynlib.create();

	std::cout << "Waiting to read...\n";

	auto thread = std::thread(&ipcThread);
	thread.detach();

	std::cout << "Moth init" << std::endl;
	Moth::init();
	Moth::context.currentScene = scene;
	while(Moth::lives() && !shouldDie) {
		if(reloading) {
			std::cout << "Skipped..." << std::endl;
		} else {
			Moth::update();
			scene->update();
			Moth::clear();
			scene->draw();
			Moth::display();
		}
		std::this_thread::sleep_for(tickRate);
	}
	shouldDie = true;
	Moth::free();
	sceneDynlib.destroy(scene);
	sceneDynlib.free();
}
