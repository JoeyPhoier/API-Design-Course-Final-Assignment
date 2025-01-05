#pragma once
#include <fstream>
#include <string_view>
#include <concepts>
#include <vector>
#include <type_traits>
#include <stdexcept>
#include <span>

class MyVariableSaver;
class MyVariableLoader;

template <typename T>
concept is_Trivially_Saveable = std::is_trivially_copyable_v<T>;

template <typename T>
concept is_Custom_Saveable = requires (T t, MyVariableSaver& saver) { { t.Serialize(saver) } -> std::same_as<void>; } &&
							 requires (T t, MyVariableLoader& loader) { { t.Unserialize(loader) } -> std::same_as<void>; };

template <typename C>
concept is_Container = requires(C c) { { c.begin() } -> std::input_or_output_iterator;
									   { c.end() } -> std::input_or_output_iterator;
									   { c.size() } -> std::convertible_to<std::size_t>; };

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

	void Save(const is_Container auto& classContainerToSave)
	{
		const size_t count = classContainerToSave.size();
		Save(count);
		if (count == 0)
		{
			return;
		}
		for (const auto& instance : classContainerToSave)
		{
			Save(instance);
		}
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

	void Load(is_Container auto& classVectorToLoad)
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
};
