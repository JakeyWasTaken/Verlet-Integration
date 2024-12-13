#include "Random.h"
#include "Time.h"
#include <cstdlib>

#define RANDOM_FLOAT_PRECISION 3 // Decimal place precision
#define RANDOM_FLOAT_EXPONENT 10 ^ RANDOM_FLOAT_PRECISION

namespace Verlet
{
	uint32_t Random::RandomUInt()
	{
		srand((uint32_t)Time::Time);

		return rand();
	}

	float Random::RandomFloat(float min, float max)
	{
		int value = RandomUInt() % RANDOM_FLOAT_EXPONENT;
		float fValue = value / RANDOM_FLOAT_EXPONENT;

		return min + (max - min) * fValue;
	}
}