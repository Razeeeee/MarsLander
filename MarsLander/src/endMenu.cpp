#include "endMenu.h"

bool contains(const sf::FloatRect& rect, float x, float y) {
    return rect.left <= x && x <= rect.left + rect.width &&
        rect.top <= y && y <= rect.top + rect.height;
}

EndMenu::EndMenu(sf::RenderWindow* window, SceneManager* sceneManager) : window(window), sceneManager(sceneManager)
{
	score = 0;

	if (!font.loadFromFile("assets/fonts/retro.ttf")) {
		std::cout << "Error loading font\n";
		exit(0);
	}

	gameOverText.setFont(font);
	gameOverText.setString("Game Over");
	gameOverText.setCharacterSize(100);
	gameOverText.setFillColor(sf::Color::White);
	gameOverText.setOrigin(gameOverText.getLocalBounds().width / 2, gameOverText.getLocalBounds().height / 2);
	gameOverText.setPosition(window->getSize().x / 2, window->getSize().y / 2 - 120);

	scoreText.setFont(font);
	scoreText.setString("Score: " + std::to_string(score));
	scoreText.setCharacterSize(50);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setOrigin(scoreText.getLocalBounds().width / 2, scoreText.getLocalBounds().height / 2);
	scoreText.setPosition(window->getSize().x / 2, window->getSize().y / 2);

	backToMenuText.setFont(font);
	backToMenuText.setString("Back to Menu");
	backToMenuText.setCharacterSize(50);
	backToMenuText.setFillColor(sf::Color::White);
	backToMenuText.setOrigin(backToMenuText.getLocalBounds().width / 2, backToMenuText.getLocalBounds().height / 2);
	backToMenuText.setPosition(window->getSize().x / 2, window->getSize().y / 2 + 95);
}

void EndMenu::handleEvent(sf::Event event)
{
	// handle event close
	if (event.type == sf::Event::Closed) {
		window->close();
	}

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2f mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
		if (contains(backToMenuText.getGlobalBounds(), mousePosition.x, mousePosition.y)) {
			std::cout << "Back to Menu\n";
			sceneManager->changeScene(1);
		}
	}
}

void EndMenu::update(float dt)
{
}

void EndMenu::render()
{
	scoreText.setString("Score: " + std::to_string(sceneManager->getScore()));
	scoreText.setOrigin(scoreText.getLocalBounds().width / 2, scoreText.getLocalBounds().height / 2);
	scoreText.setPosition(window->getSize().x / 2, window->getSize().y / 2);

	sf::View view(sf::FloatRect(0, 0, window->getSize().x, window->getSize().y));
	window->setView(view);

	sf::Vector2f mousePosition = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	if (contains(backToMenuText.getGlobalBounds(), mousePosition.x, mousePosition.y)) {
		backToMenuText.setFillColor(sf::Color::Red);
	}
	else {
		backToMenuText.setFillColor(sf::Color::White);
	}

	window->clear(sf::Color::Black);
	window->draw(gameOverText);
	window->draw(scoreText);
	window->draw(backToMenuText);
}

void EndMenu::reset()
{
}