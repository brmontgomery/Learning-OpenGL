#include "OpenGLProject/AssetClasses/Models.h"
#include "OpenGLProject/Utility/Instrumentor.h"

#include "OpenGLProject/Graphics/Renderer.h"

Ref<ModelLibrary> ModelLibrary::GetLibrary()
{
	return CreateRef<ModelLibrary>();
}

Ref<Model> Model::Create(const std::string& filepath)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:    OPENGLPROJECT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RendererAPI::API::OpenGL:  return CreateRef<Model>(filepath);
	}

	OPENGLPROJECT_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}

Ref<Model> Model::Create(const std::string& name, const std::string& filepath)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:    OPENGLPROJECT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RendererAPI::API::OpenGL:  return CreateRef<Model>(name, filepath);
	}

	OPENGLPROJECT_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}

void ModelLibrary::Add(const std::string& name, const Ref<Model>& model)
{
	OPENGLPROJECT_CORE_ASSERT(!Exists(name), "Model already exists!");
	m_Models[name] = model;
}

void ModelLibrary::Add(const Ref<Model>& model)
{
	auto& name = model->GetName();
	Add(name, model);
}

Ref<Model> ModelLibrary::Load(const std::string& filepath)
{
	auto model = Model::Create(filepath);
	Add(model);
	OPENGLPROJECT_CORE_INFO("Model: \"{0}\" created!", model->GetName());
	return model;
}

Ref<Model> ModelLibrary::Load(const std::string& name, const std::string& filepath)
{
	auto model = Model::Create(filepath);
	Add(name, model);
	OPENGLPROJECT_CORE_INFO("Model: \"{0}\" created!", name);
	return model;
}

Ref<Model> ModelLibrary::Get(const std::string& name)
{
	if (name.find('/') != std::string::npos) {
		std::string newName = "";
		auto lastSlash = name.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = name.rfind('.');
		auto count = lastDot == std::string::npos ? name.size() - lastSlash : lastDot - lastSlash;
		newName = name.substr(lastSlash, count);

		OPENGLPROJECT_CORE_ASSERT(Exists(newName), "Model not found!");
		return m_Models[newName];
	}
	OPENGLPROJECT_CORE_ASSERT(Exists(name), "Model not found!");
	return m_Models[name];
}

void ModelLibrary::showAllNames() {
	for (const std::pair<std::string, Ref<Model>>& n : m_Models) {
		OPENGLPROJECT_CORE_INFO("Model Name : {0}\n", n.first);
	}
}

bool ModelLibrary::Exists(const std::string& name) const
{
	return m_Models.find(name) != m_Models.end();
}

//------------------------------------------------------------------------------------------------------------------

Model::Model(const std::string& filepath) {
	loadModel(filepath);

	// Extract name from filepath
	auto lastSlash = filepath.find_last_of("/\\");
	lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
	auto lastDot = filepath.rfind('.');
	auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
	m_Name = filepath.substr(lastSlash, count);
}

Model::Model(const std::string& name, const std::string& filepath)
: m_Name(name) {
	loadModel(filepath);
}

void Model::Submit(const Ref<Shader>& shader, const glm::mat4& transform) {
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Submit(shader, transform, s_Texture2DLibrary);
}


void Model::loadModel(std::string path) 
{
	s_Texture2DLibrary = Texture2DLibrary::GetLibrary();

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		OPENGLPROJECT_CORE_ERROR("ERROR::ASSIMP::{0}\n", import.GetErrorString());
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));
	OPENGLPROJECT_CORE_INFO("{0}\n", directory);

	processNode(scene->mRootNode, scene);
}


void Model::processNode(aiNode* node, const aiScene* scene) {
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}


Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::vector<TextureInfo> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{

		//vertice position
		vertices.push_back(mesh->mVertices[i].x);
		vertices.push_back(mesh->mVertices[i].y);
		vertices.push_back(mesh->mVertices[i].z);

		//process normals
		vertices.push_back(mesh->mNormals[i].x);
		vertices.push_back(mesh->mNormals[i].y);
		vertices.push_back(mesh->mNormals[i].z);

		//process texture coords
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			vertices.push_back(mesh->mTextureCoords[0][i].x);
			vertices.push_back(mesh->mTextureCoords[0][i].y);
		}
		else {
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}
	}

	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	// process material
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<TextureInfo> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<TextureInfo> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}


std::vector<TextureInfo> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<TextureInfo> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		std::cout << str.C_Str() << std::endl;
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if(s_Texture2DLibrary->Exists(std::string(str.C_Str())))
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			std::string filename = std::string(str.C_Str());
			std::string path = this->directory + filename;
			s_Texture2DLibrary->Load(filename, path);

			TextureInfo texture;
			texture.type = typeName;
			texture.name = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}