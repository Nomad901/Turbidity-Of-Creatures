#pragma once
#include <iostream>
#include <format>
#include <filesystem>
#include <memory>
#include <chrono>

#include "FileSystem.h"

class LogSystem
{
public:	
	enum class typeInfo
	{
		INFO = 0,
		WARNING = 1,
		ERROR = 2,
		SERIOUS_ERROR = 3,
		ERROR_TO_DEATH = 4
	};
public:
	static LogSystem& getInstance();
 
	auto getInfo(typeInfo pType, std::string_view pMsg,
				 std::string_view pFile, int32_t pLine) -> std::string;
	auto getWarning(typeInfo pType, std::string_view pMsg,
					std::string_view pFile, int32_t pLine) -> std::string;
	auto getError(typeInfo pType, std::string_view pMsg,
				  std::string_view pFile, int32_t pLine) -> std::string;

	auto clearLogFile() -> void;
	// getting data from the log-file;
	auto getLogFile() -> std::string;

protected:
	LogSystem();
	~LogSystem();

private:
	LogSystem(const LogSystem&) = delete;
	LogSystem operator=(const LogSystem&) = delete;
	LogSystem(LogSystem&&) = delete;
	LogSystem operator=(LogSystem&&) = delete;

private:
	FileSystem mFileSystem;
	static LogSystem* mInstance;

};

// LogSystem::typeInfo for the pType
#define LOG(pType, pMsg) \
	[&]() -> std::string { \
        switch (pType) \
		{ \
            case LogSystem::typeInfo::INFO: \
                return LogSystem::getInstance().getInfo(pType, pMsg, __FILE__, __LINE__); \
            case LogSystem::typeInfo::WARNING: \
                return LogSystem::getInstance().getWarning(pType, pMsg, __FILE__, __LINE__); \
            default: \
                return LogSystem::getInstance().getError(pType, pMsg, __FILE__, __LINE__); \
		} \
	}()
