#include "gameplay.h"

Gameplay::Gameplay(sf::RenderWindow* window) : window(window)
{
	// Setting up the game UI
	gameUI = new GameUI(window);
	// Setting up the key state (up, left, right arrows)
	keyState = { false, false, false };

	// Setting up the main view
	mainView = sf::View(sf::FloatRect(0, 0, window->getSize().x, window->getSize().y));
	mainView.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));

	// Setting up the zoomed view
	zoomView = sf::View(sf::FloatRect(0, 0, window->getSize().x, window->getSize().y));
	zoomView.setViewport(sf::FloatRect(0.6f, 0.0f, 0.4f, 0.4f));
	zoomView.zoom(0.15f);

	// Setting up the zoomed view outline
	zoomViewOutline = sf::RectangleShape(sf::Vector2f(0.4f * window->getSize().x, 0.4f * window->getSize().y));
	zoomViewOutline.setOrigin(zoomViewOutline.getSize().x / 2, zoomViewOutline.getSize().y / 2);
	zoomViewOutline.setPosition(window->getSize().x * 0.8f, window->getSize().y * 0.2f);
	zoomViewOutline.setFillColor(sf::Color::Transparent);
	zoomViewOutline.setOutlineColor(sf::Color::White);
	zoomViewOutline.setOutlineThickness(4.0f);

	// Creating and building the terrain
	terrain = new Terrain(window);
	terrain->setAmplitudeModifier(0.3f);
	terrain->setFrequencyModifier(2137.0f);
	terrain->build();
	
	// Getting the landing zones from the terrain
	landingZones = new int[3];
	terrain->getLandingZones(landingZones);
	
	// Creating the player's SFML shape
	sf::RectangleShape* playerShape = new sf::RectangleShape(sf::Vector2f(20.0f, 20.0f));
	playerShape->setOrigin(playerShape->getSize().x / 2, playerShape->getSize().y / 2);
	playerShape->setPosition(window->getSize().x / 6, window->getSize().y / 6);

	// Creating the player rigidbody
	player = new Rigidbody(playerShape, window);

	// Setting the player rigidbody's properties
	player->applyForce(sf::Vector2f(0, 3000.0f));
	player->setVelocity(sf::Vector2f(100.0f, 0));
	player->setRotation(270);
	player->setDrag(0.03f);
	player->setAngularDrag(0.1f);

	// Setting the player's fuel
	fuel = 100.0f;
	score = 0.0f;
	thrust = 0.0f;
	// Setting the player's mass
	player->setMass(500.0f + fuel);

	gameEndTimer = 0.0f;
}

