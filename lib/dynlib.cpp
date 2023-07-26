#include "dynlib.hpp"

#include <system_error>

#ifdef _WIN32
	#include <windows.h>
#elif __linux__
	#include <dlfcn.h>
#endif

auto Moth::openLib(const std::string& lib) -> Moth::DynlibHandle {
#ifdef _WIN32
	return LoadLibrary(lib.data());
#elif __linux__
	return dlopen(lib.c_str(), RTLD_NOW | RTLD_LOCAL);
#endif
}

auto Moth::closeLib(Moth::DynlibHandle handle) -> void {
#ifdef _WIN32
	FreeLibrary(handle);
#elif __linux__
	dlclose(handle);
#endif
}

auto Moth::getAddrOfFn(Moth::DynlibHandle handle, const std::string& fnName) -> void* {
#ifdef _WIN32
	return (void*)GetProcAddress(handle, fnName.c_str());
#elif __linux__
	return dlsym(handle, fnName.c_str());
#endif
}

auto Moth::okHandle(Moth::DynlibHandle handle) -> bool {
#ifdef _WIN32
	return handle != 0;
#elif __linux__
	return handle != nullptr;
#endif
}

auto Moth::dynlibError() -> String {
#ifdef _WIN32
	auto err = GetLastError();
	return std::system_category().message(err);
#elif __linux__
	auto err = dlerror();
	return err == nullptr ? "" : err;
#endif
}
