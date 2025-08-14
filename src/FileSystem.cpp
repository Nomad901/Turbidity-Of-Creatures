#include "FileSystem.h"
#include "SystemMacros.h"

FileSystem::FileSystem(std::string_view pName, const std::filesystem::path& pCurrentPath,
					   std::ios_base::openmode pOpenModes, bool pJSONfile)
{
	openFile(pName, pCurrentPath, pOpenModes, pJSONfile);
}

void FileSystem::openFile(std::string_view pName, const std::filesystem::path& pPath, 
						  std::ios_base::openmode pOpenModes, bool pJSONfile)
{
	mBufferFiles.insert_or_assign(std::string(pName), std::make_unique<fileHandle>(pPath, pOpenModes, pJSONfile));
}

void FileSystem::closeFile(std::string_view pName) noexcept
{
	if (!file_exist(pName, *this))
		return;
	mBufferFiles[std::string(pName)].release();
}

void FileSystem::readFile(std::string_view pName) noexcept
{
	if (!file_exist(pName, *this))
		return;
	mBufferFiles[std::string(pName)]->readFile();
}

std::string FileSystem::readFileToString(std::string_view pName) noexcept
{
	if (!file_exist(pName, *this))
		return "";
	return mBufferFiles[std::string(pName)]->readFileString();
}

const std::filesystem::path& FileSystem::getPath(std::string_view pName) noexcept
{
	if (!file_exist(pName, *this))
		return "";
	return mBufferFiles[std::string(pName)]->getPath();
}

bool FileSystem::fileIsOpened(std::string_view pName) noexcept
{
	if (!file_exist(pName, *this))
		return false;
	return mBufferFiles[std::string(pName)]->fileIsOpened();
}

std::string FileSystem::getLastSavedText(std::string_view pName) noexcept
{
	if (!file_exist(pName, *this))
		return "";
	return mBufferFiles[std::string(pName)]->getBufferText();
}

auto FileSystem::getStorage() const noexcept -> const std::unordered_map<std::string, std::unique_ptr<fileHandle>>&
{
	return mBufferFiles;
}

const std::string& FileSystem::getResourcePath() const noexcept
{
	return mResourcePath;
}

void FileSystem::clearFile(std::string_view pName)
{
	std::ofstream ofs;
	ofs.open(mBufferFiles[std::string(pName)]->getPath(), std::ofstream::out | std::ofstream::trunc);
	ofs.close();
	mBufferFiles[std::string(pName)]->getJSONstream().clear();
}

//
// File handler
//
FileSystem::fileHandle::fileHandle(const std::filesystem::path& pPath, std::ios_base::openmode pOpenModes, bool pIsJSONstream)
{
	if (std::filesystem::exists(pPath))
		mFstream.open(pPath, pOpenModes);
	else
	{
		mFstream.open(pPath, std::ios::app);
		mFstream.close();
		mFstream.open(pPath, pOpenModes);
	}
	if (!mFstream.is_open())
		std::cout << std::format("[PATH] {} [FILE] {} [LINE] {}\n", pPath.string() + " is not opened!", 
																    __FILE__, __LINE__);
	mModes = pOpenModes;
	mPath = pPath;
	if (pIsJSONstream && (pOpenModes & std::ios::in))
	{
		try
		{
			mFstream.seekg(0);
			if (mFstream.peek() != std::ifstream::traits_type::eof())
				mJSONstream = nlohmann::json::parse(mFstream);
			else
				mJSONstream = nlohmann::json::object();
			mIsJSONfile = pIsJSONstream;
		}
		catch (...)
		{
			std::cerr << "Json file was not parsed!\n";
			mIsJSONfile = false;
		}
	}
}

FileSystem::fileHandle::~fileHandle()
{
	mFstream.close();
}

void FileSystem::fileHandle::writeFile(std::string_view pData)
{
	mFstream << pData;
	mFstream.flush();
}

std::string FileSystem::fileHandle::readFileString()
{
	std::string line;
	while (std::getline(mFstream, line))
	{
		mBufferText += line + '\n';
	}
	return mBufferText;
}

void FileSystem::fileHandle::readFile()
{
	std::string line;
	while (std::getline(mFstream, line))
	{
		std::cout << line << '\n';
		mBufferText += line + '\n';
	}
}

bool FileSystem::fileHandle::fileIsOpened() const noexcept
{
	return mFstream.is_open();
}

std::fstream& FileSystem::fileHandle::getStream() noexcept
{
	return mFstream;
}

nlohmann::json& FileSystem::fileHandle::getJSONstream() noexcept
{
	return mJSONstream;
}

const std::filesystem::path& FileSystem::fileHandle::getPath() noexcept
{
	return mPath;
}

std::string FileSystem::fileHandle::getBufferText() const noexcept
{
	return mBufferText;
}
