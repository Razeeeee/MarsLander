#include <iostream>
#include <SFML/Graphics.hpp>

#include "gameplay.h"

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

    // Creating the gameplay
    Gameplay gameplay(&window);

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
            gameplay.handleEvent(event);
        }

        // Handling the physics updates
        while (accumulator >= dt)
        {
            gameplay.update(dt);

            accumulator -= dt;
        }

        // Drawing the gameplay
        gameplay.draw();
    }

    return 0;
}