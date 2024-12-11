#include "TextureConstructor.h"
// #include <algorithm>

namespace Verlet
{
	namespace TextureConstructor
	{
		Texture* CreateTexture(const char* filePath, bool flipTexture, TextureType textureType)
		{
			// Iterate all textures and try find one that matches the specified parameters
			/*for (uint32_t i = 0; i < LoadedTextures.size(); i++)
			{
				Texture* texture = LoadedTextures[i];

				if (texture->GetFilePath() == filePath 
					&& texture->GetIsFlipped() == flipTexture 
					&& texture->GetTextureType() == textureType)
				{
					return texture;
				}
			}*/
			
			Texture* texture = new Texture(filePath, flipTexture, textureType);

			//LoadedTextures.push_back(texture);
			return texture;
		}

		/*void RemoveTexture(Texture* texture)
		{
			auto Iterator = std::find(LoadedTextures.begin(), LoadedTextures.end(), texture);

			if (Iterator != LoadedTextures.end())
			{
				LoadedTextures.erase(Iterator);
			}

			delete texture;
		}*/
	}
}