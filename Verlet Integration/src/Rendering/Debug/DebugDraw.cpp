#include "DebugDraw.h"
#include "Config.h"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>

void GetMat4Components(glm::mat4 matrix, glm::vec3& position, glm::vec3& up, glm::vec3& right, glm::vec3& forward)
{
	position = glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]);
	right = glm::vec3(matrix[0][0], matrix[0][1], matrix[0][2]);
	up = glm::vec3(matrix[1][0], matrix[1][1], matrix[1][2]);
	forward = glm::vec3(matrix[2][0], matrix[2][1], matrix[2][2]);
}

namespace Verlet
{
	Shader* grDebugDraw::pShader;
	Material* grDebugDraw::pMaterial;
	Mesh* grDebugDraw::pMeshDrawer;

	std::vector<MeshVertex> grDebugDraw::vertices;
	std::vector<uint32_t> grDebugDraw::indices;

	void grDebugDraw::Init()
	{
		std::vector<Texture*> textures;

		pShader = new Shader(CONF_DEBUG_VERT_SHADER, CONF_DEBUG_FRAG_SHADER);
		pMaterial = new Material(textures, pShader);
		pMeshDrawer = new Mesh(vertices, indices, pMaterial, false, MESH_FLAG_WIREFRAME);
	}

	void grDebugDraw::ClearDrawList()
	{
		vertices.clear();
		indices.clear();
	}

	void grDebugDraw::Draw(Camera* camera)
	{
		pShader->UseShader();
		pShader->SetMat4("model", glm::mat4(1.0f));

		pMeshDrawer->UpdateMeshData(vertices, indices, false);
		pMeshDrawer->SendMeshData();

		pMeshDrawer->Draw(camera);
	}

	void grDebugDraw::Line(glm::vec3 p0, glm::vec3 p1, glm::vec3 color0, glm::vec3 color1)
	{
		uint32_t start = vertices.size();

		MeshVertex vertexA;
		MeshVertex vertexB;

		vertexA.Position = p0;
		vertexA.VertexColor = color0;

		vertexB.Position = p1;
		vertexB.VertexColor = color1;

		vertices.push_back(vertexA);
		vertices.push_back(vertexB);

		indices.push_back(start);
		indices.push_back(start + 1);
	}

	void grDebugDraw::Line(glm::vec3 p0, glm::vec3 p1, glm::vec3 color)
	{
		Line(p0, p1, color, color);
	}

	void grDebugDraw::Poly(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 color)
	{
		Line(p0, p1, color);
		Line(p1, p2, color);
		Line(p2, p0, color);
	}

	void grDebugDraw::Quad(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color)
	{
		Poly(p0, p1, p3, color);
		Poly(p0, p2, p3, color);
	}

	void grDebugDraw::Circle(glm::mat4 transform, float radius, uint32_t subdivisions, float angle, bool reconnect, glm::vec3 color)
	{
		float anglePerChunk = floor(angle / subdivisions);

		glm::vec3 transformPosition;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 forward;

		GetMat4Components(transform, transformPosition, up, right, forward);

		glm::vec3 previousPosition = transformPosition;
		glm::vec3 firstVertex = transformPosition;

		float finishingAngle = 0.0f;

		for (float i = 0; i <= angle; i += anglePerChunk)
		{
			float xScale = cos(glm::radians(i)) * radius;
			float yScale = sin(glm::radians(i)) * radius;

			glm::vec3 vertexPosition = transformPosition + (up * yScale) + (right * xScale);

			if (i == 0)
			{
				previousPosition = vertexPosition;
				firstVertex = vertexPosition;
				finishingAngle = i;
			}

			Line(previousPosition, vertexPosition, color);
			previousPosition = vertexPosition;
			finishingAngle = i;
		}
	
		if (finishingAngle != angle)
		{
			float xScale = cos(glm::radians(angle)) * radius;
			float yScale = sin(glm::radians(angle)) * radius;

			glm::vec3 vertexPosition = transformPosition + (up * yScale) + (right * xScale);

			Line(previousPosition, vertexPosition, color);
			previousPosition = vertexPosition;
		}

		if (reconnect)
			Line(previousPosition, firstVertex);
	}

	void grDebugDraw::Circle(glm::mat4 transform, float radius, uint32_t subdivisions, glm::vec3 color)
	{
		Circle(transform, radius, subdivisions, 360.0f, false, color);
	}

