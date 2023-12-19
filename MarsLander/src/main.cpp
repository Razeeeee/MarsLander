#include <iostream>
#include <SFML/Graphics.hpp>
#include "terrain.h"

int main()
{
    const int WINDOW_WIDTH = 1920;
    const int WINDOW_HEIGHT = 1080;

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mars Lander", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    terrain terrain(&window);
    terrain.setFrequencyModifier(2137.0f);
    terrain.setAmplitudeModifier(0.3f);
    terrain.buildTerrain();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        terrain.renderBackground();
        terrain.renderTerrain();
        terrain.drawLandingZoneCandidates();

        window.display();
    }

    return 0;
}