void Gameplay::update(float deltaTime)
{
	thrust -= deltaTime * 1.5f;
	if(thrust < 0.0f) thrust = 0.0f;
	if(fuel > 0.0f) player->getFireShape()->setScale(sf::Vector2f(1, thrust));
	else player->getFireShape()->setScale(sf::Vector2f(0, 0));

	// Getting the player's position and size
    sf::Vector2f playerPos = player->getPosition();
    sf::Vector2f playerSize = player->getBoundingBoxSize();

    int landingZoneWidth = terrain->getLandingSize().x;

	// Calculating the player's cos(rotation) and sin(rotation) 
	float playerRotRad = player->getRotation() * (3.14159f / 180.0f);
	float cosA = std::cos(playerRotRad);
	float sinA = std::sin(playerRotRad);
	// Calculating the player's corner offsets
	sf::Vector2f corners[4] = {
		playerPos + sf::Vector2f(-playerSize.x / 2 * cosA + playerSize.y / 2 * sinA, -playerSize.x / 2 * sinA - playerSize.y / 2 * cosA),
		playerPos + sf::Vector2f(playerSize.x / 2 * cosA + playerSize.y / 2 * sinA, playerSize.x / 2 * sinA - playerSize.y / 2 * cosA),
		playerPos + sf::Vector2f(-playerSize.x / 2 * cosA - playerSize.y / 2 * sinA, -playerSize.x / 2 * sinA + playerSize.y / 2 * cosA),
		playerPos + sf::Vector2f(playerSize.x / 2 * cosA - playerSize.y / 2 * sinA, playerSize.x / 2 * sinA + playerSize.y / 2 * cosA)
	};

	// Finding the lowest Y of the player's bounding box using the corner offsets
	float lowestY = corners[0].y;
	for (int i = 1; i < 4; ++i) {
		if (corners[i].y > lowestY) {
			lowestY = corners[i].y;
		}
	}

	// Calculating terrain altitude at player's position
	// Calculating altitude above terrain
	// Setting up the isAboveLandingZone boolean
	float terrainYatPlayer = terrain->getTerrainYatX(playerPos.x);
	float altitudeAboveTerrain = lowestY - terrainYatPlayer;
	bool isAboveLandingZone = false;

	// Checking if the player is above a landing zone by looping over them
	for (int p = 0; p < 3; p++)
	{
		// Checking if any of the player's corners are above the landing zone
		for (int i = 0; i < 4; ++i) {
			if (corners[i].x > landingZones[p] &&
				corners[i].x < landingZones[p] + landingZoneWidth)
			{
				// If the player is above a landing zone, set isAboveLandingZone to true
				isAboveLandingZone = true;

				// Checking if the player is below the landing zone (landed)
				if (lowestY > terrainYatPlayer - landingZoneWidth / 8)
				{
					if (playerPos.x - playerSize.x / 2 < landingZones[p] || playerPos.x + playerSize.x / 2 > landingZones[p] + landingZoneWidth)
					{
						std::cout << "CRASHED" << std::endl;
						player->setPosition(sf::Vector2f(window->getSize().x / 6, window->getSize().y / 6));
						player->setAcceleration(sf::Vector2f(0, 0));
						player->setAngularVelocity(0);
						player->setAngularAcceleration(0);
						player->setVelocity(sf::Vector2f(100.0f, 0));
						player->setRotation(270);
						player->setDrag(0.03f);

						fuel = 100.0f;
						score = 0.0f;
						thrust = 0.0f;
						player->setMass(500.0f + fuel);

						return;
					}
					
					if (player->getVelocity().x > 10.0f || player->getVelocity().x < -10.0f || player->getVelocity().y > 10.0f || player->getVelocity().y < -10.0f || player->getRotation() > 10.0f || player->getRotation() > 350.0f)
					{
						std::cout << "CRASHED" << std::endl;
						player->setPosition(sf::Vector2f(window->getSize().x / 6, window->getSize().y / 6));
						player->setAcceleration(sf::Vector2f(0, 0));
						player->setAngularVelocity(0);
						player->setAngularAcceleration(0);
						player->setVelocity(sf::Vector2f(100.0f, 0));
						player->setRotation(270);
						player->setDrag(0.03f);

						fuel = 100.0f;
						score = 0.0f;
						thrust = 0.0f;
						player->setMass(500.0f + fuel);

						return;
					}	

					// If the player is landed, reset player's position and velocity
					player->setVelocity(sf::Vector2f(0, 0));
					player->setPosition(sf::Vector2f(playerPos.x, terrainYatPlayer - playerSize.y / 2 - landingZoneWidth / 8));
					player->setAngularAcceleration(0);
					player->setAngularVelocity(0);
					player->setRotation(0);

					// Get player's distance from the center of the screen both horizontally and vertically
					float distanceFromCenterX = std::abs(playerPos.x - 5 * window->getSize().x / 8);
					float distanceFromCenterY = std::abs(playerPos.y - window->getSize().y / 3);
					float distanceFromCenter = std::sqrt(distanceFromCenterX * distanceFromCenterX + distanceFromCenterY * distanceFromCenterY);

					score = (distanceFromCenter * 0.25f + fuel) * 100.0f;
					
					keyState.isUpPressed = false;
					keyState.isLeftPressed = false;
					keyState.isRightPressed = false;
					gameEndTimer += deltaTime;
					if (gameEndTimer > 10.0f)
					{
						std::cout << "LANDED" << std::endl;
						player->setPosition(sf::Vector2f(window->getSize().x / 6, window->getSize().y / 6));
						player->setAcceleration(sf::Vector2f(0, 0));
						player->setAngularVelocity(0);
						player->setAngularAcceleration(0);
						player->setVelocity(sf::Vector2f(100.0f, 0));
						player->setRotation(270);
						player->setDrag(0.03f);

						fuel = 100.0f;
						score = 0.0f;
						thrust = 0.0f;
						player->setMass(500.0f + fuel);

						gameEndTimer = 0.0f;

						return;
					}
				}
			}
		}
	}

	// Increment the player's height above terrain if the player is above a landing zone
	if(isAboveLandingZone) altitudeAboveTerrain += landingZoneWidth / 8;

	// For each corner of the player's bounding box, check for collision with the terrain
	for (int i = 0; i < 4; ++i)
	{
		if (corners[i].y > terrain->getTerrainYatX(corners[i].x))
		{
			std::cout << "CRASHED" << std::endl;
			// If collided with terrain, reset player to starting state
			player->setPosition(sf::Vector2f(window->getSize().x / 6, window->getSize().y / 6));
			player->setAcceleration(sf::Vector2f(0, 0));
			player->setAngularVelocity(0);
			player->setAngularAcceleration(0);
			player->setVelocity(sf::Vector2f(100.0f, 0));
			player->setRotation(270);
			player->setDrag(0.03f);

			fuel = 100.0f;
			score = 0.0f;
			thrust = 0.0f;
			player->setMass(500.0f + fuel);
		}
	}

	// Applying forces to the player based on the key state
	if(keyState.isUpPressed) thrust = 1.0f;
    if (thrust > 0)
    {
		if (fuel > 0.0f)
		{
			sf::Vector2f upVector = player->getUpVector();
			player->applyImpulseForce(upVector * 4500.0f * thrust);

			// Decreasing the player's fuel
			fuel -= 3.5f * deltaTime * thrust;
			// Setting the player's mass
			player->setMass(500.0f + fuel);
		}
		else fuel = 0.0f;
    }
    if (keyState.isLeftPressed)
    {
        sf::Vector2f impulseForce = sf::Vector2f(0, -500.0f);
        sf::Vector2f offset = sf::Vector2f(player->getBoundingBoxSize().x / 2, player->getBoundingBoxSize().y / 2);
        player->applyImpulseForceAtOffset(impulseForce, offset);
    }
    if (keyState.isRightPressed)
    {
        sf::Vector2f impulseForce = sf::Vector2f(0, -500.0f);
        sf::Vector2f offset = sf::Vector2f(-player->getBoundingBoxSize().x / 2, player->getBoundingBoxSize().y / 2);
        player->applyImpulseForceAtOffset(impulseForce, offset);
    }

	// Updating the player's rigidbody
    player->update(deltaTime);
	// Updating the game UI
	gameUI->update(player->getVelocity(), altitudeAboveTerrain, fuel, score);
}

