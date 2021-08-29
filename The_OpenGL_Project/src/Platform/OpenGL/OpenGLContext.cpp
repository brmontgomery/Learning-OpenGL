#include "Platform/OpenGL/OpenGLContext.h"
#include "OpenGLProject/Utility/Instrumentor.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>


OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
	: m_WindowHandle(windowHandle)
{
	OPENGLPROJECT_CORE_ASSERT(windowHandle, "Window handle is null!")
}

void OpenGLContext::Init()
{
	OPENGLPROJECT_PROFILE_FUNCTION();

	glfwMakeContextCurrent(m_WindowHandle);
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	OPENGLPROJECT_CORE_ASSERT(status, "Failed to initialize Glad!");

	OPENGLPROJECT_CORE_INFO("OpenGL Info:");
	OPENGLPROJECT_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
	OPENGLPROJECT_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
	OPENGLPROJECT_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

	OPENGLPROJECT_CORE_ASSERT(GLVersion.major > 3 || (GLVersion.major == 3 && GLVersion.minor >= 3), "Hazel requires at least OpenGL version 4.5!");
}

void OpenGLContext::SwapBuffers()
{
	OPENGLPROJECT_PROFILE_FUNCTION();

	glfwSwapBuffers(m_WindowHandle);
}