#pragma once
#include <fstream>
#include <string_view>
#include <concepts>
#include <vector>

template <typename V>
concept is_Vector = std::is_same_v<V, std::vector<typename std::remove_cvref_t<V>::value_type>>;

class MyVariableLoader
{
	std::ifstream inFile;

public:
	explicit MyVariableLoader(std::string_view fileName) : inFile(fileName.data(), std::ios::binary) {};

	void Load(auto& classInstanceToLoad)
	{
		inFile.read(std::bit_cast<char*>(&classInstanceToLoad), sizeof(classInstanceToLoad));
	}

	void Load(is_Vector auto& classVectorToLoad)
	{
		size_t count = 0;
		Load(count);
		classVectorToLoad.resize(count);
		for (auto& instance : classVectorToLoad)
		{
			Load(instance);
		}
	}
};

class MyVariableSaver
{
	std::ofstream outFile;

public:
	explicit MyVariableSaver(std::string_view fileName) : outFile(fileName.data(), std::ios::binary) {};

	void Save(const auto& classInstanceToSave)
	{
		outFile.write(std::bit_cast<char*>(&classInstanceToSave), sizeof(classInstanceToSave));
	}

	void Save(const is_Vector auto& classVectorToSave)
	{
		size_t count = classVectorToSave.size();
		Save(count);
		for (const auto& instance : classVectorToSave)
		{
			Save(instance);
		}
	}
};