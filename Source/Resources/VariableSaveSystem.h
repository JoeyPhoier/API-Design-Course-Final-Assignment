#pragma once
#include <fstream>
#include <string_view>
#include <concepts>
#include <vector>
#include <type_traits>
#include <stdexcept>

class MyVariableSaver;
class MyVariableLoader;

template <typename T>
concept is_Trivially_Saveable = std::is_trivially_copyable_v<T>;

template <typename T>
concept is_Custom_Saveable = requires (T t, MyVariableSaver& saver) { { t.Serialize(saver) } -> std::same_as<void>; } &&
							 requires (T t, MyVariableLoader& loader) { { t.Unserialize(loader) } -> std::same_as<void>; };

template <typename V>
concept is_Vector = std::is_same_v<V, std::vector<typename std::remove_cvref_t<V>::value_type>>;

template <typename S>
concept is_String = std::same_as<S, std::string>;

class MyVariableSaver
{
	std::ofstream outFile;

public:
	explicit MyVariableSaver(std::string_view fileName) : outFile(fileName.data(), std::ios::binary) {};

	void Save(const is_Trivially_Saveable auto& classInstanceToSave)
	{
		outFile.write(std::bit_cast<char*>(&classInstanceToSave), sizeof(classInstanceToSave));
	}

	void Save(const is_Custom_Saveable auto& classInstanceToSave)
	{
		classInstanceToSave.Serialize(*this);
	}

	void Save(const is_Vector auto& classVectorToSave)
	{
		const size_t count = classVectorToSave.size();
		Save(count);
		if (count == 0)
		{
			return;
		}
		for (const auto& instance : classVectorToSave)
		{
			Save(instance);
		}
	}

	void Save(const is_String auto& stringToSave)
	{
		const size_t count = stringToSave.size();
		Save(count);
		if (count == 0)
		{
			return;
		}
		outFile.write(std::bit_cast<char*>(&stringToSave.front()), count * sizeof(char));
	}
};

class MyVariableLoader
{
	std::ifstream inFile;

public:
	explicit MyVariableLoader(std::string_view fileName) : inFile(fileName.data(), std::ios::binary) {};

	void Load(is_Trivially_Saveable auto& classInstanceToLoad)
	{
		inFile.read(std::bit_cast<char*>(&classInstanceToLoad), sizeof(classInstanceToLoad));
	}

	void Load(is_Custom_Saveable auto& classInstanceToLoad)
	{
		classInstanceToLoad.Unserialize(*this);
	}

	void Load(is_Vector auto& classVectorToLoad)
	{
		size_t count = 0;
		Load(count);
		classVectorToLoad.resize(count);
		if (count == 0)
		{
			return;
		}
		for (auto& instance : classVectorToLoad)
		{
			Load(instance);
		}
	}

	void Load(is_String auto& stringToLoad)
	{
		size_t count = 0;
		Load(count);
		stringToLoad.resize(count);
		if (count == 0)
		{
			return;
		}
		inFile.read(std::bit_cast<char*>(&stringToLoad.front()), count * sizeof(char));
	}
};
