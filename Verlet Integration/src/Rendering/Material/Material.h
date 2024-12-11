#pragma once
#include "Rendering/Material/Texture/Texture.h"
#include "Rendering/Shader/Shader.h"
#include "Rendering/Camera/Camera.h"
#include <vector>

namespace Verlet
{
	class Material
	{
	public:
		Material(std::vector<Texture*> textures, Shader* pShader);

		void PreDraw(Camera* camera);

		inline Shader* GetShader() { return m_shader; };
	private:
		std::vector<Texture*> m_textures;
		Shader* m_shader;
	};
}

