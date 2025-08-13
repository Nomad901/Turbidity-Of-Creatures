#include "LogSystem.h"
#include "FileSystem.h"

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
