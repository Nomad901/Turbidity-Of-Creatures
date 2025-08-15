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
						std::string_view pFile, int32_t pLine) -> std::string
{
	std::string consoleText = std::format("[TYPE] {} [MESSAGE] {} [FILE] {} [LINE] {} [TIME] {}\n", 
										   "Info", pMsg, pFile, pLine, std::chrono::system_clock::now());
	std::cout << consoleText;
	mFileSystem.writeFile("Log", consoleText);
	return consoleText;
}

auto LogSystem::getWarning(typeInfo pType, std::string_view pMsg, 
						   std::string_view pFile, int32_t pLine) -> std::string
{
	std::string consoleText = std::format("[TYPE] {} [MESSAGE] {} [FILE] {} [LINE] {} [TIME] {}\n",
										   "Warning", pMsg, pFile, pLine, std::chrono::system_clock::now());
	std::cout << consoleText;
	mFileSystem.writeFile("Log", consoleText);
	return consoleText;
}

auto LogSystem::getError(typeInfo pType, std::string_view pMsg, 
						 std::string_view pFile, int32_t pLine) -> std::string
{
	std::string consoleText = "";
	switch (pType)
	{
	case LogSystem::typeInfo::ERROR:
		consoleText = std::format("[TYPE] {} [MESSAGE] {} [FILE] {} [LINE] {} [TIME] {}\n",
								  "Error", pMsg, pFile, pLine, std::chrono::system_clock::now());
		break;
	case LogSystem::typeInfo::SERIOUS_ERROR:
		consoleText = std::format("[TYPE] {} [MESSAGE] {} [FILE] {} [LINE] {} [TIME] {}\n",
								  "Serious Error", pMsg, pFile, pLine, std::chrono::system_clock::now());
		break;
	case LogSystem::typeInfo::ERROR_TO_DEATH:
		consoleText = std::format("[TYPE] {} [MESSAGE] {} [FILE] {} [LINE] {} [TIME] {}\n",
								  "Error to death", pMsg, pFile, pLine, std::chrono::system_clock::now());
		break;
	default:
		break;
	}
	std::cout << consoleText;
	mFileSystem.writeFile("Log", consoleText);
	if(pType == typeInfo::ERROR_TO_DEATH)
		std::abort();
	return consoleText;
}

auto LogSystem::clearLogFile() -> void
{
	mFileSystem.clearFile("Log");
}

auto LogSystem::getLogFile() -> std::string
{
	std::string logFile = mFileSystem.readFileToString("Log");
	std::cout << std::format("Log: {}\n", logFile);
	return logFile;
}

LogSystem::LogSystem()
{
	std::string resource_path = RESOURCES_PATH;
	mFileSystem.openFile("Log", resource_path + "log.txt", std::ios::app);
}

LogSystem::~LogSystem()
{
	delete mInstance;
}
