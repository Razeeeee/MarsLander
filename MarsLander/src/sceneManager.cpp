#pragma once
#include "sceneManager.h"

SceneManager::SceneManager()
{
    currentSceneID = -1;
    scenes = std::vector<Scene*>();
	score = 0;
};

SceneManager::~SceneManager()
{
    for (Scene* scene : scenes)
    {
		delete scene;
	}
	scenes.clear();
};

void SceneManager::addScene(Scene* scene)
{
	scenes.push_back(scene);
};

void SceneManager::removeScene(int id)
{
    if (id >= 0 && id < scenes.size())
    {
		delete scenes[id];
		scenes.erase(scenes.begin() + id);
	}
};

void SceneManager::render()
{
    for (Scene* scene : scenes)
    {
        if (scene && currentSceneID == scene->getID())
        {
			scene->render();
			break;
		}
	}
};

void SceneManager::update(float deltaTime)
{
    for (Scene* scene : scenes)
    {
        if (scene && currentSceneID == scene->getID())
        {
			scene->update(deltaTime);
			break;
		}
	}
};

void SceneManager::changeScene(int newSceneID)
{
    for (Scene* scene : scenes)
    {
        if (scene && newSceneID == scene->getID())
        {
            if (currentSceneID != -1)
            {
				// tutaj to co sie dzieje przed zmiana scene
			}

			currentSceneID = newSceneID;
			reset();
			std::cout << "Changed scene to " << newSceneID << "\n";
			break;
		}
	}
};

void SceneManager::handleEvent(sf::Event event)
{
    for (Scene* scene : scenes)
    {
        if (scene && currentSceneID == scene->getID())
        {
			scene->handleEvent(event);
			break;
		}
	}
};

void SceneManager::reset()
{
	for (Scene* scene : scenes)
	{
		if (scene && currentSceneID == scene->getID())
		{
			scene->reset();
			break;
		}
	}
};