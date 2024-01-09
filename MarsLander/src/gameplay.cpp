#include "gameplay.h"

Gameplay::Gameplay(sf::RenderWindow* window) : window(window)
{
    buttonState = { false, false, false };

	mainView = sf::View(sf::FloatRect(0, 0, window->getSize().x, window->getSize().y));
	zoomView = sf::View(sf::FloatRect(0, 0, window->getSize().x, window->getSize().y));
	zoomView.zoom(0.15f);

	mainView.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
	zoomView.setViewport(sf::FloatRect(0.6f, 0.0f, 0.4f, 0.4f));

	terrain = new Terrain(window);
	terrain->setAmplitudeModifier(0.3f);
	terrain->setFrequencyModifier(2137.0f);
	terrain->build();
	
	landingZones = new int[3];
	terrain->getLandingZones(landingZones);
	
	sf::RectangleShape* playerShape = new sf::RectangleShape(sf::Vector2f(20.0f, 20.0f));
	playerShape->setOrigin(playerShape->getSize().x / 2, playerShape->getSize().y / 2);
	playerShape->setPosition(window->getSize().x / 6, window->getSize().y / 6);
	playerShape->setFillColor(sf::Color::Red);

	player = new Rigidbody(playerShape, window);
	player->setMass(500);

	player->applyForce(sf::Vector2f(0, 3000.0f));
	player->setVelocity(sf::Vector2f(100.0f, 0));
	player->setRotation(270);
	player->setDrag(0.03f);
	player->setAngularDrag(0.1f);
}

void Gameplay::update(float deltaTime)
{
    sf::Vector2f playerPos = player->getPosition();
    sf::Vector2f playerSize = player->getBoundingBoxSize();

    int landingZoneWidth = terrain->getLandingSize().x;

	float playerRotRad = player->getRotation() * (3.14159f / 180.0f);

	float cosA = std::cos(playerRotRad);
	float sinA = std::sin(playerRotRad);

	sf::Vector2f corners[4] = {
		playerPos + sf::Vector2f(-playerSize.x / 2 * cosA + playerSize.y / 2 * sinA, -playerSize.x / 2 * sinA - playerSize.y / 2 * cosA),
		playerPos + sf::Vector2f(playerSize.x / 2 * cosA + playerSize.y / 2 * sinA, playerSize.x / 2 * sinA - playerSize.y / 2 * cosA),
		playerPos + sf::Vector2f(-playerSize.x / 2 * cosA - playerSize.y / 2 * sinA, -playerSize.x / 2 * sinA + playerSize.y / 2 * cosA),
		playerPos + sf::Vector2f(playerSize.x / 2 * cosA - playerSize.y / 2 * sinA, playerSize.x / 2 * sinA + playerSize.y / 2 * cosA)
	};

	float lowestY = corners[0].y;
	for (int i = 1; i < 4; ++i) {
		if (corners[i].y > lowestY) {
			lowestY = corners[i].y;
		}
	}

	float terrainYatPlayer = terrain->getTerrainYatX(playerPos.x);

	for (int p = 0; p < 3; p++)
	{
		for (int i = 0; i < 4; ++i) {
			if (corners[i].x > landingZones[p] &&
				corners[i].x < landingZones[p] + landingZoneWidth &&
				lowestY > terrainYatPlayer - landingZoneWidth / 8)
			{
				player->setVelocity(sf::Vector2f(0, 0));
				player->setPosition(sf::Vector2f(playerPos.x, terrainYatPlayer - playerSize.y / 2 - landingZoneWidth / 8));

				player->setAngularAcceleration(0);
				player->setAngularVelocity(0);
				player->setRotation(0);
			}
		}
	}

	// If player collides with terrain, reset position and velocity (check for the corners of the player)
	for (int i = 0; i < 4; ++i)
	{
		if (corners[i].y > terrain->getTerrainYatX(corners[i].x))
		{
			player->setPosition(sf::Vector2f(window->getSize().x / 6, window->getSize().y / 6));
			player->setAcceleration(sf::Vector2f(0, 0));
			player->setAngularVelocity(0);
			player->setAngularAcceleration(0);
			
			player->setVelocity(sf::Vector2f(100.0f, 0));
			player->setRotation(270);
			player->setDrag(0.03f);
		}
	}


    if (buttonState.isUpPressed)
    {
        sf::Vector2f upVector = player->getUpVector();
        player->applyImpulseForce(upVector * 4000.0f);
    }

    if (buttonState.isLeftPressed)
    {
        sf::Vector2f impulseForce = sf::Vector2f(0, -500.0f);
        sf::Vector2f offset = sf::Vector2f(player->getBoundingBoxSize().x / 2, player->getBoundingBoxSize().y / 2);
        player->applyImpulseForceAtOffset(impulseForce, offset);
    }
    if (buttonState.isRightPressed)
    {
        sf::Vector2f impulseForce = sf::Vector2f(0, -500.0f);
        sf::Vector2f offset = sf::Vector2f(-player->getBoundingBoxSize().x / 2, player->getBoundingBoxSize().y / 2);
        player->applyImpulseForceAtOffset(impulseForce, offset);
    }

    player->update(deltaTime);
}

void Gameplay::draw()
{
	window->clear();

	// Setting the center of the zoomed view to the player's position
	zoomView.setCenter(player->getPosition());

	terrain->renderBackground();
	terrain->renderTerrain();
	terrain->renderLandingZones();
	player->draw();

	// Drawing the main view
	window->setView(mainView);
	terrain->renderBackground();
	terrain->renderTerrain();
	terrain->renderLandingZones();
	player->draw();

	// Drawing the zoomed view
	window->setView(zoomView);
	terrain->renderBackground();
	terrain->renderTerrain();
	terrain->renderLandingZones();
	player->draw();

	window->display();
}

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
			buttonState.isUpPressed = true;
		}
		if (event.key.code == sf::Keyboard::Left)
		{
			buttonState.isLeftPressed = true;
		}
		if (event.key.code == sf::Keyboard::Right)
		{
			buttonState.isRightPressed = true;
		}
	}
	if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::Up)
		{
			buttonState.isUpPressed = false;
		}
		if (event.key.code == sf::Keyboard::Left)
		{
			buttonState.isLeftPressed = false;
		}
		if (event.key.code == sf::Keyboard::Right)
		{
			buttonState.isRightPressed = false;
		}
	}
}