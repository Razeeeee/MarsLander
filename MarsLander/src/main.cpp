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
    terrain.setFrequencyModifier(2137.0f); // best: 2137.0f
    terrain.setAmplitudeModifier(0.420f); // best: 0.3f
    terrain.buildTerrain();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // on press F5, rebuild the terrain
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F5)
            {
                terrain.reset();
			}
        }

        window.clear();

        terrain.renderBackground();
        terrain.renderTerrain();
        terrain.drawLandingZoneCandidates();


        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::View view(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
        sf::View zoom(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
        // set zoom center to the mouse pos using mapPixelToCoords
        zoom.setCenter(window.mapPixelToCoords(mousePos, view));
        zoom.zoom(0.05f);

        view.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
        window.setView(view);
        terrain.renderBackground();
        terrain.renderTerrain();
        terrain.drawLandingZoneCandidates();

        // make the zoom viewPort 0.25 width and 0.25 height and make it's center at mouse pos
        zoom.setViewport(sf::FloatRect(mousePos.x / (float)WINDOW_WIDTH - 0.125f, mousePos.y / (float)WINDOW_HEIGHT - 0.125f, 0.25f, 0.25f));
        window.setView(zoom);
        terrain.renderBackground();
        terrain.renderTerrain();
        terrain.drawLandingZoneCandidates();


        window.display();
    }

    return 0;
}