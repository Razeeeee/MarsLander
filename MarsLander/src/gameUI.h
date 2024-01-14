#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class GameUI
{
private:
	sf::RenderWindow* window;

	sf::Font font;
	sf::Text horizontalSpeedText;
	sf::Text verticalSpeedText;
	sf::Text altitudeText;
	sf::Text scoreText;

	sf::RectangleShape fuelBar;
	sf::RectangleShape fuelBarOutline;

public:
	GameUI(sf::RenderWindow* window);
	void update(sf::Vector2f velocity, float altitude, float fuel, float score);
	void draw();
};