#include "Scene.h"

namespace Verlet
{
	void Scene::Draw(Camera* pCamera)
	{
		for (uint32_t objectIndex = 0; objectIndex < m_objects.size(); objectIndex++)
		{
			m_objects[objectIndex]->Draw(pCamera);
		}
	}
}