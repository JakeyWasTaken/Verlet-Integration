#pragma once
#include "Rendering/Mesh/Mesh.h"
#include "Rendering/Shader/Shader.h"
#include "Rendering/Material/Material.h"
#include "Rendering/Camera/Camera.h"
#include "glm/glm.hpp"
#include <vector>

#define VEC3_ONE glm::vec3(1.0f)

namespace Verlet
{
	namespace grDebugDraw
	{
		extern Shader* pShader;
		extern Material* pMaterial;
		extern Mesh* pMeshDrawer;

		extern std::vector<MeshVertex> vertices;
		extern std::vector<uint32_t> indices;

		// TODO work out implementation specifics of this
		extern bool enabled;

		extern void Init();
		extern void ClearDrawList();
		extern void Draw(Camera* camera);

		// Drawing Methods

		extern void Line(glm::vec3 p0, glm::vec3 p1, glm::vec3 color0, glm::vec3 color1);
		extern void Line(glm::vec3 p0, glm::vec3 p1, glm::vec3 color = VEC3_ONE);

		extern void Poly(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 color = VEC3_ONE);
		
		extern void Quad(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color = VEC3_ONE);

		extern void Circle(glm::mat4 transform, float radius, uint32_t subdivisions, float angle, bool reconnect, glm::vec3 color = VEC3_ONE);
		extern void Circle(glm::mat4 transform, float radius, uint32_t subdivisions, glm::vec3 color = VEC3_ONE);
	
		extern void Sphere(glm::mat4 transform, float radius, uint32_t subdivisions, glm::vec3 color = VEC3_ONE);

		extern void Plane(glm::mat4 transform, glm::vec2 size, glm::vec3 color = VEC3_ONE);

		extern void Box(glm::mat4 transform, glm::vec3 size, glm::vec3 color = VEC3_ONE);
	};
}