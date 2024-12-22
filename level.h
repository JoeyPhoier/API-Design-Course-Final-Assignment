#pragma once 
#include <iostream>
#include <string>
#include <fstream>

//TODO: Is this really necessary?
struct Entity
{
	float x, y;

	void LoadLevelFromAFile(const std::string& filename);
};