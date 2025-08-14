#pragma once
#include "FileSystem.h"
#include "LogSystem.h"

bool file_exist(std::string_view pName, const FileSystem& pFileSystemInstance)
{
	if (!pFileSystemInstance.getStorage().contains(std::string(pName)))
	{
		LOG(LogSystem::typeInfo::ERROR, "File does not exist or you did not open this!");
		return false;
	}
	return true;
}
