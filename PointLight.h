#pragma once

#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <glm/glm.hpp>
#include "Shader.h"
#include <string>

class PointLight
{
public:
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float constant;
	float linear;
	float quadratic;

	PointLight()
	{
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
		specular = glm::vec3(0.0f, 0.0f, 0.0f);
		constant = 0.0f;
		linear = 0.0f;
		quadratic = 0.0f;
	}

	void lightUp(Shader& shader, glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float cons, float lin, float quad, int index)
	{
		position = pos;
		ambient = amb;
		diffuse = diff;
		specular = spec;
		constant = cons;
		linear = lin;
		quadratic = quad;

		std::string location = "pointLights[" + std::to_string(index) + "]";

		shader.setVec3(location + ".position", position.x, position.y, position.z);
		shader.setVec3(location + ".ambient", ambient.x, ambient.y, ambient.z);
		shader.setVec3(location + ".diffuse", diffuse.x, diffuse.y, diffuse.z);
		shader.setVec3(location + ".specular", specular.x, specular.y, specular.z);

		shader.setFloat(location + ".constant", constant);
		shader.setFloat(location + ".linear", linear);
		shader.setFloat(location + ".quadratic", quadratic);
	}
};

#endif // !DIRECTIONAL_LIGHT_H
