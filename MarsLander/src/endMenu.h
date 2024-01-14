#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "sceneManager.h"

class EndMenu : public Scene
{
private:
	sf::RenderWindow* window;
	SceneManager* sceneManager;
	sf::Font font;

	sf::Text gameOverText;
	sf::Text scoreText;
	sf::Text backToMenuText;

	int score;

public:
	EndMenu(sf::RenderWindow* window, SceneManager* sceneManager);

	void handleEvent(sf::Event event) override;
	void update(float dt) override;
	void render() override;
	int getID() const override { return 2; }
	void reset() override;
};

