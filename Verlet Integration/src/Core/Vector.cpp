#include "Vector.h"

glm::vec3 SafeNormalize(glm::vec3 vector)
{
	if (glm::length(vector) == 0)
		return VEC3_ZERO;

	return glm::normalize(vector);
}