#include "rigidbody.h"

Rigidbody::Rigidbody(sf::Shape* shape, sf::RenderWindow* window) : shape(shape), window(window)
{
	// Setting up the initial values
	position = shape->getPosition();
	velocity = sf::Vector2f(0, 0);
	acceleration = sf::Vector2f(0, 0);
	force = sf::Vector2f(0, 0);
	impulseForce = sf::Vector2f(0, 0);
	mass = 1;
	drag = 0;

	rotation = shape->getRotation();
	angularVelocity = 0;
	angularAcceleration = 0;
	torque = 0;
	impulseTorque = 0;
	// Calculating the inertia according to the shape
	inertia = 1 / 12.0f * mass * (shape->getGlobalBounds().getSize().x * shape->getGlobalBounds().getSize().x + shape->getGlobalBounds().getSize().y * shape->getGlobalBounds().getSize().y);
	angularDrag = 0;

	// Calculating the upVector
	upVector = sf::Vector2f(sin(rotation * 3.14159f / 180), -cos(rotation * 3.14159f / 180));

	boundingBoxSize = shape->getGlobalBounds().getSize();
}

void Rigidbody::update(float dt)
{
	// Calculate upVector
	upVector = sf::Vector2f(sin(rotation * 3.14159f / 180), -cos(rotation * 3.14159f / 180));

	// Calculate acceleration
	acceleration = (force + impulseForce) / mass;

	// Calculate angular acceleration
	angularAcceleration = (torque + impulseTorque) / inertia;

	// Calculate velocity
	velocity += acceleration * dt;

	// Calculate angular velocity
	angularVelocity += angularAcceleration * dt;

	// Calculate drag
	velocity -= velocity * drag * dt;
	angularVelocity -= angularVelocity * angularDrag * dt;

	// Calculate position
	position += velocity * dt;

	// Calculate rotation
	rotation += angularVelocity * dt;

	// Keep rotation between 0 and 360
	while (rotation > 360)
	{
		rotation -= 360;
	}

	// Reset impulse forces and torques
	impulseForce = sf::Vector2f(0, 0);
	impulseTorque = 0;

	// Update shape
	shape->setPosition(position);
	shape->setRotation(rotation);
}

void Rigidbody::draw()
{
	window->draw(*shape);
}

void Rigidbody::setPosition(sf::Vector2f position)
{
	this->position = position;
}

void Rigidbody::setVelocity(sf::Vector2f velocity)
{
	this->velocity = velocity;
}

void Rigidbody::setAcceleration(sf::Vector2f acceleration)
{
	this->acceleration = acceleration;
}

void Rigidbody::setForce(sf::Vector2f force)
{
	this->force = force;
}

void Rigidbody::applyForce(sf::Vector2f force)
{
	this->force += force;
}

void Rigidbody::applyImpulseForce(sf::Vector2f impulseForce)
{
	this->impulseForce += impulseForce;
}

void Rigidbody::setMass(float mass)
{
	this->mass = mass;
}

void Rigidbody::setDrag(float drag)
{
	this->drag = drag;
}

void Rigidbody::setRotation(float rotation)
{
	this->rotation = rotation;
}

void Rigidbody::setAngularVelocity(float angularVelocity)
{
	this->angularVelocity = angularVelocity;
}

void Rigidbody::setAngularAcceleration(float angularAcceleration)
{
	this->angularAcceleration = angularAcceleration;
}

void Rigidbody::setTorque(float torque)
{
	this->torque = torque;
}

void Rigidbody::applyTorque(float torque)
{
	this->torque += torque;
}

void Rigidbody::applyImpulseTorque(float impulseTorque)
{
	this->impulseTorque += impulseTorque;
}

void Rigidbody::setInertia(float inertia)
{
	this->inertia = inertia;
}

void Rigidbody::setAngularDrag(float angularDrag)
{
	this->angularDrag = angularDrag;
}

void Rigidbody::applyImpulseForceAtOffset(sf::Vector2f impulseForce, sf::Vector2f offset)
{
	// Force at an offset causes torque
	this->impulseForce += impulseForce;
	this->impulseTorque += offset.x * impulseForce.y - offset.y * impulseForce.x;
}

sf::Vector2f Rigidbody::getPosition()
{
	return position;
}

sf::Vector2f Rigidbody::getVelocity()
{
	return velocity;
}

sf::Vector2f Rigidbody::getAcceleration()
{
	return acceleration;
}

sf::Vector2f Rigidbody::getForce()
{
	return force + impulseForce;
}

float Rigidbody::getMass()
{
	return mass;
}

float Rigidbody::getDrag()
{
	return drag;
}

float Rigidbody::getRotation()
{
	return rotation;
}

float Rigidbody::getAngularVelocity()
{
	return angularVelocity;
}

float Rigidbody::getAngularAcceleration()
{
	return angularAcceleration;
}

float Rigidbody::getTorque()
{
	return torque;
}

float Rigidbody::getInertia()
{
	return inertia;
}

float Rigidbody::getAngularDrag()
{
	return angularDrag;
}

sf::Vector2f Rigidbody::getUpVector()
{
	return upVector;
}

sf::Shape* Rigidbody::getShape()
{
	return shape;
}

sf::Vector2f Rigidbody::getBoundingBoxSize()
{
	return boundingBoxSize;
}

void Rigidbody::setFillColor(sf::Color color)
{
	shape->setFillColor(color);
}
