#pragma once
#include "SimplexNoise.h"
#include "SFML/Graphics.hpp"

class terrain
{
public:
	// constructor
	terrain(sf::RenderWindow* window);
	// rendering the terrain
	void renderTerrain();
	// rendering the background
	void renderBackground();
	// return the landing zone candidates array
	void getLandingZoneCandidates(int* candidates);
	// draw the landing zone candidates
	void drawLandingZoneCandidates();
	// build terrain
	void buildTerrain();
	// reset
	void reset();

	// getters and setters
	void setFrequencyModifier(float frequencyModifier);
	void setAmplitudeModifier(float amplitudeModifier);

private:
	// generate the terrain
	void generateTerrain();
	// find the best landing zone candidates
	void findLandingZoneCandidates();
	// flatten the landing zones
	void flattenLandingZones();
	// interpolate the area around the landing zones for smoother transitions
	void interpolateLandingZoneEdges();

protected:
	// window variables
	sf::RenderWindow* renderWindow;
	int windowWidth;
	int windowHeight;

	// terrain generation variables
	float frequencyModifier;
	float amplitudeModifier;

	// landing zone variables
	int landingWidth;
	int distanceBetweenLandingZones;
	int distanceFromEdge;
	int interpolationWidth;
	int candidates[3];
	float candidateValue1;
	float candidateValue2;
	float candidateValue3;
	float* terrainArray;

	// noise variables
	float x;
	float y;
	float z;
	float noiseValue;
	float frequency;
	float amplitude;
	float lacunarity;
	float persistence;
	int octaves;
};

