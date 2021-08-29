#pragma once

#include <memory>

#include "OpenGLProject/Utility/PlatformDetection.h"

#ifdef OPENGLPROJECT_DEBUG
#if defined(OPENGLPROJECT_PLATFORM_WINDOWS)
#define OPENGLPROJECT_DEBUGBREAK() __debugbreak()
#elif defined(OPENGLPROJECT_PLATFORM_LINUX)
#include <signal.h>
#define OPENGLPROJECT_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define OPENGLPROJECT_ENABLE_ASSERTS
#else
#define OPENGLPROJECT_DEBUGBREAK()
#endif

#define OPENGLPROJECT_EXPAND_MACRO(x) x
#define OPENGLPROJECT_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define OPENGLPROJECT_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

template<typename T>
using Scope = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

#include "OpenGLProject/Utility/Log.h"
#include "OpenGLProject/Utility/Assert.h"