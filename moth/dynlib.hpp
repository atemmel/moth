#pragma once
#include <string>

#ifdef _WIN32
	#include <windows.h>
#endif

#ifdef _WIN32
	#define MOTH_EXPORT_DYNLIB __declspec(dllexport)
#else
	#define MOTH_EXPORT_DYNLIB
#endif

namespace Moth {

#ifdef _WIN32
	using DynlibHandle = HINSTANCE;
#else
	using DynlibHandle = void*;
#endif

	
auto openLib(const std::string& lib) -> DynlibHandle;

auto closeLib(DynlibHandle handle) -> void;

auto getAddrOfFn(DynlibHandle handle, const std::string& fnName) -> void*;

auto okHandle(DynlibHandle handle) -> bool;

}
