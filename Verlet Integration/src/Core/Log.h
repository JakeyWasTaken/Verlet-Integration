#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Verlet
{
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; };

	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};
}

#if VT_DEBUG

// Verlet log macros
#define VT_FATAL(...)		::Verlet::Log::GetLogger()->critical(__VA_ARGS__)
#define VT_ERROR(...)		::Verlet::Log::GetLogger()->error(__VA_ARGS__)
#define VT_WARN(...)		::Verlet::Log::GetLogger()->warn(__VA_ARGS__)
#define VT_INFO(...)		::Verlet::Log::GetLogger()->info(__VA_ARGS__)
#define VT_TRACE(...)		::Verlet::Log::GetLogger()->trace(__VA_ARGS__)

#else // VT_RELEASE

//  Verlet log macros
#define VT_FATAL
#define VT_ERROR
#define VT_WARN
#define VT_INFO
#define VT_TRACE

#endif