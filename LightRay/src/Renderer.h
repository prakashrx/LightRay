#pragma once
#include "LightRay.h"
#include "Walnut/Image.h"
#include "Utils.h"
#include "Camera.h"
#include "Ray.h"

namespace LightRay 
{
	class Renderer
	{
	public:
		Renderer();
		void Resize(uint32_t width, uint32_t height);
		void Render(const Camera& camera);
		color PerPixel(glm::vec2 coord);
		color TraceRay(const Ray& ray);
		float GetLastRenderTime() { return m_LastRenderTime; };
		std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }
	private:
		std::shared_ptr<Walnut::Image> m_FinalImage;
		uint32_t* m_ImageData = nullptr;
		float m_LastRenderTime = 0;
	};
}

