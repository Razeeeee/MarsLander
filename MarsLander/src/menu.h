#pragma once
#include "sceneManager.h"
#include <SFML/Graphics.hpp>
#include <iostream>

enum GameState {
	MainMenu,
	DifficultySelection,
	HelpMenu,
};

class Menu : public Scene
{
private:
	sf::RenderWindow* window;
	SceneManager* sceneManager;
	GameState gameState;
	sf::Font font;
	sf::Texture backgroundTexture;
	sf::Sprite background;

	sf::Text title;
	sf::Text levelDifficultyText;
	sf::Text easyText;
	sf::Text hardText;
	sf::Text backToMenuText;
	sf::Text backToMenuHelpText;

	sf::Text button;
	sf::Text helpButton;
	sf::Text exitButton;
	sf::Text playButton;

	sf::Text helpContent;

	sf::Color normalColor;
	sf::Color hoverColor;

public:
	Menu(sf::RenderWindow* window, SceneManager* sceneManager);

	void handleEvent(sf::Event event) override;
	void update(float dt) override;
	void render() override;
	int getID() const override { return 1; }
	void reset() override;
};