void Gameplay::draw()
{
	// Initial window clearing
	window->clear();

	// Setting the center of the zoomed view to the player's position
	zoomView.setCenter(player->getPosition());

	// Drawing the main view
	window->setView(mainView);
	terrain->renderBackground();
	terrain->renderTerrain();
	terrain->renderLandingZones();
	player->draw();
	gameUI->draw();
	window->draw(zoomViewOutline);

	// Drawing the zoomed view
	window->setView(zoomView);
	terrain->renderBackground();
	terrain->renderTerrain();
	terrain->renderLandingZones();
	player->draw();
	// Draw outline around zoomed view

	// Dispalying
	window->display();
}

// Handling events
void Gameplay::handleEvent(sf::Event event)
{
	if (event.type == sf::Event::Closed)
	{
		window->close();
	}

	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Up)
		{
			keyState.isUpPressed = true;
		}
		if (event.key.code == sf::Keyboard::Left)
		{
			keyState.isLeftPressed = true;
		}
		if (event.key.code == sf::Keyboard::Right)
		{
			keyState.isRightPressed = true;
		}
	}
	if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::Up)
		{
			keyState.isUpPressed = false;
		}
		if (event.key.code == sf::Keyboard::Left)
		{
			keyState.isLeftPressed = false;
		}
		if (event.key.code == sf::Keyboard::Right)
		{
			keyState.isRightPressed = false;
		}
	}
}