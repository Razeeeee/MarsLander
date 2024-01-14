#include <iostream>
#include <SFML/Graphics.hpp>

#include "gameplay.h"
#include "menu.h"
#include "endMenu.h"
#include "sceneManager.h"

int main()
{
    const int WINDOW_WIDTH = 1920;
    const int WINDOW_HEIGHT = 1080;

    sf::ContextSettings settings;
    //settings.antialiasingLevel = 8;

    // Creating the window 
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mars Lander", sf::Style::Fullscreen, settings);
    // Changing the window settings
    window.setFramerateLimit(120);

    // Creating the scene manager
    SceneManager sceneManager;
    // Setting up the scenes
    Gameplay* mars = new Gameplay(&window, &sceneManager);
    mars->setID(20);
    Gameplay* moon = new Gameplay(&window, &sceneManager);
    Terrain* moonTerrain = moon->getTerrain();
    moonTerrain->setAmplitudeModifier(0.2f);
    moonTerrain->setFrequencyModifier(3000.0f);
    moonTerrain->setTopTerrainColor(sf::Color(177, 174, 181, 255));
    moonTerrain->setBottomTerrainColor(sf::Color(127, 122, 122, 255));
    moonTerrain->setTerrainLineColor(sf::Color(150, 153, 168, 255));
    moonTerrain->setTopBackgroundColor(sf::Color::Black);
    moonTerrain->setBottomBackgroundColor(sf::Color(53, 49, 49, 255));
    moon->setGravity(1500.0f);
    moon->setID(10);
    // Add menu to the scene manager, id = 1
    sceneManager.addScene(new Menu(&window, &sceneManager));
    // Add end menu to the scene manager, id = 2
    sceneManager.addScene(new EndMenu(&window, &sceneManager));
    // Add mars and moon to the scene manager, id = 10 and id = 20
    sceneManager.addScene(mars);
    sceneManager.addScene(moon);
 
    // Set the current scene to gameplay
    sceneManager.changeScene(1);

    // Clock for frame time
    sf::Clock clock;

    // Accumulator for physics updates
    float accumulator = 0.0f;
    const float dt = 1.0f / 60.0f;

    while (window.isOpen())
    {
        // Calculating the frame time and updating the accumulator
        float frameTime = clock.restart().asSeconds();
        accumulator += frameTime;

        // Handling events
        sf::Event event;
        while (window.pollEvent(event))
        {
            sceneManager.handleEvent(event);
        }

        // Handling the physics updates
        while (accumulator >= dt)
        {
            // Update using scene manager
            sceneManager.update(dt);

            accumulator -= dt;
        }

        // Drawing the gameplay
        sceneManager.render();

        window.display();
    }

    return 0;
}