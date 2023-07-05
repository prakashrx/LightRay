#include "Renderer.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

LightRay::Renderer::Renderer() : 
	m_LastRenderTime {0}
{
}

void LightRay::Renderer::Resize(uint32_t width, uint32_t height)
{
	if (!m_FinalImage)
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	else if (m_FinalImage->GetWidth() != width || m_FinalImage->GetHeight() != height)
		m_FinalImage->Resize(width, height);
	else
		return;

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width  * height];
}

void LightRay::Renderer::Render()
{
	Walnut::Timer timer;
	
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			glm::vec2 coord { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };
			m_ImageData[y * m_FinalImage->GetWidth() + x] = PerPixel(coord);
		}
	}

	m_FinalImage->SetData(m_ImageData);
	m_LastRenderTime = timer.ElapsedMillis();
}

uint32_t LightRay::Renderer::PerPixel(glm::vec2 coord)
{
	uint8_t r = (uint8_t)(coord.x * 255.0f);
	uint8_t g = (uint8_t)(coord.y * 255.0f);
	return 0xff000000 | (g << 8) | r;
}
