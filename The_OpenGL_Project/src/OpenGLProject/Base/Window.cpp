#include "OpenGLProject/Base/Window.h"
#include "OpenGLProject/Utility/Log.h"
#include "OpenGLProject/Base/Base.h"

#ifdef OPENGLPROJECT_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

Scope<Window> Window::Create(const WindowProps& props)
{
#ifdef OPENGLPROJECT_PLATFORM_WINDOWS
	return CreateScope<WindowsWindow>(props);
#else
	OPENGLPROJECT_CORE_ASSERT(false, "Unknown platform!");
	return nullptr;
#endif
}