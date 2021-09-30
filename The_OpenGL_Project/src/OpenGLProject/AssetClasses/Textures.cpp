#include "OpenGLProject/AssetClasses/Textures.h"

#include "OpenGLProject/Graphics/Renderer.h"
#include "Platform/OpenGL/OpenGLTextures.h"


Ref<Texture2DLibrary> Texture2DLibrary::GetLibrary()
{
	return CreateRef<Texture2DLibrary>();
}

Ref<Texture2D> Texture2D::Create(const std::string& name, uint32_t width, uint32_t height)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:    OPENGLPROJECT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(name, width, height);
	}

	OPENGLPROJECT_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}

Ref<Texture2D> Texture2D::Create(const std::string& path)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:    OPENGLPROJECT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(path);
	}

	OPENGLPROJECT_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}

void Texture2DLibrary::Add(const std::string& name, const Ref<Texture2D>& texture)
{
	OPENGLPROJECT_CORE_ASSERT(!Exists(name), "Texture already exists!");
	m_Textures[name] = texture;
}

void Texture2DLibrary::Add(const Ref<Texture2D>& texture)
{
	auto& name = texture->GetName();
	Add(name, texture);
}

Ref<Texture2D> Texture2DLibrary::Load(const std::string& name, uint32_t width, uint32_t height)
{
	auto texture = Texture2D::Create(name, width, height);
	Add(texture);
	return texture;
}

Ref<Texture2D> Texture2DLibrary::Load(const std::string& filepath)
{
	auto texture = Texture2D::Create(filepath);
	Add(texture);
	return texture;
}

Ref<Texture2D> Texture2DLibrary::Load(const std::string& name, const std::string& filepath)
{
	auto texture = Texture2D::Create(filepath);
	Add(name, texture);
	return texture;
}


Ref<Texture2D> Texture2DLibrary::Get(const std::string& name)
{
	if (name.find('/') != std::string::npos) {
		std::string newName = "";
		auto lastSlash = name.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = name.rfind('.');
		auto count = lastDot == std::string::npos ? name.size() - lastSlash : lastDot - lastSlash;
		newName = name.substr(lastSlash, count);

		OPENGLPROJECT_CORE_ASSERT(Exists(newName), "Texture not found!");
		return m_Textures[newName];
	}
	else {
		OPENGLPROJECT_CORE_ASSERT(Exists(name), "Texture not found!");
		return m_Textures[name];
	}
}

void Texture2DLibrary::showAllNames() {
	for (const std::pair<std::string, Ref<Texture2D>>& n : m_Textures) {
		OPENGLPROJECT_CORE_INFO("Texture Name : {0}\n", n.first);
	}
}

bool Texture2DLibrary::Exists(const std::string& name) const
{
	return m_Textures.find(name) != m_Textures.end();
}