#pragma once
#include <iostream>
#include <format>
#include <filesystem>
#include <memory>

#include "FileSystem.h"

class LogSystem
{
public:
	enum class typeInfo;
public:
	auto getInfo(typeInfo pType, std::string_view pMsg,
				 std::string_view pFile, int32_t pLine) const noexcept -> const std::string&;
	auto getWarning(typeInfo pType, std::string_view pMsg,
					std::string_view pFile, int32_t pLine) const noexcept -> const std::string&;
	auto getError(typeInfo pType, std::string_view pMsg,
				  std::string_view pFile, int32_t pLine) const noexcept -> const std::string&;

protected:
	enum class typeInfo
	{
		INFO = 0,
		WARNING = 1,
		ALMOST_ERROR_WARNING = 2,
		ERROR = 3,
		HUGE_ERROR = 4,
		SERIOUS_ERROR = 5,
		VERY_SERIOUS_ERROR = 6,
		ERROR_TO_DEATH = 7
	};

private:
	FileSystem mFileSystem;
	LogSystem* mInstance;

} gLogSystem;

#define LOG(pType, pMsg) \
    do \
	{ \
        switch (pType) \
		{ \
            case gLogSystem.typeInfo::INFO: \
                gLogSystem.getInfo(pType, pMsg, __FILE__, __LINE__); \
                break; \
            case gLogSystem.typeInfo::WARNING: \
                gLogSystem.getWarning(pType, pMsg, __FILE__, __LINE__); \
                break; \
            default: \
                gLogSystem.getError(pType, pMsg, __FILE__, __LINE__); \
                break; \
		} \
    } while (0) 