#include "menu.h"

bool isMouseOverText(const sf::Text& text, const sf::RenderWindow& window) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    return text.getGlobalBounds().contains(mousePos);
}

void handleHelpButtonClick(GameState& gameState) {
    std::cout << "Help button pressed\n";
    gameState = HelpMenu;
    // Add your specific actions
}

void handleExitButtonClick(sf::RenderWindow& window) {
    std::cout << "Exit button pressed\n";
    window.close();
}

Menu::Menu(sf::RenderWindow* window, SceneManager* sceneManager) : window(window), sceneManager(sceneManager)
{
    gameState = MainMenu;
    if (!font.loadFromFile("assets/fonts/retro.ttf")) {
		std::cout << "Error loading font\n";
        exit(0);
	}

    if (!backgroundTexture.loadFromFile("assets/images/background.jpg")) {
        std::cout << "Error loading background texture\n";
        exit(0);
    }

    background.setTexture(backgroundTexture);

    title.setFont(font);
    title.setString("Celestial Lander");
    title.setCharacterSize(120);
    title.setFillColor(sf::Color::White);
    title.setPosition(window->getSize().x / 2 - title.getGlobalBounds().width / 2, 120);

    button.setFont(font);
    button.setString("START");
    button.setCharacterSize(60);
    button.setPosition(window->getSize().x / 2 - button.getGlobalBounds().width / 2,
        window->getSize().y / 2 - button.getGlobalBounds().height / 2);

    helpButton.setFont(font);
    helpButton.setString("HELP");
    helpButton.setCharacterSize(55);
    helpButton.setPosition(window->getSize().x / 2 - helpButton.getGlobalBounds().width - 60,
        window->getSize().y / 2 + button.getGlobalBounds().height + 50);

    exitButton.setFont(font);
    exitButton.setString("EXIT");
    exitButton.setCharacterSize(55);
    exitButton.setPosition(window->getSize().x / 2 + 60,
        window->getSize().y / 2 + button.getGlobalBounds().height + 50);

    levelDifficultyText.setFont(font);
    levelDifficultyText.setString("Difficulty level:");
    levelDifficultyText.setCharacterSize(50);
    sf::FloatRect textBounds = levelDifficultyText.getLocalBounds();
    levelDifficultyText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    levelDifficultyText.setPosition(window->getSize().x / 2.0f, 100);

    easyText.setFont(font);
    easyText.setString("Easy");
    easyText.setCharacterSize(40);
    easyText.setFillColor(sf::Color::White);
    sf::FloatRect easyBounds = easyText.getLocalBounds();
    easyText.setOrigin(easyBounds.left + easyBounds.width / 2.0f, easyBounds.top + easyBounds.height / 2.0f);
    easyText.setPosition(window->getSize().x / 2.0f, 200);

    hardText.setFont(font);
    hardText.setString("Hard");
    hardText.setCharacterSize(40);
    hardText.setFillColor(sf::Color::White);
    sf::FloatRect hardBounds = hardText.getLocalBounds();
    hardText.setOrigin(hardBounds.left + hardBounds.width / 2.0f, hardBounds.top + hardBounds.height / 2.0f);
    hardText.setPosition(window->getSize().x / 2.0f, 275);

    backToMenuText.setFont(font);
    backToMenuText.setString("Back to menu");
    backToMenuText.setCharacterSize(40);
    backToMenuText.setFillColor(sf::Color::White);
    sf::FloatRect BackToMenuBounds = backToMenuText.getLocalBounds();
    backToMenuText.setOrigin(BackToMenuBounds.left + BackToMenuBounds.width / 2.0f, BackToMenuBounds.top + BackToMenuBounds.height / 2.0f);
    backToMenuText.setPosition(window->getSize().x / 2.0f, 375);

    backToMenuHelpText.setFont(font);
    backToMenuHelpText.setString("Back to menu");
    backToMenuHelpText.setCharacterSize(40);
    backToMenuHelpText.setFillColor(sf::Color::White);
    sf::FloatRect BackToMenuHelpBounds = backToMenuHelpText.getLocalBounds();
    backToMenuHelpText.setOrigin(BackToMenuHelpBounds.left + BackToMenuHelpBounds.width / 2.0f, BackToMenuHelpBounds.top + BackToMenuHelpBounds.height / 2.0f);
    backToMenuHelpText.setPosition(BackToMenuHelpBounds.width + 10, 500);

    normalColor = sf::Color::White;
    hoverColor = sf::Color::Red;

    helpContent.setFont(font);
    helpContent.setCharacterSize(40);
    helpContent.setFillColor(sf::Color::White);
    helpContent.setString("Welcome to Celestial Lander!\n\n"
        "Controls:\n"
        "   - Left/Right Arrow - Rotating the lander\n"
        "   - Up Arrow - Applying thrust\n"
        "   - F7 - Resetting the level\n\n"

        "Press 'ESC' to go back to the main menu.");
    helpContent.setPosition(30, 30);

    SaveManager save("savegame");
    int moonHS = save.getMoonHS();
    int marsHS = save.getMarsHS();
    std::cout << "Gotten moon HS: " << moonHS << std::endl;
    std::cout << "Gotten mars HS: " << marsHS << std::endl;

    moonHSText.setFont(font);
    moonHSText.setCharacterSize(40);
    moonHSText.setFillColor(sf::Color::White);
    moonHSText.setString("Moon highscore: " + std::to_string(moonHS));
    moonHSText.setOrigin(moonHSText.getLocalBounds().width, moonHSText.getLocalBounds().height / 2);
    moonHSText.setPosition(window->getSize().x - 40, 30);

    marsHSText.setFont(font);
    marsHSText.setCharacterSize(40);
    marsHSText.setFillColor(sf::Color::White);
    marsHSText.setString("Mars highscore: " + std::to_string(marsHS));
    marsHSText.setOrigin(moonHSText.getLocalBounds().width, marsHSText.getLocalBounds().height / 2);
    marsHSText.setPosition(window->getSize().x - 40, 70);
}

