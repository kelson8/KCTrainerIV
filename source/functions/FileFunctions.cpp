#include "FileFunctions.h"

#include <iostream>
#include <fstream>

#ifdef _WIN32
#include <Windows.h>
#endif

//------------- File functions ---------------/

#ifdef _WIN32
bool FileFunctions::DoesFileExist(const std::string& fileName)
{
	std::ifstream file(fileName);
	return file.good();
}

std::string FileFunctions::CurrentDirectory()
{
    char current_dir[MAX_PATH];
    if (GetCurrentDirectoryA(MAX_PATH, current_dir) != 0)
    {
        return current_dir;
    }
    else {
        return "Error getting current directory.";
    }
}

#endif


/// <summary>
/// Check if a directory exists, for now this only works for Windows.
/// TODO Make this work on Linux if I get around to testing it on there.
/// </summary>
/// <param name="folderPath">The path to check</param>
/// <returns>If the folder exists</returns>
bool
#ifdef _WIN32
FileFunctions::CheckDirectoryExists(LPCSTR folderPath)
#else
FileFunctions::CheckDirectoryExists(std::string folderPath)
#endif //_WIN32
{
#ifdef _WIN32

	// TODO Move this into another function.
	// Create the directory if it doesn't already exist.
	// CreateDirectory(logPath.c_str(), NULL);
	// CreateDirectory(folderPath, NULL);

	// Check if the directory exists
	// https://stackoverflow.com/questions/8233842/how-to-check-if-directory-exist-using-c-and-winapi
	DWORD ftyp = GetFileAttributesA(folderPath);
	if(ftyp == INVALID_FILE_ATTRIBUTES) {
		return false; // Something is wrong with the path
	}

	if(ftyp & FILE_ATTRIBUTE_DIRECTORY) {
		return true; // This is a valid directory
	}

	return false; // Not a directory

	//
	// TRACE("KCNet - 'ViceExtended\logs' folder not found, creating one for you");
#else
	// Not implemented on other systems.
	return false;
#endif //_WIN32
}

//------------- End file functions ---------------/

