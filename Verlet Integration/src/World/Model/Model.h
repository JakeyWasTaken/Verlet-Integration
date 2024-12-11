#pragma once
#include "Rendering/Mesh/Mesh.h"
#include "Rendering/Shader/Shader.h"
#include <vector>
#include <stdint.h>

namespace Verlet
{
	static uint32_t GetNextModelIndex()
	{
		static uint32_t count = 0;
		return count++;
	}

	class Model
	{
	public:
		Model(const char* filePath, Shader* pShader, bool isStatic);
		~Model();

		void Draw(glm::mat4 transform, glm::vec3 color, Camera* camera);
		void Draw(glm::mat4 transform, Camera* camera);

		inline Shader* GetShader() { return m_shader; };
		inline std::vector<Texture*>* GetLoadedTextures() { return &m_loadedTextures; };
		inline bool IsStatic() { return m_isStatic; };

		inline void AddMesh(Mesh* mesh) { m_meshes.push_back(mesh); };

	private:
		uint32_t m_id = GetNextModelIndex();
		bool m_isStatic;

		std::vector<Mesh*> m_meshes;
		std::vector<Texture*> m_loadedTextures;
		Shader* m_shader;
	};
}