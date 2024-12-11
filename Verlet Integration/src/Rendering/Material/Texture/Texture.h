#pragma once
#include <iostream>
#include <vector>
#include "glad/glad.h"

namespace Verlet
{
	enum TextureType
	{
		Diffuse,
		Specular,
		Normal,
		NumOfTextureTypes,
	};

	// Should this go in Mesh?
	struct MeshTexture {
		GLuint id;
		TextureType type;
		std::string typeStr;
		std::string filePath;
	};

	// Never call the destructor/constructor on this directly use the TextureConstructor namespace
	class Texture
	{
	public:
		Texture(const char* filePath, bool flipTexture, TextureType textureType);

		void LoadTexture(bool failFatally);

		inline GLuint GetId() { return m_textureId; };
		inline MeshTexture* GetMeshTexture() { return m_meshTexture; };
		inline TextureType GetTextureType() { return m_textureType; };
		inline std::string GetFilePath() { return m_filePath; };
		inline bool GetIsFlipped() { return m_flipTexture; };
	private:
		MeshTexture* m_meshTexture;
		TextureType m_textureType;
		
		// We dont store a texture unit as thats set during material pre-render
		GLuint m_textureId;

		std::string m_filePath;
		bool m_flipTexture;
	};
}