	void grDebugDraw::Sphere(glm::mat4 transform, float radius, uint32_t subdivisions, glm::vec3 color)
	{
		glm::vec3 transformPosition;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 forward;

		GetMat4Components(transform, transformPosition, up, right, forward);

		constexpr float rad90 = glm::radians(90.0f);
		Circle(transform, radius, subdivisions, color);

		float anglePerChunk = floor(360.0f / fmin(15, subdivisions));

		for (float i = 0; i <= 360.0f; i += anglePerChunk)
		{
			float xScale = cos(glm::radians(i)) * radius;
			float yScale = sin(glm::radians(i)) * radius;

			Circle(
				glm::translate(transform, glm::vec3(1, 0, 0) * xScale) * glm::eulerAngleXYZ(0.0f, rad90, 0.0f),
				abs(yScale),
				subdivisions, 
				color
			);

			Circle(
				glm::translate(transform, glm::vec3(0, 1, 0) * yScale) * glm::eulerAngleXYZ(rad90, 0.0f, 0.0f),
				abs(xScale),
				subdivisions,
				color
			);
		}
	}

	void grDebugDraw::Plane(glm::mat4 transform, glm::vec2 size, glm::vec3 color)
	{
		glm::vec3 transformPosition;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 forward;

		GetMat4Components(transform, transformPosition, up, right, forward);

		glm::vec3 topLeft = transformPosition + (up * size.y) + (-right * size.x);
		glm::vec3 topRight = transformPosition + (up * size.y) + (right * size.x);

		glm::vec3 btmLeft = transformPosition + (-up * size.y) + (-right * size.x);
		glm::vec3 btmRight = transformPosition + (-up * size.y) + (right * size.x);

		Quad(topLeft, topRight, btmLeft, btmRight, color);
	}

	void grDebugDraw::Box(glm::mat4 transform, glm::vec3 size, glm::vec3 color)
	{
		glm::vec3 transformPosition;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 forward;

		GetMat4Components(transform, transformPosition, up, right, forward);

		glm::vec3 p0 = transformPosition + (-forward * size) + (up * size) + (-right * size);
		glm::vec3 p1 = transformPosition + (-forward * size) + (up * size) + (right * size);
		glm::vec3 p2 = transformPosition + (forward * size) + (up * size) + (-right * size);
		glm::vec3 p3 = transformPosition + (forward * size) + (up * size) + (right * size);
		Quad(p0, p1, p2, p3, color);

		p0 = transformPosition + (-forward * size) + (-up * size) + (-right * size);
		p1 = transformPosition + (-forward * size) + (-up * size) + (right * size);
		p2 = transformPosition + (forward * size) + (-up * size) + (-right * size);
		p3 = transformPosition + (forward * size) + (-up * size) + (right * size);
		Quad(p0, p1, p2, p3, color);

		p0 = transformPosition + (forward * size) + (up * size) + (-right * size);
		p1 = transformPosition + (forward * size) + (up * size) + (right * size);
		p2 = transformPosition + (forward * size) + (-up * size) + (-right * size);
		p3 = transformPosition + (forward * size) + (-up * size) + (right * size);
		Quad(p0, p1, p2, p3, color);

		p0 = transformPosition + (-forward * size) + (up * size) + (-right * size);
		p1 = transformPosition + (-forward * size) + (up * size) + (right * size);
		p2 = transformPosition + (-forward * size) + (-up * size) + (-right * size);
		p3 = transformPosition + (-forward * size) + (-up * size) + (right * size);
		Quad(p0, p1, p2, p3, color);

		p0 = transformPosition + (-forward * size) + (up * size) + (-right * size);
		p1 = transformPosition + (-forward * size) + (-up * size) + (-right * size);
		p2 = transformPosition + (forward * size) + (up * size) + (-right * size);
		p3 = transformPosition + (forward * size) + (-up * size) + (-right * size);
		Quad(p0, p1, p2, p3, color);

		p0 = transformPosition + (-forward * size) + (up * size) + (right * size);
		p1 = transformPosition + (-forward * size) + (-up * size) + (right * size);
		p2 = transformPosition + (forward * size) + (up * size) + (right * size);
		p3 = transformPosition + (forward * size) + (-up * size) + (right * size);
		Quad(p0, p1, p2, p3, color);
	}
}