#pragma once
#include "World/Object/Object.h"
#include "Rendering/Camera/Camera.h"
#include <vector>

namespace Verlet
{
	class Scene
	{
	public:
		Scene() = default;

		inline void PushObject(Object* pObject) { m_objects.push_back(pObject); };

		void Draw(Camera* pCamera);

	private:
		std::vector<Object*> m_objects;
	};
}

