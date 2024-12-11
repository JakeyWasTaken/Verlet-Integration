#pragma once
#include "glm/glm.hpp"
#include "World/Model/Model.h"

namespace Verlet
{
	class Object
	{
	public:
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 rotation = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);

		glm::vec3 color = glm::vec3(1.0f);

		Object(Model* pModel);

		inline Model* GetModel() { return m_model; };

	private:
		Model* m_model;
	};
}
