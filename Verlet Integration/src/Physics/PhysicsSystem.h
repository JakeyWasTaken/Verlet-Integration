/*
* // THEORY \\
* 
* The Physics system is entirely for simulating point masses constrainted to one another that can collide
* IT DOES NOT interact with any meshes / models etc.
* 
* This system aims to implement XPBD in a robust and extendable way
*/

#pragma once
#include "PhysicsObject.h"
#include "glm/glm.hpp"
#include "stdint.h"
#include "Core/Time.h"
#include "Config.h"
#include <vector>

namespace Verlet
{
	namespace Physics
	{
		struct SystemSettings
		{
			uint32_t substeps = 8;
			float updateRate = 1.0f / 120.0f;
			float velocityDamper = 0.005f;
			float gravity = 9.8f; // m/s^2
		};

		class phSystem
		{
		public:
			SystemSettings settings;
			bool isSimulationPaused = false;
			
			void Update();

#if DEBUG_FEATURES
			// ImGui and Debug Draw
			void DrawDebug();
#endif

			inline bool ReadyToUpdate() { return m_accumulatedDelta >= settings.updateRate; };
			inline void IncrementAccumulator(float delta) { m_accumulatedDelta += delta; };

			inline void AddObject(phObject* object) { m_objects.push_back(object); };

		private:
			std::vector<phObject*> m_objects;

#if DEBUG_FEATURES
			std::vector<phObject*> m_exploringObjects;
#endif
			float m_accumulatedDelta = 0.0f;
		};
	}
}

