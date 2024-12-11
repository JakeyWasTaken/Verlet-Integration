#include "Model.h"
#include "World/Model/Loader/ModelLoader.h"

namespace Verlet
{
	Model::Model(const char* filePath, Shader* pShader, bool isStatic)
	{
		m_shader = pShader;
		m_isStatic = isStatic;

		ModelLoader::LoadFileIntoModel(filePath, this);
	}

	Model::~Model()
	{
		// delete stuff here!
	}

	void Model::Draw(glm::mat4 transform, glm::vec3 color, Camera* camera)
	{
		m_shader->UseShader();
		m_shader->SetMat4("model", transform);
		m_shader->SetVec3("color", color);

		for (uint32_t i = 0; i < m_meshes.size(); i++)
		{
			m_meshes[i]->Draw(camera);
		}
	}

	void Model::Draw(glm::mat4 transform, Camera* camera)
	{
		Draw(transform, glm::vec3(1.0f), camera);
	}
}