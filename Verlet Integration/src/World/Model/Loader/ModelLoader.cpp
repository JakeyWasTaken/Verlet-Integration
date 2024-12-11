#include "ModelLoader.h"
#include "File/File.h"
#include "Core/Log.h"

#define ProcessTextures(aiType, type) ::std::vector<Texture*> type##Maps = LoadMaterialTextures(material, aiType, type, model); textures.insert(textures.end(), type##Maps.begin(), type##Maps.end());

namespace Verlet
{
	void ModelLoader::LoadFileIntoModel(const char* rawFilePath, Model* model)
	{
		std::string filePath = FileLoader::GetFilePath(rawFilePath);

		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(filePath, PROCESSING_FLAGS);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			VT_ERROR("Failed to read model file.\n{0}", import.GetErrorString());
			return;
		}

		ProcessNode(scene->mRootNode, scene, model);
	}

	void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, Model* model)
	{
		// Process all the current nodes meshes
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			model->AddMesh(ProcessMesh(mesh, scene, model));
		}

		// Process all the nodes children
		for (uint32_t i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene, model);
		}
	}

	Mesh* ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, Model* model)
	{
		std::vector<MeshVertex> vertices;
		std::vector<Texture*> textures;
		std::vector<uint32_t> indices;

		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			MeshVertex vertex;

			// Process vertex position & normal
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;

			/*vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;*/

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 texCoords;
				texCoords.x = mesh->mTextureCoords[0][i].x;
				texCoords.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = texCoords;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f);

			vertices.push_back(vertex);
		}
	
		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (uint32_t j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
	
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			ProcessTextures(aiTextureType_DIFFUSE, Diffuse);
			ProcessTextures(aiTextureType_SPECULAR, Specular);
			ProcessTextures(aiTextureType_HEIGHT, Normal);
		}

		Material* material = new Material(textures, model->GetShader());

		return new Mesh(vertices, indices, material, model->IsStatic());
	}

	std::vector<Texture*> ModelLoader::LoadMaterialTextures(aiMaterial* material, aiTextureType type, TextureType texType, Model* model)
	{
		std::vector<Texture*> textures;

		for (uint32_t i = 0; i < material->GetTextureCount(type); i++)
		{
			aiString textureFilePath;
			material->GetTexture(type, i, &textureFilePath);

			bool exists = false;

			std::vector<Texture*>* loadedTextures = model->GetLoadedTextures();
			for (uint32_t j = 0; j < loadedTextures->size(); j++)
			{
				Texture* loadedTexture = (*loadedTextures)[j];
				// Check if the file path and the texture type is the same
				if (
					std::strcmp(loadedTexture->GetFilePath().data(), textureFilePath.C_Str()) == 0 &&
					loadedTexture->GetTextureType() == texType
					)
				{
					textures.push_back((*loadedTextures)[j]);
					exists = true;
					break;
				}
			}

			if (exists)
				continue;

			Texture* texture = TextureConstructor::CreateTexture(textureFilePath.C_Str(), (PROCESSING_FLAGS & aiProcess_FlipUVs) != 0, texType);
			textures.push_back(texture);
			loadedTextures->push_back(texture);
		}

		return textures;
	}
}

