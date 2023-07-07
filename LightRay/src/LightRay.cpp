#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "LightRay.h"
#include "Renderer.h"

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Render Time : %.3fms", m_Renderer.GetLastRenderTime());
		if (ImGui::Button("Render")) {
			Render();
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
		ImGui::Begin("ViewPort");
		auto region = ImGui::GetContentRegionAvail();
		float x = ImGui::GetWindowHeight();
		m_ViewPortHeight = region.y, m_ViewPortWidth = region.x;

		auto image = m_Renderer.GetFinalImage();

		if (image) {
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(), (float)image->GetHeight() }, ImVec2(0,1), ImVec2(1,0));
		}
		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}

	void Render() {
		m_Renderer.Resize(m_ViewPortWidth, m_ViewPortHeight);
		m_Renderer.Render();
	}

private:
	uint32_t m_ViewPortWidth = 0, m_ViewPortHeight = 0;
	LightRay::Renderer m_Renderer;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "LightRay";
	spec.Width = 700, spec.Height = 463;
	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	//app->SetMenubarCallback([app]()
	//{
	//	if (ImGui::BeginMenu("File"))
	//	{
	//		if (ImGui::MenuItem("Exit"))
	//		{
	//			app->Close();
	//		}
	//		ImGui::EndMenu();
	//	}
	//});
	return app;
}