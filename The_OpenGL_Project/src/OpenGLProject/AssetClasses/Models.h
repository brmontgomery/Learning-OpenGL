#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "OpenGLProject/AssetClasses/Mesh.h"
#include "OpenGLProject/AssetClasses/Textures.h"

#include <stb-master/stb_image.h>
#include <assimp/config.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "OpenGLProject/Utility/Log.h"

class Model {
public:
	//functions
	Model(const std::string& filepath);
	Model(const std::string& name, const std::string& filepath);

	void Model::Submit(const Ref<Shader>& shader, const glm::mat4& transform);

	const std::string& GetName() { return m_Name; }


	static Ref<Model> Create(const std::string& filepath);
	static Ref<Model> Create(const std::string& name, const std::string& filepath);

private:
	std::string m_Name;

	//model data
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<TextureInfo> textures_loaded;

	Ref<Texture2DLibrary> s_Texture2DLibrary;

	//functions
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<TextureInfo> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

class ModelLibrary
{
public:
	ModelLibrary() {}
	ModelLibrary::~ModelLibrary() {};
	static Ref<ModelLibrary> GetLibrary();

	void Add(const std::string& name, const Ref<Model>& model);
	void Add(const Ref<Model>& model);
	Ref<Model> Load(const std::string& filepath);
	Ref<Model> Load(const std::string& name, const std::string& filepath);

	Ref<Model> Get(const std::string& name);
	void showAllNames();

	bool Exists(const std::string& name) const;
private:
	std::unordered_map<std::string, Ref<Model>> m_Models;
};


