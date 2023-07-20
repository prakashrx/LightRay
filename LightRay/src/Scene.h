#pragma once
#include <vector>
#include "Core.h"

class Scene
{
public:
	std::vector<Sphere> m_Spheres;
	void AddSphere(const Sphere& sphere);

};

