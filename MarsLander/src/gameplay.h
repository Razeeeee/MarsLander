#pragma once
#include <SFML/Graphics.hpp>
#include "terrain.h"
#include "rigidbody.h"
#include "gameUI.h"
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
	GameUI* gameUI;
	sf::RenderWindow* window;
	KeyState keyState;

	sf::View mainView;
	sf::View zoomView;
	sf::RectangleShape zoomViewOutline;
	
	Terrain* terrain;
	int* landingZones;

	Rigidbody* player;
	float fuel;
	float score;

public:
	Gameplay(sf::RenderWindow* window);

	void handleEvent(sf::Event event);

	void update(float deltaTime);
	void draw();

	Rigidbody getPlayer() { return *player; }

};