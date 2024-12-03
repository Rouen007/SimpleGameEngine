#include "sepch.h"
#include "OrthographicCameraController.h"
#include "SimpleEngine/Core/Input.h"
#include "SimpleEngine/Core/KeyCodes.h"

namespace SE
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio)
		, m_Camera(-m_AspectRatio*m_ZoomLevel, m_AspectRatio*m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
		, m_Rotation(rotation)
		, m_CameraPosition(0.0f, 0.0f, 0.0f)
	{

	}
	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		SE_PROFILE_FUNCTION();

		if (Input::IsKeyPressed(SE_KEY_A))
		{
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		}
		else if (Input::IsKeyPressed(SE_KEY_D))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		}

		if (Input::IsKeyPressed(SE_KEY_S))
		{
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		}
		else if (Input::IsKeyPressed(SE_KEY_W))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		}
		if (m_Rotation)
		{
			if (Input::IsKeyPressed(SE_KEY_Q))
			{
				m_CameraRotation -= m_CameraRotationSpeed;
			}
			else if (Input::IsKeyPressed(SE_KEY_E))
			{
				m_CameraRotation += m_CameraRotationSpeed;
			}
			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);
	}
	void OrthographicCameraController::OnEvent(Event& e)
	{
		SE_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN_X(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN_X(OrthographicCameraController::OnWindowResized));
	}
	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		SE_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset();
		m_ZoomLevel = std::max(0.1f, m_ZoomLevel);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		SE_PROFILE_FUNCTION();

		m_AspectRatio = (float) e.GetWidth() / e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;

	}
}