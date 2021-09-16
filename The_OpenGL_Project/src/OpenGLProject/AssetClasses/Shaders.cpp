#include "OpenGLProject/AssetClasses/Shaders.h"
#include "OpenGLProject/Utility/Instrumentor.h"

#include "OpenGLProject/Graphics/Renderer.h"
#include "Platform/OpenGL/OpenGLShaders.h"

Ref<ShaderLibrary> ShaderLibrary::GetLibrary()
{
	return CreateRef<ShaderLibrary>();
}

Ref<Shader> Shader::Create(const std::string& filepath)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:    OPENGLPROJECT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RendererAPI::API::OpenGL:  return CreateRef<OpenGLShader>(filepath);
	}

	OPENGLPROJECT_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}

Ref<Shader> Shader::Create(const std::string& filepath1, const std::string& filepath2)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:    OPENGLPROJECT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RendererAPI::API::OpenGL:  return CreateRef<OpenGLShader>(filepath1, filepath2);
	}

	OPENGLPROJECT_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}

Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:    OPENGLPROJECT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RendererAPI::API::OpenGL:  return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
	}

	OPENGLPROJECT_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}

void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
{
	OPENGLPROJECT_CORE_ASSERT(!Exists(name), "Shader already exists!");
	m_Shaders[name] = shader;
}

void ShaderLibrary::Add(const Ref<Shader>& shader)
{
	auto& name = shader->GetName();
	Add(name, shader);
}

Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
{
	auto shader = Shader::Create(filepath);
	Add(shader);
	return shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
{
	auto shader = Shader::Create(filepath);
	Add(name, shader);
	return shader;
}
Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath1, const std::string& filepath2)
{
	auto shader = Shader::Create(filepath1, filepath2);
	Add(name, shader);
	return shader;
}

Ref<Shader> ShaderLibrary::Get(const std::string& name)
{
	OPENGLPROJECT_CORE_ASSERT(Exists(name), "Shader not found!");
	return m_Shaders[name];
}

void ShaderLibrary::showAllNames() {
	for (const std::pair<std::string, Ref<Shader>>& n : m_Shaders) {
		OPENGLPROJECT_CORE_INFO("Shader Name : {0}\n", n.first);
	}
}

bool ShaderLibrary::Exists(const std::string& name) const
{
	return m_Shaders.find(name) != m_Shaders.end();
}