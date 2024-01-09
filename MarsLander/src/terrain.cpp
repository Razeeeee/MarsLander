#include "terrain.h"
#include <iostream>

Terrain::Terrain(sf::RenderWindow* window) : renderWindow(window)
{
	// get the window width and height
	windowWidth = renderWindow->getSize().x;
	windowHeight = renderWindow->getSize().y;

	// initialize the landing zone variables
	landingSize = sf::Vector2i((int)(windowWidth * 0.03f), (int)(windowWidth * 0.03f / 8));
	distanceBetweenLandingZones = windowWidth * 0.1f; // min distance between landing zones
	distanceFromEdge = windowWidth * 0.2f; // min distance from the edge of the screen

	// initialize the terrain array
	terrainArray = new float[windowWidth];

	// initialize the candidate values
	// will be used to find the best landing zone candidates
	candidateValue1 = 1000000.0f;
	candidateValue2 = 1000000.0f;
	candidateValue3 = 1000000.0f;
	
	// initialize the interpolation width
	// the width of the area around the landing zone that will be interpolated
	interpolationWidth = landingSize.x * 0.25f;

	// initialize the noise variables - change for different results and/or difficulty
	frequencyModifier = 1500.0f;
	amplitudeModifier = 0.3f;

	frequency = static_cast<float>(rand() % 2 + 4) / frequencyModifier;
	amplitude = windowHeight * amplitudeModifier;

	// initialize the random seed
	srand(time(NULL));

	// initialize the noise variables
	x = static_cast<float>(rand() % 1000) / 1000.0f;
	y = static_cast<float>(rand() % 1000) / 1000.0f;
	z = static_cast<float>(rand() % 1000) / 1000.0f;
	noiseValue = 0.0f;
	lacunarity = 2.0f;
	persistence = 0.5f;
	octaves = 4;

	// initialize the landing zone candidates array
	for (int i = 0; i < 3; ++i)
	{
		landingZones[i] = 0;
	}
}

void Terrain::generateTerrain()
{
	std::cout << "Generating terrain..." << std::endl;
	SimplexNoise noise;

	// generate the frequency and amplitude
	frequency = static_cast<float>(rand() % 2 + 4) / frequencyModifier;
	amplitude = windowHeight * amplitudeModifier;

	// generate the terrain
	for (int i = 0; i < windowWidth; ++i)
	{
		noiseValue = noise.fractal(octaves, x, y, z);
		// the noise value is between -1 and 1
		// to make terrain just above the bottom of the screen, we adjust the value
		terrainArray[i] = windowHeight - noiseValue * amplitude - amplitude;
		
		x += frequency;
	}
}

void Terrain::findLandingZones()
{
	// each candidate is found by finding the lowest difference in height between two points
	// the first one has no other restrictions
	// the second one cannot be within a certain distance of the first one
	// the third one cannot be within a certain distance of the first or second one

	std::cout << "Finding landing zone candidates..." << std::endl;
	for (int i = distanceFromEdge; i < windowWidth - landingSize.x - distanceFromEdge; ++i)
	{
		if (fabs(terrainArray[i] - terrainArray[i + landingSize.x]) < candidateValue1)
		{
			candidateValue1 = fabs(terrainArray[i] - terrainArray[i + landingSize.x]);
			landingZones[0] = i;
		}
	}

	for (int i = distanceFromEdge; i < windowWidth - landingSize.x - distanceFromEdge; ++i)
	{
		if (i < landingZones[0] - distanceBetweenLandingZones || i > landingZones[0] + distanceBetweenLandingZones)
		{
			if (fabs(terrainArray[i] - terrainArray[i + landingSize.x]) < candidateValue2)
			{
				candidateValue2 = fabs(terrainArray[i] - terrainArray[i + landingSize.x]);
				landingZones[1] = i;
			}
		}
	}

	for (int i = distanceFromEdge; i < windowWidth - landingSize.x - distanceFromEdge; ++i)
	{
		if (i < landingZones[0] - distanceBetweenLandingZones || i > landingZones[0] + distanceBetweenLandingZones)
		{
			if (i < landingZones[1] - distanceBetweenLandingZones || i > landingZones[1] + distanceBetweenLandingZones)
			{
				if (fabs(terrainArray[i] - terrainArray[i + landingSize.x]) < candidateValue3)
				{
					candidateValue3 = fabs(terrainArray[i] - terrainArray[i + landingSize.x]);
					landingZones[2] = i;
				}
			}
		}
	}

	std::cout << "Candidate 1: " << landingZones[0] << std::endl;
	std::cout << "Candidate 2: " << landingZones[1] << std::endl;
	std::cout << "Candidate 3: " << landingZones[2] << std::endl;
}

void Terrain::flattenLandingZones()
{
	std::cout << "Flattening landing zones..." << std::endl;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = landingZones[i]; j < landingZones[i] + landingSize.x; ++j)
		{
			// flattening the landing zone to the height of the first point
			terrainArray[j] = terrainArray[landingZones[i]];
		}
	}
}

