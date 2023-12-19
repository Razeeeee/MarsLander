#include <iostream>
#include <SFML/Graphics.hpp>
#include "terrain.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Mars Lander", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    terrain terrain(&window);
    terrain.setFrequencyModifier(1500.0f);
    terrain.setAmplitudeModifier(0.2f);
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

        // clear the background as a vertical gradient from sf::Color(162, 150, 146, 255) to sf::Color(213, 202, 186, 255)
        sf::Color startColor(162, 150, 146, 255);
        sf::Color endColor(213, 202, 186, 255);
        sf::VertexArray background(sf::Quads, 4);
        background[0].position = sf::Vector2f(0, 0);
        background[1].position = sf::Vector2f(WINDOW_WIDTH, 0);
        background[2].position = sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT);
        background[3].position = sf::Vector2f(0, WINDOW_HEIGHT);
        background[0].color = startColor;
        background[1].color = startColor;
        background[2].color = endColor;
        background[3].color = endColor;
        window.draw(background);

        terrain.renderTerrain();
        terrain.drawLandingZoneCandidates();

        window.display();
    }

    return 0;
}