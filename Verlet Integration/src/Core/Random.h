#pragma once
#include <stdint.h>

namespace Verlet
{
	class Random
	{
	public:
		static uint32_t RandomUInt();
		static float RandomFloat(float min = 0, float max = 1);
	};
}