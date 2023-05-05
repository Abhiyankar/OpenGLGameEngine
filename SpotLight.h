#pragma once

#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include <glm/glm.hpp>
#include "Shader.h"

class SpotLight
{
public:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;

	SpotLight()
	{
		position = glm::vec3(0.0f, 0.0f, 0.0f);
		direction = glm::vec3(0.0f, 0.0f, 0.0f);
		ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
		specular = glm::vec3(0.0f, 0.0f, 0.0f);
		constant = 0.0f;
		linear = 0.0f;
		quadratic = 0.0f;
		cutOff = 0.0f;
		outerCutOff = 0.0f;
	}

	void lightUp(Shader& shader, glm::vec3 pos, glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float cons, float lin, float quad, float cOff, float ocOff)
	{
		position = pos;
		direction = dir;
		ambient = amb;
		diffuse = diff;
		specular = spec;
		constant = cons;
		linear = lin;
		quadratic = quad;
		cutOff = cOff;
		outerCutOff = ocOff;

		shader.setVec3("spotLight.position", position.x, position.y, position.z);
		shader.setVec3("spotLight.direction", direction.x, direction.y, direction.z);
		shader.setVec3("spotLight.ambient", ambient.x, ambient.y, ambient.z);
		shader.setVec3("spotLight.diffuse", diffuse.x, diffuse.y, diffuse.z);
		shader.setVec3("spotLight.specular", specular.x, specular.y, specular.z);

		shader.setFloat("spotLight.constant", constant);
		shader.setFloat("spotLight.linear", linear);
		shader.setFloat("spotLight.quadratic", quadratic);
		shader.setFloat("spotLight.cutOff", cutOff);
		shader.setFloat("spotLight.outerCutOff", outerCutOff);
	}
};

#endif // !DIRECTIONAL_LIGHT_H
