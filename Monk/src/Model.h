#pragma once


#include "Mesh.h" 
#include "Renderer.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <glm/glm.hpp>
#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>

#include <iterator>
#include <utility>

class Model
{
private:
	Renderer *m_renderer;
	std::vector<Texture*> m_tex_loaded;
	std::vector<Mesh> m_meshes;
	std::string m_directory;
	
	void load_model(const std::string& path)
	{
		auto import = Assimp::Importer{};

		// the flag aiProcess_Triangulate means to process all _faces_ as triangles 
		auto scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return;
		}
		m_directory = path.substr(0, path.find_last_of('/'));
		process_node(scene->mRootNode, scene);
	}

	void process_node(aiNode* node, const aiScene* scene)
	{
		// Our data structure isn't really appropriate for translating meshes and their parent child relationship. We should actually
		// keep the recurisve parent-child structure rather than just dumping everything into the meshes vector.

		// 
		for (uint32_t i = 0; i < node->mNumMeshes; ++i)
		{
			auto mesh = scene->mMeshes[node->mMeshes[i]];
			m_meshes.push_back(process_mesh(mesh, scene));
		}

		for (uint32_t i = 0; i < node->mNumChildren; ++i)
		{
			process_node(node->mChildren[i], scene);
		}
	}

	Mesh process_mesh(aiMesh* mesh, const aiScene* scene)
	{
		auto vertices = std::vector<Vertex>{};
		auto textures = std::vector<Texture*>{};
		auto indices = std::vector<uint32_t>{};

		for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
		{
			// process vertex positions, normals and texture coords
			auto position_v = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			auto normal_v = glm::vec3(0.0f);
			if (mesh->mNormals != nullptr)
			{
				auto normal_v = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			}
			
			auto texCoords_v = glm::vec2(0.0f, 0.0f);
			if (mesh->mTextureCoords[0])
			{
				texCoords_v = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			} 
			auto vertex = Vertex{ position_v, normal_v, texCoords_v };
			vertices.push_back(vertex);
		}

		// process the indices. This means processing each face as a triangle. This is how assimp stores them 
		for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
		{
			auto face = mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// process the material
		if (mesh->mMaterialIndex >= 0)
		{
			auto material = scene->mMaterials[mesh->mMaterialIndex];

			auto diffuse_map = load_material_textures(material, aiTextureType_DIFFUSE, DIFFUSE);
			auto specular_map = load_material_textures(material, aiTextureType_SPECULAR, SPECULAR);

			textures.insert(textures.end(), diffuse_map.begin(), diffuse_map.end());
			textures.insert(textures.end(), specular_map.begin(), specular_map.end());
		}

		return Mesh(vertices, std::move(textures), indices);
	}

	std::vector<Texture*> load_material_textures(aiMaterial *mat, aiTextureType type, TextureType typeName)
	{
		auto textures = std::vector<Texture*>{};
		for (uint32_t i = 0; i < mat->GetTextureCount(type); ++i)
		{
			auto ai_str = aiString{};
			mat->GetTexture(type, i, &ai_str);

			auto ai_std_string = std::string{ ai_str.C_Str() };

			auto file_str = m_directory + "/" + std::string{ai_str.C_Str()};
			if (ai_std_string.rfind("Program Files", 0) == 0)
			{
				file_str = m_directory + "/diffuse.jpeg";
			}
			auto texture = m_renderer->load_texture(file_str, typeName);
			textures.push_back(texture);
		}
		return textures;
	}

public:
	Model(const char* path, Renderer* renderer) 
		: m_renderer{renderer}
	{
		load_model(path);
	}
	
	void Draw(Shader* shader)
	{
		for (uint32_t i = 0; i < m_meshes.size(); ++i)
		{
			m_meshes[i].draw(shader);
		}
	}
};