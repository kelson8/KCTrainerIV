#include "IniFunctions.h"

#include "ini.h"

// Ini files, TODO Setup
//std::ofstream KCTrainerIV::mTeleportIniFile;
//std::ofstream KCTrainerIV::mConfigIniFile;

std::string teleportsIniFileName = "KCTrainerTeleports.ini";
std::string configIniFileName = "KCTrainerConfig.ini";

// TODO Test this later
// Well this builds, so I will test it later.
mINI::INIFile ini(configIniFileName);

mINI::INIStructure cfg;

#pragma region ReVCIni

// These are copied from ReVC for easier to use INI functions.

bool
ReadIniIfExists(const char* cat, const char* key, bool* out)
{
	mINI::INIMap<std::string> section = cfg.get(cat);
	if (section.has(key)) {
		char* endPtr;
		*out = strtoul(section.get(key).c_str(), &endPtr, 0);
		return true;
	}
	return false;
}

bool
ReadIniIfExists(const char* cat, const char* key, float* out)
{
	mINI::INIMap<std::string> section = cfg.get(cat);
	if (section.has(key)) {
		char* endPtr;
		*out = strtof(section.get(key).c_str(), &endPtr);
		return true;
	}
	return false;
}

bool
ReadIniIfExists(const char* cat, const char* key, char* out, int size)
{
	mINI::INIMap<std::string> section = cfg.get(cat);
	if (section.has(key)) {
		strncpy(out, section.get(key).c_str(), size - 1);
		out[size - 1] = '\0';
		return true;
	}
	return false;
}

void
StoreIni(const char* cat, const char* key, float val)
{
	char temp[50];
	sprintf(temp, "%f", val);
	cfg[cat][key] = temp;
}

void
StoreIni(const char* cat, const char* key, char* val, int size)
{
	cfg[cat][key] = val;
}

#pragma endregion
