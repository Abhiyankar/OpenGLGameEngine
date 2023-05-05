#pragma once

#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <glm/glm.hpp>
#include "Shader.h"

class DirectionalLight
{
public:
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	DirectionalLight()
	{
		direction = glm::vec3(0.0f,0.0f,0.0f);
		ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
		specular = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	void lightUp(Shader& shader, glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
	{
		direction = dir;
		ambient = amb;
		diffuse = diff;
		specular = spec;

		shader.setVec3("dirLight.direction",direction.x,direction.y,direction.z);
		shader.setVec3("dirLight.ambient", ambient.x, ambient.y, ambient.z);
		shader.setVec3("dirLight.diffuse", diffuse.x, diffuse.y, diffuse.z);
		shader.setVec3("dirLight.specular", specular.x, specular.y, specular.z);
	}
};

#endif // !DIRECTIONAL_LIGHT_H
