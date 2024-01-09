#include <iostream>
#include <SFML/Graphics.hpp>

#include "gameplay.h"

int main()
{
    const int WINDOW_WIDTH = 1920;
    const int WINDOW_HEIGHT = 1080;

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mars Lander", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    Gameplay gameplay(&window);

    // Clock for frame time
    sf::Clock clock;

    // Accumulator
    float accumulator = 0.0f;
    const float dt = 1.0f / 60.0f;

    while (window.isOpen())
    {
        // Frame time
        float frameTime = clock.restart().asSeconds();
        accumulator += frameTime;

        sf::Event event;
        while (window.pollEvent(event))
        {
            gameplay.handleEvent(event);
        }

        while (accumulator >= dt)
        {
            gameplay.update(dt);

            accumulator -= dt;
        }

        gameplay.draw();
    }

    return 0;
}