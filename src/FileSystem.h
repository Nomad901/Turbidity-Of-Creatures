#pragma once
#include <string>
#include <filesystem>
#include <memory>
#include <vector>
#include <fstream>
#include <unordered_map>

#include "JSON/json.hpp"

class FileSystem
{
public:
	FileSystem() = default;
	FileSystem(std::string_view pName, const std::filesystem::path& pCurrentPath, bool readOnly = false);
	
	void openFile(std::string_view pName, const std::filesystem::path& pPath, bool pReadOnly = false);
	void closeFile(std::string_view pName) noexcept;
	
	void writeFile(std::string_view pName, std::string_view pMessage);
	void writeFileJSON();
	
	void readFile(std::string_view pName) const noexcept;
	nlohmann::json readJSON(std::string_view pName) const noexcept;
	std::string readFileToString(std::string_view pName) const noexcept;

	const std::filesystem::path& getPath(std::string_view pName) const noexcept;
	bool fileIsOpened(std::string_view pName) const noexcept;

	class fileHandle
	{
	public:
		fileHandle(const std::filesystem::path& pPath, bool mReadOnly = false)
		{
			
		}
		~fileHandle()
		{
			mFstream.close();
		}

		inline std::fstream& getStream() noexcept
		{
			return mFstream;
		}
		inline const std::filesystem::path& getPath() noexcept
		{
			return mPath;
		}

	private:
		std::fstream mFstream;
		std::filesystem::path mPath;
	};

private:
	std::string mBufferText;
	std::unordered_map<std::string, std::unique_ptr<fileHandle>> mBufferFiles;
};

