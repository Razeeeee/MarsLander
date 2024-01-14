#pragma once
#include "SimplexNoise.h"
#include "SFML/Graphics.hpp"

class Terrain
{
public:
	Terrain(sf::RenderWindow* window);

	// Rendering the terrain, background and landing zone candidates
	void renderTerrain();
	void renderBackground();
	void renderLandingZones();

	// Building and resetting the terrain
	void build();
	void reset();

	// Getters
	void getLandingZones(int* landingZones);
	sf::Vector2i getLandingSize();
	float getTerrainYatX(int x);

	// Setters
	void setFrequencyModifier(float frequencyModifier);
	void setAmplitudeModifier(float amplitudeModifier);

	void setTopBackgroundColor(sf::Color topBackgroundColor);
	void setBottomBackgroundColor(sf::Color bottomBackgroundColor);
	void setTopTerrainColor(sf::Color topTerrainColor);
	void setBottomTerrainColor(sf::Color bottomTerrainColor);
	void setTerrainLineColor(sf::Color terrainLineColor);

private:
	// Terrain generation and landing zone candidate finding
	void generateTerrain();
	void findLandingZones();
	void flattenLandingZones();
	void interpolateLandingZoneEdges();

	sf::RenderWindow* renderWindow;
	int windowWidth;
	int windowHeight;

	float frequencyModifier;
	float amplitudeModifier;

	sf::Vector2i landingSize;
	int distanceBetweenLandingZones;
	int distanceFromEdge;
	int interpolationWidth;

	int landingZones[3];
	float candidateValue1;
	float candidateValue2;
	float candidateValue3;
	float* terrainArray;

	float x;
	float y;
	float z;
	float noiseValue;
	float frequency;
	float amplitude;
	float lacunarity;
	float persistence;
	int octaves;

	sf::Color topBackgroundColor;
	sf::Color bottomBackgroundColor;
	sf::Color topTerrainColor;
	sf::Color bottomTerrainColor;
	sf::Color terrainLineColor;
};