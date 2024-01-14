#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

class Scene
{
public:
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void handleEvent(sf::Event event) = 0;
    virtual int getID() const = 0;
    virtual void reset() = 0;
};

class SceneManager
{
private:
    std::vector<Scene*> scenes;
    int currentSceneID;
    int score;

public:
    SceneManager();
    ~SceneManager();

    void addScene(Scene* scene);
    void removeScene(int id);
    void render();
    void update(float deltaTime);
    void changeScene(int newSceneID);
    void handleEvent(sf::Event event);
    void reset();
    int getScore() { return score; }
    void setScore(int newScore) { score = newScore; }
};