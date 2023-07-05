#pragma once
#include<memory>

#include "Walnut/Image.h"
#include <glm/glm.hpp>

namespace LightRay 
{
	class Renderer
	{
	public:
		Renderer();
		void Resize(uint32_t width, uint32_t height);
		void Render();
		uint32_t PerPixel(glm::vec2 coord);
		float GetLastRenderTime() { return m_LastRenderTime; };
		std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }
	private:
		std::shared_ptr<Walnut::Image> m_FinalImage;
		uint32_t* m_ImageData = nullptr;
		float m_LastRenderTime = 0;
	};
}

