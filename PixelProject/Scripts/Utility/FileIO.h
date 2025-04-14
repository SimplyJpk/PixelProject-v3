#pragma once
#include <SDL3/SDL_filesystem.h>

#include <filesystem>
#include <fstream>

namespace PixelProject::IO
{
	static std::string GetBasePath()
	{
		return SDL_GetBasePath();
	}

	static std::filesystem::path GetExecutablePath()
	{
		return SDL_GetBasePath();
	}

	static std::filesystem::path GetFilePath(const std::string& filename)
	{
		return GetExecutablePath() / filename;
	}

	static bool DoesFullPathExist(const std::string& filename)
	{
		return std::filesystem::exists(filename);
	}

	static bool DoesFileExist(const std::string& filename)
	{
		return DoesFullPathExist(GetFilePath(filename).string());
	}

	static bool DoesDirectoryExist(const std::string& directory, const bool include_base = true)
	{
		return std::filesystem::exists((include_base ? SDL_GetBasePath() : "") + directory);
	}

	static void CreateDirIfNotExists(const std::string& directory)
	{
		if (!DoesDirectoryExist(directory)) {
			std::filesystem::create_directory(directory);
		}
	}

	static bool CreateDirectory(const std::string& directory)
	{
		return std::filesystem::create_directory(directory);
	}

	static bool CreateFileIfNotExists(const std::string& filename)
	{
		const auto path = std::filesystem::path(filename);
		std::string searchPath = path.string();
		if (!path.has_root_directory())
			searchPath = GetFilePath(searchPath).string();

		if (exists(path))
			return true;
		if (!exists(path.parent_path()))
			CreateDirectory(path.parent_path().string());

		std::ofstream file(path);
		file.close();
		return true;
	}

	static std::string GetFileContents(const std::string& filePath, bool isFullPath = false)
	{
		const auto usedPath = isFullPath ? filePath : GetFilePath(filePath).string();
		if (!DoesFullPathExist(usedPath))
			return "";

		std::ifstream ifs(usedPath);
		std::string content(
			(std::istreambuf_iterator<char>(ifs)),
			(std::istreambuf_iterator<char>()));
		if (content.length() <= 0)
			return "";

		return content;
	}
}
