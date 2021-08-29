#include "OpenGLProject/Graphics/RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "OpenGLProject/Base/Base.h"


RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

Scope<RendererAPI> RendererAPI::Create()
{
	switch (s_API)
	{
	case RendererAPI::API::None:    OPENGLPROJECT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RendererAPI::API::OpenGL:  return CreateScope<OpenGLRendererAPI>();
	}

	OPENGLPROJECT_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}