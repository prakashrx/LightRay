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

void LightRay::Renderer::Render(const Camera& camera)
{
	Walnut::Timer timer;

	Ray ray;
	ray.Origin = camera.GetPosition();

	float aspectRatio = m_FinalImage->GetWidth() / (float)m_FinalImage->GetHeight();
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{

			//glm::vec2 coord { (float)x * aspectRatio / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };
			//// The normalized device coordinates still range from -1 to 1 on the x and y axes, representing your viewport (or image plane) in front of the camera.
			//coord = coord * 2.0f - 1.0f;
			//auto pixelColor = PerPixel(coord);
			//pixelColor = glm::clamp(pixelColor, glm::vec4(0.0f), glm::vec4(1.0f));
			//m_ImageData[y * m_FinalImage->GetWidth() + x] = Utils::ConvertToUint32(pixelColor);

			
			ray.Direction = camera.GetRayDirections()[y * m_FinalImage->GetWidth() + x];
			auto pixelColor = TraceRay(ray);
			pixelColor = glm::clamp(pixelColor, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[y * m_FinalImage->GetWidth() + x] = Utils::ConvertToUint32(pixelColor);
		}
	}

	m_FinalImage->SetData(m_ImageData);
	m_LastRenderTime = timer.ElapsedMillis();
}

color LightRay::Renderer::TraceRay(const Ray& ray)
{

	auto r = 0.5f;
	const glm::vec3& O = ray.Origin;
	const glm::vec3& D = ray.Direction;

	auto a = glm::dot(D, D);
	auto b = 2.0f * glm::dot(D, O);
	auto c = glm::dot(O, O) - r * r;

	auto discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return color(0, 0, 0, 1);

	//t = [-b ± sqrt(b*b - 4ac)] / (2a)
	auto t0 = (-b - glm::sqrt(discriminant)) / (2.0f * a); // This should be the shortest hit point. because -ve sign

	auto hitpoint = O + (D * t0);
	auto sphereCenter = glm::vec3(0, 0, 0);
	auto normal = glm::normalize(hitpoint - sphereCenter);


	auto lightDir = glm::normalize(glm::vec3(-1, -1, -1));
	auto lightIntensity = glm::max(glm::dot(normal, -lightDir), 0.0f);

	auto sphereColor = color(1, 0, 1, 1);

	return sphereColor * lightIntensity;
}


color LightRay::Renderer::PerPixel(glm::vec2 coord)
{

	/*
	* Sphere Equation : x*x + y*y + z*z = r*r   which is nothing but P.P (dot product) where P is a 3d point on the surface of the sphere. 
	* A more generic form would be  (P-C).(P-C) = r*r , where C is the 3d center point of that sphere. 
	* If C is (0,0,0) then the equation simply becomes P.P = r*r
	*/

	/*
	* Ray Equation : O + tD.
	* where O is the origin point of the ray, and D is the unit vector that represents the Direction.
	* t is the time step which when multiplied by D gives you the point at time t along the ray.
	* 
	*/

	/*
	* you want to find the point where the ray intersects with the sphere, if any. 
	* That is, you want to find a value of t such that R lies on the sphere. 
	* Substituting the second equation into the first gives: (O + tD) . (O + tD) = r*r
	* Expanding this out, rearranging terms and simplifying gives a quadratic equation in t:
		t*t*(D.D) + t*2*(D.O) + O.O - r*r = 0
		the generic form would be : t*t*(D.D) + t*2*(D.(O-C)) + (O-C).(O-C) - r*r = 0
	*/

	/*
	* A quadratic equation has the general form "at^2 + bt + c = 0", and its solutions can be found using the quadratic formula:
	* t = [-b ± sqrt(b*b - 4ac)] / (2a)
	* If the term under the square root (b*b - 4ac), called the discriminant, is negative, then the equation has no real solutions and the ray does not intersect with the sphere. 
	* If the discriminant is zero, there is one solution and the ray is tangent to the sphere. If the discriminant is positive, there are two solutions: one where the ray enters the sphere, and one where it exits. 
	* For rendering purposes, you generally want the smaller of the two positive solutions, as that is the first point the ray hits.
	*/

	/*
	* a = D.D
	* b = 2*(D.O)
	* c = O.O - r*r
	* discriminant = b*b - 4*a*c
	* where D is the ray direction, O is the ray origin
	*/

	/*
	* Suppose the sphere is still at the origin, with a radius of 0.5.
	*/
	auto r = 0.5f;

	/*
	* place your camera (the origin of the rays) at some point along the positive z-axis, say at (0,0,1). This will have the camera looking towards the sphere in negative z direction.
	*/
	auto O = glm::vec3(0, 0, 1.0f);

	/*
	* In 3D space, a vector pointing from one point to another is found by subtracting the coordinates of the first point from the coordinates of the second.
	So in this case:
	O (0,0,-1) is the origin of the ray (the location of the camera)
	P (x,y,0) is the point on the image plane that we want the ray to pass through
	The vector D = P - O then gives the direction in which the ray should be cast.

	This gives a vector (x, y, 1) that points from the camera location (0,0,-1) to the desired point on the image plane (x, y, 0). This vector D defines the direction of the ray for ray tracing.
	*/
	auto D = glm::vec3(coord, 0.0f) - O;
	
	auto a = glm::dot(D, D);
	auto b = 2.0f * glm::dot(D, O);
	auto c = glm::dot(O, O) - r*r;

	auto discriminant = b * b - 4 * a * c;
	if (discriminant < 0) 
		return color(0, 0, 0, 1);

	

	//t = [-b ± sqrt(b*b - 4ac)] / (2a)
	auto t0 = (-b - glm::sqrt(discriminant)) / (2.0f * a); // This should be the shortest hit point. because -ve sign
	//auto t1 = (-b + glm::sqrt(discriminant)) / (2 * a);

	auto hitpoint = O + (D * t0);
	auto sphereCenter = glm::vec3(0, 0, 0);
	auto normal = glm::normalize(hitpoint - sphereCenter);


	auto lightDir = glm::normalize(glm::vec3(-1, -1, -1));
	auto lightIntensity = glm::max(glm::dot(normal, -lightDir), 0.0f);

	auto sphereColor = color(1, 0, 1, 1);
	//return color(hitpoint, 1);
	//return color(normal * 0.5f + 0.5f,1);
	return sphereColor * lightIntensity;
}

