#include "Material.h"
#include "Core/Log.h"
#include "Core/Time.h"
#include "World/Lighting/Lighting.h"
#include "glad/glad.h"
#include <string>

namespace Verlet
{
	Material::Material(std::vector<Texture*> textures, Shader* pShader)
	{
		m_textures.insert(m_textures.end(), textures.begin(), textures.end());
		m_shader = pShader;
	}

	void Material::PreDraw(Camera* camera)
	{
		uint32_t indices[TextureType::NumOfTextureTypes] = {0};

		for (uint32_t i = 0; i < m_textures.size(); i++)
		{
			Texture* texture = m_textures[i];
			MeshTexture* mTexture = texture->GetMeshTexture();

			std::string number = std::to_string(indices[mTexture->type]++);
			std::string name = mTexture->typeStr;

			m_shader->SetInt("material." + name + number, i);

			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mTexture->id);
		}

		glActiveTexture(GL_TEXTURE0);

		m_shader->UseShader();

		m_shader->SetVec3("ambient", Lighting::AmbientColor);
		m_shader->SetVec3("sunPosition", Lighting::SunPosition);
		m_shader->SetVec3("cameraPosition", camera->GetPosition());

		m_shader->SetMat4("projection", camera->GetProjectionMatrix());
		m_shader->SetMat4("view", camera->GetViewMatrix());
	}
}