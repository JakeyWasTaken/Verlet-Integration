#include "Mesh.h"
#include "glm/glm.hpp"
#include "Core/Log.h"
#include "vector"

#define Z_VEC3 glm::vec3(0.0f, 0.0f, 1.0f)
#define Y_VEC3 glm::vec3(0.0f, 1.0f, 0.0f)

#define SET_VERTEX_DATA(index, size, datatype, member) ::glEnableVertexAttribArray(index);\
::glVertexAttribPointer(index, size, datatype, GL_FALSE, sizeof(MeshVertex), (void*)offsetof(MeshVertex, member));

namespace Verlet
{
	Mesh::Mesh(std::vector<MeshVertex> vertices, std::vector<uint32_t> indices, Material* material, bool isStatic, uint32_t flags)
	{
		m_vertices = vertices;
		m_indices = indices;

		m_material = material;

		m_flags = flags;
		
		SetStatic(isStatic);
		SolveMeshSize();

		VT_INFO("Mesh size: {0}, {1}, {2}", m_size.x, m_size.y, m_size.z);

		// I have to do this otherwise it shouts at me
		m_VAO = NULL;
		m_VBO = NULL;
		m_EBO = NULL;

		SetupBuffers();
	}

	// This always does smooth shading
	void Mesh::SolveNormals()
	{
		// Set each normal to <0,0,0>
		for (uint32_t index = 0; index < m_vertices.size(); index++)
		{
			m_vertices[index].Normal = glm::vec3(0.0f, 0, 0);
		}

		for (uint32_t indiciesIndex = 0; indiciesIndex < m_indices.size(); indiciesIndex += 3)
		{
			uint32_t indiceA = m_indices[indiciesIndex];
			uint32_t indiceB = m_indices[indiciesIndex + 1];
			uint32_t indiceC = m_indices[indiciesIndex + 2];

			MeshVertex A = m_vertices[indiceA];
			MeshVertex B = m_vertices[indiceB];
			MeshVertex C = m_vertices[indiceC];

			glm::vec3 p = glm::cross(B.Position - A.Position, C.Position - A.Position);
			A.Normal += p;
			B.Normal += p;
			C.Normal += p;
		}

		// Normalize normals
		for (uint32_t index = 0; index < m_vertices.size(); index++)
		{
			MeshVertex vertex = m_vertices[index];
			vertex.Normal = glm::normalize(vertex.Normal);
		}
	}

	void Mesh::SolveMeshSize()
	{
		glm::vec3 min = glm::vec3(FLT_MAX);
		glm::vec3 max = glm::vec3(-FLT_MAX);

		for (uint32_t i = 0; i < m_vertices.size(); i++)
		{
			MeshVertex vertex = m_vertices[i];
			min = glm::min(min, vertex.Position);
			max = glm::max(max, vertex.Position);
		}

		m_size = (max - min) / 2.0f;
	}

	void Mesh::UpdateMeshData(std::vector<MeshVertex> vertices, std::vector<uint32_t> indices, bool recomputeNormals)
	{
		m_vertices = vertices;
		m_indices = indices;

		if (recomputeNormals)
			SolveNormals();
	}

	void Mesh::SendMeshData()
	{
		if (m_vertices.size() == 0)
			return;

		// Send data to gpu
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(MeshVertex), &m_vertices[0], m_drawType);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint32_t),
			&m_indices[0], GL_STATIC_DRAW);

		// Declare the vertex attributes
		
		// Position
		SET_VERTEX_DATA(0, 3, GL_FLOAT, Position);
		// Normal
		SET_VERTEX_DATA(1, 3, GL_FLOAT, Normal);
		// Vertex Color
		SET_VERTEX_DATA(2, 3, GL_FLOAT, VertexColor);
		// TexCoords
		SET_VERTEX_DATA(3, 2, GL_FLOAT, TexCoords);

		glBindVertexArray(0);
	}
	
	void Mesh::Draw(Camera* camera)
	{
		m_material->PreDraw(camera);

		if (m_indices.size() == 0)
			return;

		bool isDoubleSided = (m_flags & MESH_FLAG_DOUBLE_SIDED) != 0;
		bool isWireframe = (m_flags & MESH_FLAG_WIREFRAME) != 0;

		if (isDoubleSided)
			glDisable(GL_CULL_FACE);
		else
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);
		}

		// Draw Mesh
		glPolygonMode(
			GL_FRONT_AND_BACK,
			isWireframe ? GL_LINE : GL_FILL
		);

		glBindVertexArray(m_VAO);
		glDrawElements(
			isWireframe ? GL_LINES : GL_TRIANGLES, 
			m_indices.size(), 
			GL_UNSIGNED_INT, 
			0
		);
		glBindVertexArray(0);

	}

	void Mesh::SetupBuffers()
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		VT_TRACE("Generated mesh buffers, VAO: {0}, VBO: {1}, EBO: {2}", m_VAO, m_VBO, m_EBO);
		
		SendMeshData();
	}
}