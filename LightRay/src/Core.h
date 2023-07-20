#pragma once
#include <glm/glm.hpp>
#include <vector>

using color = glm::vec4;

struct Ray
{
	glm::vec3 Origin;
	glm::vec3 Direction;
};

struct Sphere
{
	glm::vec3 Origin { 0.0f, 0.0f, 0.0f };
	float Radius = .5f;
	color Albedo{ 1.0f, .0f, .0f, 1.0f };

};