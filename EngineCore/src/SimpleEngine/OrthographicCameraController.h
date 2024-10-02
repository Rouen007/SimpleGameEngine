#pragma once
#include "SimpleEngine/Renderer/OrthographicCamera.h"
#include "SimpleEngine/Core/Timestep.h"
#include "SimpleEngine/Events/ApplicationEvent.h"
#include "SimpleEngine/Events/MouseEvent.h"

namespace SE
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_ZoomLevel = 1.0f;
		float m_AspectRatio;
		OrthographicCamera m_Camera;
		bool m_Rotation;

		glm::vec3 m_CameraPosition;
		float m_CameraMoveSpeed = 0.5f;
		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = 0.2f;
	};
}