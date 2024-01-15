#include "gameUI.h"

GameUI::GameUI(sf::RenderWindow* window) : window(window)
{
	// Loading the font
	font = sf::Font();
	if (!font.loadFromFile("assets/fonts/retro.ttf"))
	{
		std::cout << "Error loading font" << std::endl;
		exit(0);
	}

	// Setting up the text
	horizontalSpeedText = sf::Text();
	horizontalSpeedText.setFont(font);
	horizontalSpeedText.setCharacterSize(30);
	horizontalSpeedText.setFillColor(sf::Color::White);
	horizontalSpeedText.setPosition(10, 10);

	verticalSpeedText = sf::Text();
	verticalSpeedText.setFont(font);
	verticalSpeedText.setCharacterSize(30);
	verticalSpeedText.setFillColor(sf::Color::White);
	verticalSpeedText.setPosition(10, 46);

	altitudeText = sf::Text();
	altitudeText.setFont(font);
	altitudeText.setCharacterSize(30);
	altitudeText.setFillColor(sf::Color::White);
	altitudeText.setPosition(10, 82);

	scoreText = sf::Text();
	scoreText.setFont(font);
	scoreText.setCharacterSize(30);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(10, 118);

	// Setting up the fuel bar
	fuelBar = sf::RectangleShape(sf::Vector2f(20.0f, window->getSize().y / 2));
	fuelBar.setOrigin(fuelBar.getSize().x / 2, fuelBar.getSize().y);
	fuelBar.setPosition(20, window->getSize().y / 2 - fuelBar.getSize().y / 2);
	fuelBar.setFillColor(sf::Color::White);
	fuelBar.rotate(180.0f);

	// Setting up the fuel bar outline
	fuelBarOutline = sf::RectangleShape(sf::Vector2f(20.0f, window->getSize().y / 2));
	fuelBarOutline.setOrigin(fuelBarOutline.getSize().x / 2, fuelBarOutline.getSize().y);
	fuelBarOutline.setPosition(20, window->getSize().y / 2 + fuelBarOutline.getSize().y / 2);
	fuelBarOutline.setFillColor(sf::Color::Transparent);
	fuelBarOutline.setOutlineColor(sf::Color::White);
	fuelBarOutline.setOutlineThickness(4.0f);
}

void GameUI::update(sf::Vector2f velocity, float altitudeAboveTerrain, float fuel, float score, sf::Vector2f position)
{
	if (position.x > window->getSize().x * 0.55f && position.y < window->getSize().y * 0.45f)
	{
		// set origin of text to the right
		horizontalSpeedText.setOrigin(horizontalSpeedText.getLocalBounds().width, 0);
		verticalSpeedText.setOrigin(verticalSpeedText.getLocalBounds().width, 0);
		altitudeText.setOrigin(altitudeText.getLocalBounds().width, 0);
		scoreText.setOrigin(scoreText.getLocalBounds().width, 0);
		// set position of text to the right
		horizontalSpeedText.setPosition(window->getSize().x - 10, 10);
		verticalSpeedText.setPosition(window->getSize().x - 10, 46);
		altitudeText.setPosition(window->getSize().x - 10, 82);
		scoreText.setPosition(window->getSize().x - 10, 118);

		// set fuel bar origin to the right
		fuelBar.setPosition(window->getSize().x - 20, window->getSize().y / 2 - fuelBarOutline.getSize().y / 2);
		fuelBarOutline.setPosition(window->getSize().x - 20, window->getSize().y / 2 + fuelBarOutline.getSize().y / 2);
	}
	else
	{
		// set origin of text to the left
		horizontalSpeedText.setOrigin(0, 0);
		verticalSpeedText.setOrigin(0, 0);
		altitudeText.setOrigin(0, 0);
		scoreText.setOrigin(0, 0);
		// set position of text to the left
		horizontalSpeedText.setPosition(10, 10);
		verticalSpeedText.setPosition(10, 46);
		altitudeText.setPosition(10, 82);
		scoreText.setPosition(10, 118);

		// set fuel bar origin to the left
		fuelBar.setPosition(20, window->getSize().y / 2 - fuelBarOutline.getSize().y / 2);
		fuelBarOutline.setPosition(20, window->getSize().y / 2 + fuelBarOutline.getSize().y / 2);
	}

	// Calculating the absolute value of the velocity as an integer
	sf::Vector2i velocityIntAbs = sf::Vector2i(std::abs(velocity.x), std::abs(velocity.y));
	// Updating the text
	if(velocityIntAbs.x <= 10) horizontalSpeedText.setFillColor(sf::Color::Green);
	else horizontalSpeedText.setFillColor(sf::Color::White);
	if(velocityIntAbs.y <= 10) verticalSpeedText.setFillColor(sf::Color::Green);
	else verticalSpeedText.setFillColor(sf::Color::White);
	horizontalSpeedText.setString("Horizontal Speed: " + std::to_string(velocityIntAbs.x) + " m/s");
	verticalSpeedText.setString("Vertical Speed: " + std::to_string(velocityIntAbs.y) + " m/s");

	// Calculating the absolute value of the altitude as an integer
	int altitude = (int)fabs(altitudeAboveTerrain);
	// Making sure the altitude is not negative
	if(altitude < 0) altitude = 0;
	// Updating the text
	altitudeText.setString("Altitude: " + std::to_string(altitude) + " m");

	// Updating the fuel bar
	fuelBar.setSize(sf::Vector2f(20.0f, window->getSize().y / 2 * (fuel / 100.0f)));

	// Updating the score text
	scoreText.setString("Score: " + std::to_string((int)score));
}

void GameUI::draw()
{
	// Drawing the text
	window->draw(horizontalSpeedText);
	window->draw(verticalSpeedText);
	window->draw(altitudeText);
	window->draw(scoreText);

	// Drawing the fuel bar
	window->draw(fuelBar);
	window->draw(fuelBarOutline);
}