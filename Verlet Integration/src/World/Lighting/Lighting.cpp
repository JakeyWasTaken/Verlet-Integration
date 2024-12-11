#include "Lighting.h"

namespace Verlet
{
	glm::vec3 Lighting::SkyColor = glm::vec3(0.0f);
	glm::vec3 Lighting::AmbientColor = glm::vec3(0.2745f);

	glm::vec3 Lighting::SunPosition = glm::vec3(1.0f) * 1000.0f;
}