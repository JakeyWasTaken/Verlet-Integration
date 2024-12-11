#include "Texture.h"
#include "File/File.h"
#include "Core/Log.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Vendor/stb_image.h"

namespace Verlet
{
	Texture::Texture(const char* filePath, bool flipTexture, TextureType textureType)
	{
		m_filePath = std::string(filePath);
		m_flipTexture = flipTexture;
		m_textureType = textureType;

		// Create mesh texture
		MeshTexture* meshTexture = new MeshTexture;
		meshTexture->id = -1;
		meshTexture->filePath = m_filePath;
		meshTexture->type = textureType;

		if (textureType == Diffuse)
			meshTexture->typeStr = "texture_diffuse";
		else if (textureType == Specular)
			meshTexture->typeStr = "texture_specular";
		else if (textureType == Normal)
			meshTexture->typeStr = "texture_normal";

		m_meshTexture = meshTexture;

		LoadTexture(true);
	}

	void Texture::LoadTexture(bool failFatally)
	{
		std::string filePath = FileLoader::GetFilePath(m_filePath);

		GLuint textureId;
		glGenTextures(1, &textureId);

		m_meshTexture->id = textureId;
		m_textureId = textureId;

		int width, height, nOfComponents;
		stbi_set_flip_vertically_on_load(m_flipTexture);

		unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nOfComponents, 0);

		if (!data)
		{
			stbi_image_free(data);

			if (failFatally)
			{
				VT_FATAL("Failed to load texture at path:\n{0}", m_filePath.c_str());
				exit(-1);
			}

			VT_ERROR("Failed to load texture at path:\n{0}", m_filePath.c_str());
			return;
		}

		GLenum texFormat;
		if (nOfComponents == 1)
			texFormat = GL_RED;
		else if (nOfComponents == 3)
			texFormat = GL_RGB;
		else if (nOfComponents == 4)
			texFormat = GL_RGBA;
		else
		{
			if (failFatally)
			{
				VT_FATAL("Unknown texture format, component count: {0}", nOfComponents);
				exit(-1);
			}

			VT_ERROR("Unknown texture format, component count: {0}", nOfComponents);
			return;
		}

		// Bind to texture, assign data and generate mip-maps
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, texFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D); 

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Unbind from texture
		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(data);
	}
}