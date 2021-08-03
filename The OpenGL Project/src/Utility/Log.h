#pragma once

#include "Utility/Base.h"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)


	//logging class
	class Log
	{
	public:
		static void Init();
		static void Shutdown();

		//getters
		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

// Core log macros
#define OPENGLPROJECT_CORE_TRACE(...)		::Log::getCoreLogger()->trace(__VA_ARGS__)
#define OPENGLPROJECT_CORE_INFO(...)		::Log::getCoreLogger()->info(__VA_ARGS__)
#define OPENGLPROJECT_CORE_WARN(...)		::Log::getCoreLogger()->warn(__VA_ARGS__)
#define OPENGLPROJECT_CORE_ERROR(...)		::Log::getCoreLogger()->error(__VA_ARGS__)
#define OPENGLPROJECT_CORE_CRITICAL(...)   ::Log::getCoreLogger()->critical(__VA_ARGS__)
										
// Client log macros					
#define OPENGLPROJECT_TRACE(...)			::Log::getClientLogger()->trace(__VA_ARGS__)
#define OPENGLPROJECT_INFO(...)			::Log::getClientLogger()->info(__VA_ARGS__)
#define OPENGLPROJECT_WARN(...)			::Log::getClientLogger()->warn(__VA_ARGS__)
#define OPENGLPROJECT_ERROR(...)			::Log::getClientLogger()->error(__VA_ARGS__)
#define OPENGLPROJECT_FATAL(...)			::Log::getClientLogger()->critical(__VA_ARGS__)