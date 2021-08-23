#include "OpenGLProject/Graphics/Buffer.h"

#include "OpenGLProject/Graphics/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"


Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:    OPENGLPROJECT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexBuffer>(size);
	}

	OPENGLPROJECT_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}

Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:    OPENGLPROJECT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexBuffer>(vertices, size);
	}

	OPENGLPROJECT_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}

Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:    OPENGLPROJECT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RendererAPI::API::OpenGL:  return CreateRef<OpenGLIndexBuffer>(indices, size);
	}

	OPENGLPROJECT_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}