void Menu::handleEvent(sf::Event event)
{
	if (event.type == sf::Event::Closed)
	{
		window->close();
	}

    switch (gameState) {
    case MainMenu:
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (isMouseOverText(button, *window)) {
                    gameState = DifficultySelection;
                }
                else if (isMouseOverText(helpButton, *window)) {
                    SaveManager save("savegame");
                    int moonHS = save.getMoonHS();
                    int marsHS = save.getMarsHS();
                    std::cout << "Gotten moon HS: " << moonHS << std::endl;
                    std::cout << "Gotten mars HS: " << marsHS << std::endl;
                    moonHSText.setString("Moon highscore: " + std::to_string(moonHS));
                    moonHSText.setOrigin(moonHSText.getLocalBounds().width, moonHSText.getLocalBounds().height / 2);
                    moonHSText.setPosition(window->getSize().x - 40, 30);
                    marsHSText.setString("Mars highscore: " + std::to_string(marsHS));
                    marsHSText.setOrigin(moonHSText.getLocalBounds().width, marsHSText.getLocalBounds().height / 2);
                    marsHSText.setPosition(window->getSize().x - 40, 70);
                    handleHelpButtonClick(gameState);
                }
                else if (isMouseOverText(exitButton, *window)) {
                    handleExitButtonClick(*window);
                }
            }
        }

        if (isMouseOverText(button, *window)) {
            button.setFillColor(hoverColor);
        }
        else {
            button.setFillColor(normalColor);
        }

        if (isMouseOverText(helpButton, *window)) {
            helpButton.setFillColor(hoverColor);
        }
        else {
            helpButton.setFillColor(normalColor);
        }

        if (isMouseOverText(exitButton, *window)) {
            exitButton.setFillColor(hoverColor);
        }
        else {
            exitButton.setFillColor(normalColor);
        }
        break;

    case DifficultySelection:
        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

            if (easyText.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                easyText.setFillColor(hoverColor);
            }
            else {
                easyText.setFillColor(normalColor);
            }

            if (hardText.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                hardText.setFillColor(hoverColor);
            }
            else {
                hardText.setFillColor(normalColor);
            }

            if (backToMenuText.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                backToMenuText.setFillColor(hoverColor);
            }
            else {
                backToMenuText.setFillColor(normalColor);
            }
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            gameState = MainMenu;
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(*window);

                if (easyText.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    std::cout << "Selected easy level\n";
                    sceneManager->changeScene(10);
                }
                else if (hardText.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    std::cout << "Selected hard level\n";
                    sceneManager->changeScene(20);
                }
                else if (backToMenuText.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    gameState = MainMenu;
                }
            }
        }
        break;

    case HelpMenu:
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (isMouseOverText(backToMenuHelpText, *window)) {
                    gameState = MainMenu;
                }
            }
        }

        if (isMouseOverText(backToMenuHelpText, *window)) {
            backToMenuHelpText.setFillColor(hoverColor);
        }
        else {
            backToMenuHelpText.setFillColor(normalColor);
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            gameState = MainMenu;
        }
        break;
    }
}

