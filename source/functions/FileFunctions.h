#pragma once

#include <string>

#ifdef _WIN32
#include <Windows.h>
#endif

class FileFunctions {
public:
#ifdef _WIN32
	// TODO Fix for Linux

	// Folder stuff
	static bool DoesFileExist(const std::string& fileName);
	static std::string CurrentDirectory();

	static bool CheckDirectoryExists(LPCSTR folderPath);
#else 
	static bool CheckDirectoryExists(std::string folderPath);
#endif
};