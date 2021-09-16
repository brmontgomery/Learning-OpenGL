#pragma once

#include "OpenGLProject/Base/Base.h"
#include "OpenGLProject/AssetClasses/Shaders.h"

#include "OpenGLProject/Base/Window.h"
//#include "Eros/Core/Layers/LayerStack.h"
//#include "Eros/Events/Event.h"
//#include "Eros/Events/ApplicationEvent.h"

#include "OpenGLProject/Utility/Timestep.h"

//#include "Eros/ImGui/ImGuiLayer.h"

#include "OpenGLProject/Utility/Log.h"

class Application
{
public:
	Application(const std::string appName);
	~Application();

	std::string getAppName() const {
		return appName;
	}

	void Run();
	void Close();

	Window& GetWindow() { return *m_Window; }

	//void OnEvent(Event& e);

	//void PushLayer(Layer* layer);
	//void PushOverlay(Layer* layer);

	//ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

private:
	//bool OnWindowClose(WindowCloseEvent& e);
	//bool OnWindowResize(WindowResizeEvent& e);

	std::string appName;
	Scope<Window> m_Window;
	//ImGuiLayer* m_ImGuiLayer;
	bool m_Running = true;
	bool m_Minimized = false;
	//LayerStack m_LayerStack;
	float m_LastFrameTime = 0.0f;
protected:
	Ref<ShaderLibrary> s_ShaderLibrary;
};

