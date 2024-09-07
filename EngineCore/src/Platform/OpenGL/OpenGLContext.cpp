#include "sepch.h"
#include "OpenGLContext.h"

#include "SimpleEngine/Log.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace SE
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
		SE_CORE_ASSERT(windowHandle, "windowHandle is null");
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SE_CORE_ASSERT(status, "Failed to initialize Glad!");
		SE_CORE_INFO("OpenGL Renderer : {0} {1}", (const char*)glGetString(GL_VENDOR), (const char*)glGetString(GL_RENDERER));

	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}