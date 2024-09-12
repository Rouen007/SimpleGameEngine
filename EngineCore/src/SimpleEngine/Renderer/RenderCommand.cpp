#include "sepch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace SE
{
	RendererAPI* RenderCommand::s_RenderAPI = new OpenGLRendererAPI;
}