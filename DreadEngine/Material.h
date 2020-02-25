#pragma once
#include "glm.hpp"

class Material
{
public:	
	Material() : ambient(1), diffuse(1), specular(0), emissive(0), specularPower(1), opacity(1) {}
	~Material() {}

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 emissive;

	float specularPower;
	float opacity;
};

