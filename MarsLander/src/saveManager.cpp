#include "saveManager.h"

SaveManager::SaveManager(const std::string& saveFile) : saveFile(saveFile) {}

int SaveManager::getMarsHS()
{
    int temp = 0;
    int returnHS = 0;
    std::ifstream saveInput("saves/" + saveFile);
    if (saveInput.is_open())
    {
        saveInput >> temp >> returnHS;
        saveInput.close();
    }
    else
    {
        std::cout << "Couldn't find save file!" << std::endl;
        exit(0);
    }

    return returnHS;
}

int SaveManager::getMoonHS()
{
    int returnHS = 0;
    std::ifstream saveInput("saves/" + saveFile);
    if (saveInput.is_open())
    {
        saveInput >> returnHS;
        saveInput.close();
    }
    else
    {
        std::cout << "Couldn't find save file!" << std::endl;
        exit(0);
    }

    return returnHS;
}

void SaveManager::saveMarsHS(int score)
{
    int moonHS = getMoonHS();
    std::ofstream saveOutput("saves/" + saveFile);
    if (saveOutput.is_open())
    {
        saveOutput << moonHS << " " << score << std::endl;
        saveOutput.close();
        std::cout << "Saved Mars HS: " << score << std::endl;
    }
    else
    {
        std::cout << "Couldn't find save file!" << std::endl;
        exit(0);
    }
}

void SaveManager::saveMoonHS(int score)
{
    int marsHS = getMarsHS();
    std::ofstream saveOutput("saves/" + saveFile);
    if (saveOutput.is_open())
    {
        saveOutput << score << " " << marsHS << std::endl;
        saveOutput.close();
        std::cout << "Saved Moon HS: " << score << std::endl;
    }
    else
    {
        std::cout << "Couldn't find save file!" << std::endl;
        exit(0);
    }
}
