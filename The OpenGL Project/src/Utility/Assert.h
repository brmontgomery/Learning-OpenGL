#pragma once

#include "Utility/Base.h"
#include "Utility/Log.h"
#include <filesystem>

#ifdef OPENGLPROJECT_ENABLE_ASSERTS

// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define OPENGLPROJECT_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { OPENGLPROJECT##type##ERROR(msg, __VA_ARGS__); OPENGLPROJECT_DEBUGBREAK(); } }
#define OPENGLPROJECT_INTERNAL_ASSERT_WITH_MSG(type, check, ...) OPENGLPROJECT_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define OPENGLPROJECT_INTERNAL_ASSERT_NO_MSG(type, check) OPENGLPROJECT_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", OPENGLPROJECT_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define OPENGLPROJECT_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define OPENGLPROJECT_INTERNAL_ASSERT_GET_MACRO(...) OPENGLPROJECT_EXPAND_MACRO( OPENGLPROJECT_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, OPENGLPROJECT_INTERNAL_ASSERT_WITH_MSG, OPENGLPROJECT_INTERNAL_ASSERT_NO_MSG) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define OPENGLPROJECT_ASSERT(...) OPENGLPROJECT_EXPAND_MACRO( OPENGLPROJECT_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define OPENGLPROJECT_CORE_ASSERT(...) OPENGLPROJECT_EXPAND_MACRO( OPENGLPROJECT_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define OPENGLPROJECT_ASSERT(...)
#define OPENGLPROJECT_CORE_ASSERT(...)
#endif