#include "Base/Application.h"

#include "Utility/Log.h"

//#include "Eros/Renderer/Renderer.h"

//#include "Eros/Core/Input/Input.h"

#include "Utility/Log.h"
#include "Utility/Instrumentor.h"

#include <GLFW/glfw3.h>

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& name, ApplicationCommandLineArgs args)
	: m_CommandLineArgs(args)
{
	OPENGLPROJECT_PROFILE_FUNCTION();

	OPENGLPROJECT_CORE_ASSERT(!s_Instance, "Application already exists!");
	s_Instance = this;
	m_Window = Window::Create(WindowProps(name));
	//m_Window->SetEventCallback(OPENGLPROJECT_BIND_EVENT_FN(Application::OnEvent));

	//Renderer::Init();

	//m_ImGuiLayer = new ImGuiLayer();
	//PushOverlay(m_ImGuiLayer);
}

Application::~Application()
{
	OPENGLPROJECT_PROFILE_FUNCTION();

	//Renderer::Shutdown();
}

/*void Application::PushLayer(Layer* layer)
{
	OPENGLPROJECT_PROFILE_FUNCTION();

	m_LayerStack.PushLayer(layer);
	layer->OnAttach();
}*/

/*void Application::PushOverlay(Layer* layer)
{
	OPENGLPROJECT_PROFILE_FUNCTION();

	m_LayerStack.PushOverlay(layer);
	layer->OnAttach();
}*/

void Application::Close()
{
	m_Running = false;
}

/*void Application::OnEvent(Event& e)
{
	OPENGLPROJECT_PROFILE_FUNCTION();

	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(OPENGLPROJECT_BIND_EVENT_FN(Application::OnWindowClose));
	dispatcher.Dispatch<WindowResizeEvent>(OPENGLPROJECT_BIND_EVENT_FN(Application::OnWindowResize));

	for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
	{
		if (e.Handled)
			break;
		(*it)->OnEvent(e);
	}
}*/

void Application::Run()
{
	OPENGLPROJECT_PROFILE_FUNCTION();

	while (m_Running)
	{
		OPENGLPROJECT_PROFILE_SCOPE("RunLoop");

		float time = (float)glfwGetTime();
		Timestep timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;

		if (!m_Minimized)
		{
			/*{
				OPENGLPROJECT_PROFILE_SCOPE("LayerStack OnUpdate");

				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}

			m_ImGuiLayer->Begin();
			{
				OPENGLPROJECT_PROFILE_SCOPE("LayerStack OnImGuiRender");

				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();
			*/
		}

		m_Window->OnUpdate();
	}
}

/*bool Application::OnWindowClose(WindowCloseEvent& e)
{
	m_Running = false;
	return true;
}*/

/*bool Application::OnWindowResize(WindowResizeEvent& e)
{
	OPENGLPROJECT_PROFILE_FUNCTION();

	if (e.GetWidth() == 0 || e.GetHeight() == 0)
	{
		m_Minimized = true;
		return false;
	}

	m_Minimized = false;
	Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

	return false;
}*/