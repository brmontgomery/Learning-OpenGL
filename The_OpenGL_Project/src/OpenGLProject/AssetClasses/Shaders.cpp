#include "OpenGLProject/AssetClasses/Shaders.h"

#include "OpenGLProject/Graphics/Renderer.h"
#include "Platform/OpenGL/OpenGLShaders.h"

#include "OpenGLProject/Utility/Instrumentor.h"

ShaderLibrary* ShaderLibrary::s_ShaderLibraryInstance = nullptr;

ShaderLibrary* ShaderLibrary::Get()
{
	/**
	 * This is a safer way to create an instance. instance = new Singleton is
	 * dangeruous in case two instance threads wants to access at the same time
	 */
	if (s_ShaderLibraryInstance == nullptr) {
		s_ShaderLibraryInstance = new ShaderLibrary();
	}

	return s_ShaderLibraryInstance;
}

ShaderLibrary::~ShaderLibrary()
{
	OPENGLPROJECT_PROFILE_FUNCTION();
	
	delete s_ShaderLibraryInstance;
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

Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepathvert, const std::string& filepathfrag)
{
	auto shader = Shader::Create(name, filepathvert, filepathfrag);
	Add(name, shader);
	return shader;
}

Ref<Shader> ShaderLibrary::Get(const std::string& name)
{
	OPENGLPROJECT_CORE_ASSERT(Exists(name), "Shader not found!");
	return m_Shaders[name];
}

bool ShaderLibrary::Exists(const std::string& name) const
{
	return m_Shaders.find(name) != m_Shaders.end();
}