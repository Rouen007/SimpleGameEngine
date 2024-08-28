#pragma once
#include "SimpleEngine/Renderer/GraphicsContexture.h"

struct GLFWwindow;

namespace SE
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}
