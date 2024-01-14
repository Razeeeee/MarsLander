#pragma once
#include <SFML/Graphics.hpp>
#include "terrain.h"
#include "rigidbody.h"
#include "gameUI.h"
#include "sceneManager.h"
#include <iostream>
#include <vector>

struct KeyState
{
	bool isUpPressed = false;
	bool isLeftPressed = false;
	bool isRightPressed = false;
};

class Gameplay : public Scene
{
private:
	GameUI* gameUI;
	sf::RenderWindow* window;
	SceneManager* sceneManager;
	KeyState keyState;

	sf::View mainView;
	sf::View zoomView;
	sf::RectangleShape zoomViewOutline;
	
	Terrain* terrain;
	int* landingZones;

	Rigidbody* player;
	float thrust;
	float fuel;
	float score;

	float gameEndTimer;

	int levelID = 0;

public:
	Gameplay(sf::RenderWindow* window, SceneManager* sceneManager);

	void handleEvent(sf::Event event) override;

	// override Scene update and draw
	void update(float deltaTime) override;
	void render() override;
	int getID() const override;
	void setID(int id);

	void reset() override;

	void setGravity(float gravity);

	Rigidbody getPlayer() { return *player; }
	Terrain* getTerrain() { return terrain; }

};