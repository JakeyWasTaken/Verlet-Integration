#include "Object.h"
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/euler_angles.hpp>

namespace Verlet
{
	Object::Object(Model* pModel)
	{
		m_model = pModel;
	}

	glm::mat4 Object::GetObjectMatrix()
	{
		glm::mat4 matrix(1.0f);
		matrix = glm::translate(matrix, position);
		matrix *= glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);
		matrix = glm::scale(matrix, scale);

		return matrix;
	}

	void Object::Draw(Camera* camera)
	{
		m_model->Draw(GetObjectMatrix(), color, camera);
	}
}
