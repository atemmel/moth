#pragma once

#define ExportClassHppMacro(BASE, IMPL, MSG) \
	static_assert(std::is_base_of<BASE, IMPL>(), MSG); \
	\
	extern "C" MOTH_EXTERN auto create_##IMPL() -> BASE*; \
	\
	extern "C" MOTH_EXTERN auto destroy_##IMPL(BASE* a) -> void;

#define ExportClassCppMacro(BASE, IMPL, MSG) \
	static_assert(std::is_base_of<BASE, IMPL>(), MSG); \
	\
	extern "C" auto create_##IMPL() -> BASE* { \
		return new (IMPL)(); \
	} \
	\
	extern "C" auto destroy_##IMPL(BASE* a) -> void { \
		delete a; \
	}

#define ExportClassMacro(BASE, IMPL, MSG) \
	ExportClassHppMacro(BASE, IMPL, MSG) \
	ExportClassCppMacro(BASE, IMPL, MSG)
