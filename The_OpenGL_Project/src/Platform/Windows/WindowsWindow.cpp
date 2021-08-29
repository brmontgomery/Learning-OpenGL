#include "WindowsWindow.h"
#include "OpenGLProject/Base/Base.h"

#include "OpenGLProject/Graphics/Renderer.h"

#include "OpenGLProject/Utility/Instrumentor.h"

//#include "OPENGLPROJECT/Core/Input/Input.h"

//#include "OPENGLPROJECT/Core/Events/ApplicationEvents.h"
//#include "OPENGLPROJECT/Core/Events/MouseEvents.h"
//#include "OPENGLPROJECT/Core/Events/KeyEvents.h"

static uint8_t s_GLFWWindowCount = 0;

static void GLFWErrorCallback(int error, const char* description)
{
	OPENGLPROJECT_ERROR("GLFW Error ({0}): {1}", error, description);
}

WindowsWindow::WindowsWindow(const WindowProps& props)
{
	OPENGLPROJECT_PROFILE_FUNCTION();

	Init(props);
}

WindowsWindow::~WindowsWindow()
{
	OPENGLPROJECT_PROFILE_FUNCTION();

	Shutdown();
}

void WindowsWindow::Init(const WindowProps& props)
{
	OPENGLPROJECT_PROFILE_FUNCTION();

	windowData.Title = props.Title;
	windowData.Width = props.Width;
	windowData.Height = props.Height;

	OPENGLPROJECT_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

	if (s_GLFWWindowCount == 0)
	{
		OPENGLPROJECT_PROFILE_SCOPE("glfwInit");
		int success = glfwInit();
		OPENGLPROJECT_ASSERT(success, "Could not intialize GLFW!");
		glfwSetErrorCallback(GLFWErrorCallback);
	}

	{
		OPENGLPROJECT_PROFILE_SCOPE("glfwCreateWindow");
		if (Renderer::GetAPI() == RendererAPI::API::Vulkan)
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#if defined(OPENGLPROJECT_DEBUG)
		if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
		window = glfwCreateWindow((int)props.Width, (int)props.Height, windowData.Title.c_str(), nullptr, nullptr);
		++s_GLFWWindowCount;
	}

	m_Context = GraphicsContext::Create(window);
	m_Context->Init();

	glfwSetWindowUserPointer(window, &windowData);
	SetVSync(true);

	// Set GLFW callbacks
	/*glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.Width = width;
		data.Height = height;

		WindowResizeEvent event(width, height);
		data.EventCallback(event);
	});

	glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		WindowCloseEvent event;
		data.EventCallback(event);
	});

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		switch (action)
		{
		case GLFW_PRESS:
		{
			KeyPressedEvent event(static_cast<KeyCode>(key), 0);
			data.EventCallback(event);
			break;
		}
		case GLFW_RELEASE:
		{
			KeyReleasedEvent event(static_cast<KeyCode>(key));
			data.EventCallback(event);
			break;
		}
		case GLFW_REPEAT:
		{
			KeyPressedEvent event(static_cast<KeyCode>(key), 1);
			data.EventCallback(event);
			break;
		}
		}
	});

	glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int keycode)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		KeyTypedEvent event(static_cast<KeyCode>(keycode));
		data.EventCallback(event);
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		switch (action)
		{
		case GLFW_PRESS:
		{
			MouseButtonPressedEvent event(static_cast<MouseCode>(button));
			data.EventCallback(event);
			break;
		}
		case GLFW_RELEASE:
		{
			MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
			data.EventCallback(event);
			break;
		}
		}
	});

	glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		MouseScrolledEvent event((float)xOffset, (float)yOffset);
		data.EventCallback(event);
	});

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		MouseMovedEvent event((float)xPos, (float)yPos);
		data.EventCallback(event);
	});*/
}

void WindowsWindow::Shutdown()
{
	OPENGLPROJECT_PROFILE_FUNCTION();

	glfwDestroyWindow(window);
	--s_GLFWWindowCount;

	if (s_GLFWWindowCount == 0)
	{
		glfwTerminate();
	}
}

void WindowsWindow::OnUpdate()
{
	OPENGLPROJECT_PROFILE_FUNCTION();

	glfwPollEvents();

	
}

void WindowsWindow::SetVSync(bool enabled)
{
	OPENGLPROJECT_PROFILE_FUNCTION();

	/*
	if (enabled)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
	*/
	windowData.VSync = enabled;
}

bool WindowsWindow::IsVSync() const
{
	return windowData.VSync;
}