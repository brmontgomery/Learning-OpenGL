#include "OpenGLProject/Base/Application.h"

#include "OpenGLProject/Utility/Log.h"
#include "OpenGLProject/AssetClasses/Models.h"

#include "OpenGLProject/Graphics/Renderer.h"

//#include "Eros/Core/Input/Input.h"

#include "OpenGLProject/Utility/Log.h"
#include "OpenGLProject/Utility/Instrumentor.h"

#include <GLFW/glfw3.h>

Application* Application::s_AppInstance = nullptr;

Application* Application::Get(const std::string& name)
{
	/**
	 * This is a safer way to create an instance. instance = new Singleton is
	 * dangeruous in case two instance threads wants to access at the same time
	 */
	if (s_AppInstance == nullptr) {
		s_AppInstance = new Application(name);
		s_AppInstance->m_Window = Window::Create(WindowProps(name));
	}
	
	auto shaderLib = ShaderLibrary::Get();

	//initialize everything else
	shaderLib->Load("FancyShader", "assets/Shaders/lightingShader.vert", "assets/Shaders/lightingShader.frag");
	shaderLib->Load("FlatShader", "assets/Shaders/lampShader.vert", "assets/Shaders/lampShader.frag");

	//lamp VAO stuff
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	//set the coords for lights to light the object
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	Model nanosuitModel("assets/models/nanosuit/crysis_nano_suit_2/scene.gltf");

	return s_AppInstance;
}

Application::~Application()
{
	OPENGLPROJECT_PROFILE_FUNCTION();

	//Renderer::Shutdown();
	delete s_AppInstance;
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