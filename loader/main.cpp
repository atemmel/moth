#include "context.hpp"
#include <internal.hpp>

#include <cstdio>
#include <cmath>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <mutex>
#include <string>
#include <string_view>
#include <thread>

constexpr auto tickRate = std::chrono::milliseconds(16);

std::string baseScene;
std::string libPath;
moth::Scene* scene;
Set<String> registeredScenes;

//auto recompile() -> void {
	//system("cd ../dynamic && ./make_dyn.sh moth_dynlib_tmp.so");
	//system("cd ../dynamic && mv -f moth_dynlib_tmp.so moth_dynlib.so");
//}

struct SceneFns {
	moth::CreateSceneFn create = nullptr;
	moth::DestroySceneFn destroy = nullptr;
};

struct SceneDynlib {
	moth::internal::DynlibHandle handle = {0};
	Map<String, SceneFns> scenes;

	auto ok() const -> bool {
		return moth::internal::okHandle(handle);
	}

	auto free() -> void {
		scenes[baseScene].destroy(scene);
		moth::internal::closeLib(handle);
		handle = {0};
		scenes.clear();
	}
};

SceneDynlib dynlib;
std::mutex libmutex;

auto getDynlibFnStr(const std::string& hppname, const std::string& prefix) -> std::string {
	size_t last = hppname.find_last_of('.');
	size_t first = hppname.find_last_of('/') + 1;
	auto base = hppname.substr(first, last - first);
	return prefix + base;
}

auto loadSceneDynlib(const std::string& lib, Set<String> scenes) -> SceneDynlib {
	if(moth::context.currentScene != nullptr) {
		moth::context.currentScene = nullptr;
	}
	std::cerr << "Trying to load " <<  lib << '\n';
	SceneDynlib newDynlib;
	newDynlib.handle = moth::internal::openLib(lib);
	if(!newDynlib.ok()) {
		std::cerr << "Error: " << moth::internal::dynlibError() << '\n';
		return SceneDynlib{};
	}


	for(const auto& sceneName : scenes) {
		auto createStr = "create_" + sceneName;
		auto destroyStr = "destroy_" + sceneName;

		auto create = (moth::CreateSceneFn)moth::internal::getAddrOfFn(newDynlib.handle, createStr.c_str());
		auto destroy = (moth::DestroySceneFn)moth::internal::getAddrOfFn(newDynlib.handle, destroyStr.c_str());
		
		std::cout << createStr << " : " << destroyStr <<  '\n';
		if(create == nullptr || destroy == nullptr) {
			std::cout << "Not found\n";
			newDynlib.free();
			return SceneDynlib{};
		}

		std::cout << "Found\n";
		newDynlib.scenes.insert({sceneName, {create, destroy}});
	}
	return newDynlib;
}

auto instantiateScene() -> void {
	scene = dynlib.scenes[baseScene].create();
	moth::context.currentScene = scene;
}

auto reload() -> bool {
	std::cout << dynlib.ok() << '\n';
	dynlib.free();
	dynlib = loadSceneDynlib(libPath, registeredScenes);
	if(!dynlib.ok()) {
		std::cerr << "Could not reload handle " << moth::internal::dynlibError() << '\n';
		return false;
	}
	instantiateScene();
	return true;
}

bool reloading = false;
bool shouldDie = false;

auto ipcThread() -> void {
		String msg;
READ_STDIN:
		if(shouldDie) {
			return;
		}
		std::getline(std::cin, msg, '\n');
		if(shouldDie) {
			return;
		}
		std::scoped_lock lock(libmutex);
		std::cout << msg << std::endl;
		if(msg.starts_with("reload")) {
			reloading = true;
		} else if(msg.starts_with("register:")) {

		}
		goto READ_STDIN;
}

auto main(int argc, char** argv) -> int {
	if(argc < 2) {
		std::cerr << "Base scene not specified, exiting..." << std::endl;
		return 4;
	}

	baseScene = argv[1];
	libPath = (std::filesystem::current_path() / "build/shared.dll").string();
	std::cout << "libPath: '" << libPath << '\'' << std::endl;
	std::cout << "baseScene: '" << baseScene << '\'' << std::endl;

	registeredScenes.insert(baseScene);

	dynlib = loadSceneDynlib(libPath, registeredScenes);
	if(!dynlib.ok()) {
		std::cerr << "Could not open handle " << moth::internal::dynlibError() << '\n';
		return 1;
	}

	instantiateScene();

	std::cout << "Waiting to read...\n";

	auto thread = std::thread(&ipcThread);
	thread.detach();

	std::cout << "moth init" << std::endl;
	moth::internal::init();
	while(moth::internal::lives() && !shouldDie) {
		std::scoped_lock lock(libmutex);
		if(reloading) {
			if(!reload()) {
				shouldDie = true;
			} 
			reloading = false;
		} else {
			moth::internal::update();
			moth::clear();
			moth::internal::draw();
			moth::internal::display();
		}
		std::this_thread::sleep_for(tickRate);
	}
	shouldDie = true;
	std::cout << "Freeing scene\n";
	dynlib.free();
	std::cout << "Internal free scene\n";
	moth::internal::free();
}
