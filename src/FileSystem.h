#pragma once
#include <iostream>
#include <format>
#include <string>
#include <filesystem>
#include <memory>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <type_traits>

#include "JSON/json.hpp"

class FileSystem
{
private:
	class fileHandle;
public:
	FileSystem() = default;
	// std::ios::in - reading
	// std::ios::out - writing
	FileSystem(std::string_view pName, const std::filesystem::path& pCurrentPath,
			   std::ios_base::openmode pOpenModes, bool pJSONfile = false);
	
	void openFile(std::string_view pName, const std::filesystem::path& pPath, std::ios_base::openmode pOpenModes,
				  bool pJSONfile = false);
	void closeFile(std::string_view pName) noexcept;
	
	template<typename Key, typename Value>
	void writeFile(std::string_view pName, const std::initializer_list<std::pair<Key, Value>>& pData);
	template<typename T>
	void writeFile(std::string_view pName, T&& pText);

	void readFile(std::string_view pName) noexcept;
	std::string readFileToString(std::string_view pName) noexcept;
	template<typename TypeColumn1, typename TypeColumn2>
	std::string readJSON(std::string_view pName, const TypeColumn1& pNameColumn1, const TypeColumn2& pNameColumn2);

	const std::filesystem::path& getPath(std::string_view pName) noexcept;
	bool fileIsOpened(std::string_view pName) noexcept;
	std::string getLastSavedText(std::string_view pName) noexcept;

	auto getStorage() const noexcept -> const std::unordered_map<std::string, std::unique_ptr<fileHandle>>&;
	const std::string& getResourcePath() const noexcept;
	void clearFile(std::string_view pName);

private:
	class fileHandle
	{
	public:
		fileHandle(const std::filesystem::path& pPath, std::ios_base::openmode pOpenModes, bool pIsJSONstream = false);
		~fileHandle();

		template<typename Key, typename Value>
		void writeFile(const std::initializer_list<std::pair<Key, Value>>& pStorage);
		template<typename T>
			requires requires(T&& pData)
			{
				std::to_string(pData);
			}
		void writeFile(T&& pData);
		void writeFile(std::string_view pData);

		template<typename column1T, typename column2T>
		void readFile(const column1T& pName1, const column2T& pName2);
		template<typename column1T, typename column2T>
		std::string readFileString(const column1T& pName1, const column2T& pName2);
		std::string readFileString();
		void readFile();

		bool fileIsOpened() const noexcept;

		std::fstream& getStream() noexcept;
		nlohmann::json& getJSONstream() noexcept;
		const std::filesystem::path& getPath() noexcept;
		std::string getBufferText() const noexcept;

	private:
		std::fstream mFstream;
		std::filesystem::path mPath;

		nlohmann::json mJSONstream;
		bool mIsJSONfile{ false };

		std::string mBufferText{};
		std::ios_base::openmode mModes;
	};

private:
	std::unordered_map<std::string, std::unique_ptr<fileHandle>> mBufferFiles;
	std::string mResourcePath{ RESOURCES_PATH };
};

template<typename Key, typename Value>
inline void FileSystem::writeFile(std::string_view pName, const std::initializer_list<std::pair<Key, Value>>& pData)
{
	if (!mBufferFiles.contains(std::string(pName)))
	{
		std::cout << "File is not contained!\n";
		return;
	}
	mBufferFiles[std::string(pName)]->writeFile(pData);
}

template<typename T>
inline void FileSystem::writeFile(std::string_view pName, T&& pText)
{
	if (!mBufferFiles.contains(std::string(pName)))
	{
		std::cout << "File is not contained!\n";
		return;
	}
	mBufferFiles[std::string(pName)]->writeFile(pText);
}

template<typename TypeColumn1, typename TypeColumn2>
inline std::string FileSystem::readJSON(std::string_view pName, const TypeColumn1& pNameColumn1, const TypeColumn2& pNameColumn2)
{
	if (!mBufferFiles.contains(std::string(pName)))
	{
		std::cout << "File is not contained!\n";
		return;
	}
	return mBufferFiles[std::string(pName)]->readFileString(pNameColumn1, pNameColumn2);
}

template<typename Key, typename Value>
inline void FileSystem::fileHandle::writeFile(const std::initializer_list<std::pair<Key, Value>>& pStorage)
{
	std::ofstream ofstr(mPath, std::ios::app);
	for (auto& [key, value] : pStorage)
	{
		mJSONstream[key] = value;
		ofstr << mJSONstream[key];
	}
}

template<typename T>
	requires requires(T&& pData)
	{
		std::to_string(pData);
	}
inline void FileSystem::fileHandle::writeFile(T&& pData)
{
	mFstream << std::to_string(pData);
}

template<typename column1T, typename column2T>
void FileSystem::fileHandle::readFile(const column1T& pName1, const column2T& pName2)
{
	std::cout << mJSONstream[pName1][pName2];
	mBufferText = mJSONstream[pName1][pName2].get<std::string>();
}

template<typename column1T, typename column2T>
inline std::string FileSystem::fileHandle::readFileString(const column1T& pName1, const column2T& pName2)
{
	mBufferText = mJSONstream[pName1][pName2].get<std::string>();
	return mBufferText;
}
