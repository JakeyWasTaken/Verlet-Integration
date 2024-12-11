#pragma once
#include "glm/glm.hpp"
#include "glad/glad.h"
#include "Rendering/Material/Material.h"
#include "Rendering/Camera/Camera.h"
#include <iostream>
#include "vector"

#define MESH_MODIFIER_WIREFRAME    0x00000001
#define MESH_MODIFIER_DOUBLE_SIDED 0x00000010 // Unused

namespace Verlet
{
	struct MeshVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 VertexColor = glm::vec3(1.0f);
		glm::vec2 TexCoords;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<MeshVertex> vertices, std::vector<uint32_t> indices, Material* material, bool isStatic, uint32_t modifiers = 0);

		inline GLuint GetVAO() { return m_VAO; };
		inline GLuint GetVBO() { return m_VBO; };
		inline GLuint GetEBO() { return m_EBO; };
		inline Material* GetMaterial() { return m_material; };
		inline void SetStatic(bool isStatic) { m_drawType = isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW; };

		void SolveNormals();

		void UpdateMeshData(std::vector<MeshVertex> vertices, std::vector<uint32_t> indices, bool recomputeNormals);
		void SendMeshData();

		void Draw(Camera* camera);
	private:
		GLenum m_drawType;
		GLuint m_VAO, m_VBO, m_EBO;

		Material* m_material;

		std::vector<MeshVertex> m_vertices;
		std::vector<uint32_t> m_indices;

		uint32_t m_modifiers;

		void SetupBuffers();
	};
}