#pragma once

#ifndef RIGIDBODY_H
#define RIGIDBOYD_H

#include <glm/glm.hpp>

class RigidBody
{
public:
	float mass;

	glm::vec3 pos;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	RigidBody(float mass = 1.0f, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 veloctiy = glm::vec3(0.0f), glm::vec3 acceleration = glm::vec3(0.0f));
	
	void update(float dt);

	void applyForce(glm::vec3 force);
	void applyForce(glm::vec3 direction, float magnitude);

	void applyAcceleration(glm::vec3 acceleration);
	void applyAcceleration(glm::vec3 direction, float magnitude);

	void applyImpulse(glm::vec3 force, float dT);
	void applyImpulse(glm::vec3 direction, float magnitude, float dT);

	void transformEnergy(float joules);
};


#endif // !RIGIDBODY_H