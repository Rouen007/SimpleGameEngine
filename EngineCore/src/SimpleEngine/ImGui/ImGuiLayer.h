#pragma once
#include "SimpleEngine/Layer.h"
#include "SimpleEngine/Events/ApplicationEvent.h"
#include "SimpleEngine/Events/KeyEvent.h"
#include "SimpleEngine/Events/MouseEvent.h"

namespace SE
{
	class SENGINE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	
	private:
		float m_Time = 0;
	};
}