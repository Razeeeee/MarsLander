#pragma once
#include <iostream>
#include <fstream>

class SaveManager
{
public:
	SaveManager(const std::string& saveFile);

	void saveMoonHS(int score);
	void saveMarsHS(int score);
	int getMoonHS();
	int getMarsHS();

private:
	std::string saveFile;
};

