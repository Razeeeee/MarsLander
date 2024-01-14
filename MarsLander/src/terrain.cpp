#include "terrain.h"
#include <iostream>

Terrain::Terrain(sf::RenderWindow* window) : renderWindow(window)
{
	// Getting the window size
	windowWidth = renderWindow->getSize().x;
	windowHeight = renderWindow->getSize().y;

	// Initializing the landing zone properties
	landingSize = sf::Vector2i((int)(windowWidth * 0.03f), (int)(windowWidth * 0.03f / 8));
	distanceBetweenLandingZones = windowWidth * 0.1f; // Min. distance between landing zones
	distanceFromEdge = windowWidth * 0.2f; // Min. distance from the edge of the screen
	interpolationWidth = landingSize.x * 0.25f; // Width of the area to interpolate around the landing zone
	for (int i = 0; i < 3; ++i)
	{
		landingZones[i] = 0; // Initializing the landing zones array
	}

	// Creating the terrain array
	terrainArray = new float[windowWidth];

	// Initializing the candidate values
	// They will be used to find the best landing zone candidates
	candidateValue1 = 1000000.0f;
	candidateValue2 = 1000000.0f;
	candidateValue3 = 1000000.0f;

	// Initializing the noise variables
	// Noise variables can be later changed to create difficulty terrain
	frequencyModifier = 1500.0f;
	amplitudeModifier = 0.3f;

	// Using the modifiers to generate the frequency and amplitude
	frequency = static_cast<float>(rand() % 2 + 4) / frequencyModifier;
	amplitude = windowHeight * amplitudeModifier;

	// Initializing a random seed
	srand(time(NULL));

	// Creating the Simplex noise variables
	x = static_cast<float>(rand() % 1000) / 1000.0f;
	y = static_cast<float>(rand() % 1000) / 1000.0f;
	z = static_cast<float>(rand() % 1000) / 1000.0f;
	noiseValue = 0.0f;
	lacunarity = 2.0f;
	persistence = 0.5f;
	octaves = 4;

	// Setting the colors
	// The colors are used to create a gradient for the background and the terrain
	topBackgroundColor = sf::Color(162, 150, 146, 255);
	bottomBackgroundColor = sf::Color(213, 202, 186, 255);
	topTerrainColor = sf::Color(172, 133, 105, 255);
	bottomTerrainColor = sf::Color(149, 103, 85, 255);
	terrainLineColor = sf::Color(171, 120, 96, 255);
}

void Terrain::generateTerrain()
{
	std::cout << "Generating terrain..." << std::endl;
	std::cout << "Amp multi: " << amplitudeModifier << ", freq multi: " << frequencyModifier << std::endl;
	SimplexNoise noise;

	// Calculate the frequency and amplitude again (could have been changed)
	frequency = static_cast<float>(rand() % 2 + 4) / frequencyModifier;
	amplitude = windowHeight * amplitudeModifier;

	// Generating the terrain using Simplex noise
	for (int i = 0; i < windowWidth; ++i)
	{
		noiseValue = noise.fractal(octaves, x, y, z);
		// The noise value is between -1 and 1
		// Adjusting the value to be properly placed on the screen
		terrainArray[i] = windowHeight - noiseValue * amplitude - amplitude;
		
		x += frequency;
	}
}

void Terrain::findLandingZones()
{
	/*
	Each candidate is found by finding the lowest difference in height between two points
	The first one has no other restrictions
	The second one cannot be within a certain distance of the first one
	The third one cannot be within a certain distance of the first or second one
	*/

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
		// For the whole length of the landing zone
		for (int j = landingZones[i]; j < landingZones[i] + landingSize.x; ++j)
		{
			// Flattening the landing zone
			terrainArray[j] = terrainArray[landingZones[i]];
		}
	}
}

