
#include "OpenGLProject/Graphics/GraphicsContext.h"

#include "OpenGLProject/Graphics/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include "OpenGLProject/Utility/Instrumentor.h"

Scope<GraphicsContext> GraphicsContext::Create(void* window)
{
	OPENGLPROJECT_PROFILE_FUNCTION();
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:    OPENGLPROJECT_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
	case RendererAPI::API::Vulkan:  OPENGLPROJECT_ASSERT(false, "RendererAPI::Vulkan is currently not supported!"); return nullptr;
	}

	OPENGLPROJECT_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}