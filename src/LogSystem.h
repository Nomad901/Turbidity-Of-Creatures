#pragma once
#include <iostream>
#include <format>
#include <filesystem>
#include <memory>

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
				 std::string_view pFile, int32_t pLine) const noexcept -> const std::string&;
	auto getWarning(typeInfo pType, std::string_view pMsg,
					std::string_view pFile, int32_t pLine) const noexcept -> const std::string&;
	auto getError(typeInfo pType, std::string_view pMsg,
				  std::string_view pFile, int32_t pLine) const noexcept -> const std::string&;

protected:
	LogSystem() = default;
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
    do \
	{ \
        switch (pType) \
		{ \
            case LogSystem::typeInfo::INFO: \
                LogSystem::getInstance().getInfo(pType, pMsg, __FILE__, __LINE__); \
                break; \
            case LogSystem::typeInfo::WARNING: \
                LogSystem::getInstance().getWarning(pType, pMsg, __FILE__, __LINE__); \
                break; \
            default: \
                LogSystem::getInstance().getError(pType, pMsg, __FILE__, __LINE__); \
                break; \
		} \
    } while (0) 