void Terrain::interpolateLandingZoneEdges()
{
	std::cout << "Interpolating landing zone edges..." << std::endl;
	for (int i = 0; i < 3; ++i)
	{
		// Interpolating the area around the landing zone for smoother transitions using lerp
		for (int j = 0; j < interpolationWidth; ++j)
		{
			// Interpolate separate for the left and right edges
			terrainArray[landingZones[i] - j] = terrainArray[landingZones[i]] + (terrainArray[landingZones[i] - interpolationWidth] - terrainArray[landingZones[i]]) * (static_cast<float>(j) / static_cast<float>(interpolationWidth));
			terrainArray[landingZones[i] + landingSize.x + j] = terrainArray[landingZones[i] + landingSize.x] + (terrainArray[landingZones[i] + landingSize.x + interpolationWidth] - terrainArray[landingZones[i] + landingSize.x]) * (static_cast<float>(j) / static_cast<float>(interpolationWidth));
		}
	}
}

void Terrain::renderBackground()
{
	// Rendering the background as a vertical gradient
	sf::Color startColor = topBackgroundColor;
	sf::Color endColor = bottomBackgroundColor;
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
		// Rendering the terrain as a series of lines between points in the terrain array
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(i, terrainArray[i]), terrainLineColor),
			sf::Vertex(sf::Vector2f(i + 1, terrainArray[i + 1]), terrainLineColor)
		};

		if(i <= landingZones[0] || i >= landingZones[0] + landingSize.x)
			renderWindow->draw(line, 2, sf::Lines);

		// Filling in the area below the terrain curve with the gradient
		sf::Color startColor = topTerrainColor;
		sf::Color endColor = bottomTerrainColor;
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
	// Returns the landing zone candidates array
	for (int i = 0; i < 3; ++i)
	{
		candidates[i] = this->landingZones[i];
	}
}

void Terrain::renderLandingZones()
{
	for (int i = 0; i < 3; ++i)
	{
		// Rendering the landing zones as rectangles
		sf::RectangleShape rectangle(sf::Vector2f(landingSize.x, landingSize.x / 8));
		rectangle.setPosition(landingZones[i], terrainArray[landingZones[i]] - landingSize.x / 8);
		rectangle.setFillColor(sf::Color(103, 98, 96, 255));
		renderWindow->draw(rectangle);
	}
}

void Terrain::build()
{
	// Clock that measures the time it takes to build the terrain
	sf::Clock clock;
	clock.restart();

	// Building the terrain 
	generateTerrain();
	findLandingZones();
	flattenLandingZones();
	interpolateLandingZoneEdges();

	std::cout << "Terrain built in " << clock.getElapsedTime().asSeconds() << " seconds." << std::endl;
}

void Terrain::reset()
{
	std::cout << "Resetting terrain..." << std::endl;
	// Resetting the candidate values
	candidateValue1 = 1000000.0f;
	candidateValue2 = 1000000.0f;
	candidateValue3 = 1000000.0f;

	// Resetting the noise variables
	x = static_cast<float>(rand() % 1000) / 1000.0f;
	y = static_cast<float>(rand() % 1000) / 1000.0f;
	z = static_cast<float>(rand() % 1000) / 1000.0f;

	// Resetting the landing zones array
	for (int i = 0; i < 3; ++i)
	{
		landingZones[i] = 0;
	}

	// Resetting the terrain array
	for (int i = 0; i < windowWidth; ++i)
	{
		terrainArray[i] = 0.0f;
	}

	// Rebuilding the terrain
	build();
}

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

void Terrain::setTopBackgroundColor(sf::Color topBackgroundColor)
{
	this->topBackgroundColor = topBackgroundColor;
}

void Terrain::setBottomBackgroundColor(sf::Color bottomBackgroundColor)
{
	this->bottomBackgroundColor = bottomBackgroundColor;
}

void Terrain::setTopTerrainColor(sf::Color topTerrainColor)
{
	this->topTerrainColor = topTerrainColor;
}

void Terrain::setBottomTerrainColor(sf::Color bottomTerrainColor)
{
	this->bottomTerrainColor = bottomTerrainColor;
}

void Terrain::setTerrainLineColor(sf::Color terrainLineColor)
{
	this->terrainLineColor = terrainLineColor;
}