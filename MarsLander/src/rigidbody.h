#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class Rigidbody
{
private:
	// Reference to the shape and window
	sf::RenderWindow* window;
	sf::Shape* shape;
	sf::Vector2f boundingBoxSize;
	sf::Texture* shapeTexture;

	sf::Shape* fireShape;
	sf::Texture* fireTexture;

	// Kinematics and dynamics variables
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;
	sf::Vector2f force;
	sf::Vector2f impulseForce;

	float mass;
	float drag;

	float rotation;
	float angularVelocity;
	float angularAcceleration;
	float torque;
	float impulseTorque;
	float inertia;
	float angularDrag;

	sf::Vector2f upVector;

	bool hasFlippedClockwise = false;

public:
	Rigidbody(sf::Shape* shape, sf::RenderWindow* window);

	void update(float dt, float* score);
	void draw();

	void setPosition(sf::Vector2f position);
	void setVelocity(sf::Vector2f velocity);
	void setAcceleration(sf::Vector2f acceleration);
	void setForce(sf::Vector2f force);
	void applyForce(sf::Vector2f force);
	void applyImpulseForce(sf::Vector2f impulseForce);

	void setMass(float mass);
	void setDrag(float drag);

	void setRotation(float rotation);
	void setAngularVelocity(float angularVelocity);
	void setAngularAcceleration(float angularAcceleration);
	void setTorque(float torque);
	void applyTorque(float torque);
	void applyImpulseTorque(float impulseTorque);
	void setInertia(float inertia);
	void setAngularDrag(float angularDrag);

	void applyImpulseForceAtOffset(sf::Vector2f impulseForce, sf::Vector2f offset);

	sf::Vector2f getPosition();
	sf::Vector2f getVelocity();
	sf::Vector2f getAcceleration();
	sf::Vector2f getForce();
	float getMass();
	float getDrag();

	float getRotation();
	float getAngularVelocity();
	float getAngularAcceleration();
	float getTorque();
	float getInertia();
	float getAngularDrag();

	sf::Vector2f getUpVector();
	sf::Shape* getShape();
	sf::Shape* getFireShape();

	sf::Vector2f getBoundingBoxSize();
	void setFillColor(sf::Color color);
	void setHasFlippedForwards(bool hasFlippedForwards);
};