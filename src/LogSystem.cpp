#include "LogSystem.h"
#include "FileSystem.h"

LogSystem* LogSystem::mInstance = nullptr;

LogSystem& LogSystem::getInstance()
{
	if (!mInstance)
	{
		delete mInstance;
		mInstance = new LogSystem();
	}
	return *mInstance;
}

auto LogSystem::getInfo(typeInfo pType, std::string_view pMsg,
						std::string_view pFile, int32_t pLine) const noexcept -> const std::string&
{
	
	return "";
}

auto LogSystem::getWarning(typeInfo pType, std::string_view pMsg, 
						   std::string_view pFile, int32_t pLine) const noexcept -> const std::string&
{

	return "";
}

auto LogSystem::getError(typeInfo pType, std::string_view pMsg, 
						 std::string_view pFile, int32_t pLine) const noexcept -> const std::string&
{

	return "";
}

LogSystem::~LogSystem()
{
	delete mInstance;
}
