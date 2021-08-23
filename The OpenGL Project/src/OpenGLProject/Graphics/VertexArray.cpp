#include "OpenGLProject/Graphics/VertexArray.h"

#include "OpenGLProject/Graphics/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"


Ref<VertexArray> VertexArray::Create()
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:    OPENGLPROJECT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexArray>();
	}

	OPENGLPROJECT_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}