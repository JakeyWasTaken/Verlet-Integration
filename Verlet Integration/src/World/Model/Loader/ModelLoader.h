#pragma once
#include "World/Model/Model.h"
#include "Rendering/Material/Texture/TextureConstructor.h"
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define PROCESSING_FLAGS aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals

namespace Verlet
{
	namespace ModelLoader
	{
		extern void LoadFileIntoModel(const char* rawFilePath, Model* model);
		extern void ProcessNode(aiNode* node, const aiScene* scene, Model* model);
		extern Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, Model* model);
		extern std::vector<Texture*> LoadMaterialTextures(aiMaterial* material, aiTextureType type, TextureType texType, Model* model);
	}
}

