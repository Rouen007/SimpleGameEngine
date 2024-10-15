#pragma once

#include "SimpleEngine.h"

class SandBox2D : public SE::Layer
{
public:
	SandBox2D();
	virtual ~SandBox2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(SE::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(SE::Event& event) override;
private:
	SE::OrthographicCameraController m_CameraController;
	//temp
	SE::Ref<SE::VertexArray> m_SquareVA;
	SE::Ref<SE::Shader> m_FlatColorShader;
	SE::Ref<SE::Texture2D> m_QiyaTexture;

	glm::vec4 m_SquareColor{ 0.2f, 0.3f, 0.8f, 1.0f };

};
