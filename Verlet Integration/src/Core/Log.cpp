#include "Log.h"

namespace Verlet
{
	std::shared_ptr<spdlog::logger> Log::s_Logger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_Logger = spdlog::stdout_color_mt("VERLET");
		s_Logger->set_level(spdlog::level::trace);

		VT_INFO("Initialized logger");
	}
}