void Menu::update(float dt)
{

}

void Menu::render()
{
    window->clear();

    sf::View view(sf::FloatRect(0, 0, window->getSize().x, window->getSize().y));
    window->setView(view);   
    window->draw(background);

    switch (gameState) {
    case MainMenu:
        window->draw(title);
        window->draw(button);
        window->draw(helpButton);
        window->draw(exitButton);

        if (isMouseOverText(button, *window)) {
            sf::Text hoverTextStart("Press to start", font, 20);
            hoverTextStart.setPosition(window->getSize().x / 2 - hoverTextStart.getGlobalBounds().width / 2,
                window->getSize().y / 2 + button.getGlobalBounds().height / 2 + 30);
            hoverTextStart.setFillColor(sf::Color::White);
            window->draw(hoverTextStart);
        }
        break;

    case DifficultySelection:
        window->draw(levelDifficultyText);
        window->draw(easyText);
        window->draw(hardText);
        window->draw(backToMenuText);
        break;

    case HelpMenu:
        window->draw(helpContent);
        window->draw(moonHSText);
        window->draw(marsHSText);
        window->draw(backToMenuHelpText);
        break;
    }
}

void Menu::reset()
{
    std::cout << "Resetting menu\n";

	gameState = MainMenu;

    button.setFillColor(normalColor);
	helpButton.setFillColor(normalColor);
	exitButton.setFillColor(normalColor);
	easyText.setFillColor(normalColor);
	hardText.setFillColor(normalColor);
	backToMenuText.setFillColor(normalColor);
	backToMenuHelpText.setFillColor(normalColor);

    sf::FloatRect BackToMenuHelpBounds = backToMenuHelpText.getLocalBounds();
    title.setPosition(window->getSize().x / 2 - title.getGlobalBounds().width / 2, 120);
	button.setPosition(window->getSize().x / 2 - button.getGlobalBounds().width / 2,
        		window->getSize().y / 2 - button.getGlobalBounds().height / 2);
	helpButton.setPosition(window->getSize().x / 2 - helpButton.getGlobalBounds().width - 60,
        		window->getSize().y / 2 + button.getGlobalBounds().height + 50);
	exitButton.setPosition(window->getSize().x / 2 + 60,
        		window->getSize().y / 2 + button.getGlobalBounds().height + 50);
	levelDifficultyText.setPosition(window->getSize().x / 2.0f, 100);
	easyText.setPosition(window->getSize().x / 2.0f, 200);
	hardText.setPosition(window->getSize().x / 2.0f, 275);
	backToMenuText.setPosition(window->getSize().x / 2.0f, 375);
	backToMenuHelpText.setPosition(BackToMenuHelpBounds.width + 10, 500);
}