void Terrain::interpolateLandingZoneEdges()
{
	std::cout << "Interpolating landing zone edges..." << std::endl;
	for (int i = 0; i < 3; ++i)
	{
		// interpolating the area around the landing zone for smoother transitions
		// using linear interpolation
		for (int j = 0; j < interpolationWidth; ++j)
		{
			// interpolate separate for the left and right edges
			terrainArray[landingZones[i] - j] = terrainArray[landingZones[i]] + (terrainArray[landingZones[i] - interpolationWidth] - terrainArray[landingZones[i]]) * (static_cast<float>(j) / static_cast<float>(interpolationWidth));
			terrainArray[landingZones[i] + landingSize.x + j] = terrainArray[landingZones[i] + landingSize.x] + (terrainArray[landingZones[i] + landingSize.x + interpolationWidth] - terrainArray[landingZones[i] + landingSize.x]) * (static_cast<float>(j) / static_cast<float>(interpolationWidth));
		}
	}
}

void Terrain::renderBackground()
{
	sf::Color startColor(162, 150, 146, 255);
	sf::Color endColor(213, 202, 186, 255);
	sf::VertexArray background(sf::Quads, 4);
	background[0].position = sf::Vector2f(0, 0);
	background[1].position = sf::Vector2f(windowWidth, 0);
	background[2].position = sf::Vector2f(windowWidth, windowHeight);
	background[3].position = sf::Vector2f(0, windowHeight);
	background[0].color = startColor;
	background[1].color = startColor;
	background[2].color = endColor;
	background[3].color = endColor;
	renderWindow->draw(background);
}

void Terrain::renderTerrain()
{
	for (int i = 0; i < windowWidth - 1; ++i)
	{
		// renders the terrain as a series of lines between points in the terrain array
		// doesn't draw the landing zone candidates
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(i, terrainArray[i]), sf::Color(171, 120, 96, 255)),
			sf::Vertex(sf::Vector2f(i + 1, terrainArray[i + 1]), sf::Color(171, 120, 96, 255))
		};

		if(i <= landingZones[0] || i >= landingZones[0] + landingSize.x)
			renderWindow->draw(line, 2, sf::Lines);

		// fill in the area below the terrain curve
		// make it a vertical gradient from sf::Color(172,133,105,255) to sf::Color(149,103,85,255)
		sf::Color startColor(172, 133, 105, 255);
		sf::Color endColor(149, 103, 85, 255);
		sf::VertexArray background(sf::Quads, 4);
		background[0].position = sf::Vector2f(i, terrainArray[i]);
		background[1].position = sf::Vector2f(i + 1, terrainArray[i + 1]);
		background[2].position = sf::Vector2f(i + 1, windowHeight);
		background[3].position = sf::Vector2f(i, windowHeight);
		background[0].color = startColor;
		background[1].color = startColor;
		background[2].color = endColor;
		background[3].color = endColor;
		renderWindow->draw(background);
	}
}

void Terrain::getLandingZones(int* candidates)
{
	// returns the landing zone candidates array
	for (int i = 0; i < 3; ++i)
	{
		candidates[i] = this->landingZones[i];
	}
}

void Terrain::renderLandingZones()
{
	for (int i = 0; i < 3; ++i)
	{
		sf::RectangleShape rectangle(sf::Vector2f(landingSize.x, landingSize.x / 8));
		rectangle.setPosition(landingZones[i], terrainArray[landingZones[i]] - landingSize.x / 8);
		rectangle.setFillColor(sf::Color(103, 98, 96, 255));
		renderWindow->draw(rectangle);
	}
}

void Terrain::build()
{
	// clock that measures the time it takes to build the terrain
	sf::Clock clock;
	clock.restart();

	// combine the functions above
	generateTerrain();
	findLandingZones();
	flattenLandingZones();
	interpolateLandingZoneEdges();

	std::cout << "Terrain built in " << clock.getElapsedTime().asSeconds() << " seconds." << std::endl;
}

void Terrain::reset()
{
	// reset the candidate values
	candidateValue1 = 1000000.0f;
	candidateValue2 = 1000000.0f;
	candidateValue3 = 1000000.0f;

	// reset the noise variables
	x = static_cast<float>(rand() % 1000) / 1000.0f;
	y = static_cast<float>(rand() % 1000) / 1000.0f;
	z = static_cast<float>(rand() % 1000) / 1000.0f;

	// reset the landing zone candidates array
	for (int i = 0; i < 3; ++i)
	{
		landingZones[i] = 0;
	}

	// reset the terrain array
	for (int i = 0; i < windowWidth; ++i)
	{
		terrainArray[i] = 0.0f;
	}

	// rebuild the terrain
	build();
}

// getters and setters
void Terrain::setFrequencyModifier(float frequencyModifier)
{
	this->frequencyModifier = frequencyModifier;
}

void Terrain::setAmplitudeModifier(float amplitudeModifier)
{
	this->amplitudeModifier = amplitudeModifier;
}

sf::Vector2i Terrain::getLandingSize()
{
	return landingSize;
}

float Terrain::getTerrainYatX(int x)
{
	return terrainArray[x];
}