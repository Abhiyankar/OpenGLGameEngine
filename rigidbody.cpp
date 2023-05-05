#include "rigidbody.h"
#include <iostream>


RigidBody::RigidBody(float mass, glm::vec3 pos, glm::vec3 velocity, glm::vec3 acceleration)
	: mass(mass), pos(pos), velocity(velocity), acceleration(acceleration)
{
}

void RigidBody::update(float dt)
{
	pos += velocity * dt + 0.5f * acceleration * (dt * dt);
	velocity += acceleration * dt;
	//std::cout << pos.x * dt<< "," << pos.y *dt<< "," << pos.z * dt<< std::endl;
}

void RigidBody::applyForce(glm::vec3 force)
{
	acceleration += force / mass;
}

void RigidBody::applyForce(glm::vec3 direction, float magnitude)
{
	applyForce(direction * magnitude);
}

void RigidBody::applyAcceleration(glm::vec3 a)
{
	acceleration += a;
}

void RigidBody::applyAcceleration(glm::vec3 direction, float magnitude)
{
	applyAcceleration(direction * magnitude);
}

void RigidBody::applyImpulse(glm::vec3 force, float dT)
{
	velocity += force / mass * dT;
}

void RigidBody::applyImpulse(glm::vec3 direction, float magnitude, float dT)
{
	applyImpulse(direction * magnitude, dT);
}

void RigidBody::transformEnergy(float joules)
{
	if (joules == 0)
	{
		return;
	}

	float deltaV = sqrt(2 * abs(joules) / mass);

	velocity += joules > 0 ? deltaV : -deltaV;
}
