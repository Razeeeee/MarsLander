#pragma once
#include <SFML/Graphics.hpp>
#include "terrain.h"
#include "rigidbody.h"
#include <iostream>
#include <vector>

struct KeyState
{
	bool isUpPressed = false;
	bool isLeftPressed = false;
	bool isRightPressed = false;
};

class Gameplay
{
private:
	sf::RenderWindow* window;
	KeyState buttonState;

	sf::View mainView;
	sf::View zoomView;
	
	Terrain* terrain;
	int* landingZones;

	Rigidbody* player;

public:
	Gameplay(sf::RenderWindow* window);

	void handleEvent(sf::Event event);

	void update(float deltaTime);
	void draw();

	Rigidbody getPlayer() { return *player; }

};