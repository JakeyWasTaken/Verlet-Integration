#pragma once
// #include <vector>
#include "./Texture.h"

namespace Verlet
{
	namespace TextureConstructor
	{
		//static std::vector<Texture*> LoadedTextures;

		Texture* CreateTexture(const char* filePath, bool flipTexture, TextureType textureType);
		//void RemoveTexture(Texture* texture);
	}
}
