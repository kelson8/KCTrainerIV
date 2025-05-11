// Credit to this project on GitHub for the original source: 
// https://github.com/akifle47/InGameTimecycEditor

// These trainers can be used as a reference for a guide to some of the Natives:
// Three Socks Trainer: https://github.com/Three-Socks/GTAIV-Trainer
// XMC Trainer: https://github.com/EmmanuelU/xmc-mod-menu

//#include "TimecycEditor.h"
#include "KCTrainerIV.h"

#include <iostream>

// TODO Figure out how to fix linker errors when 
// using the PlayerMenu or other files.

// Seems like something in the IVSDK is causing this.


// IVSDK
//#pragma comment(lib, "version.lib")
//#include <windows.h>
//#include <stdint.h>
//#include <string>
//#include <list>
//#include <d3dx9.h>


#include "IVSDK.h"
#include "injector/injector.hpp"
//
#include "Addresses.h"
#include "Scripting/Scripting.h"
#include "Hooks.h"

// My headers
#include "FileFunctions.h"

// Variables
std::string fontFile = "cheat_menu.ttf";

#define IMGUIBUTTON ImGui::Button

// These are required for some items.
void EnablePlayerControl();

void SetupImGuiStyle(ImGuiIO& io);

// I renamed most variables and objects from TimecycEditor to KCTrainerIV, including in the header and main.cpp.

// TODO Set this up with the IVSDK, make an ImGui test on GTA IV.
// I have added IVSDK to this project and it seems to build with it.

using namespace Scripting;

/// <summary>
/// Initialize some of the memory addresses, these can now be obtained from the IVSDK.
/// </summary>
/// <param name="baseAddress">Base address of GTAIV.exe</param>
void KCTrainerIV::Initialize(const uint8_t *baseAddress)
{
	int32_t gameVersion;
	Utils::GetGameVersion(gameVersion);

	
	// These addresses below look like they are the same as in the IVSDK, well some of them anyways.
	switch(gameVersion)
	{
		case 1040:
			mTimeCycle = (Timecycle*)(baseAddress + 0xCF46F0);
			mHour = (int32_t*)(baseAddress + 0xC7AD84);
			mMinutes = (int32_t*)(baseAddress + 0xC7AD80);
			mTimerLength = (uint32_t*)(baseAddress + 0xC7AD88);
			FORCE_WEATHER_NOW = (FORCE_WEATHER_NOWT*)(baseAddress + 0x446930);
			RELEASE_WEATHER = (RELEASE_WEATHERT*)(baseAddress + 0x4469A0);
			SET_TIME_ONE_DAY_FORWARD = (SET_TIME_ONE_DAY_FORWARDT*)(baseAddress + 0x5CB090);
			SET_TIME_ONE_DAY_BACK = (SET_TIME_ONE_DAY_BACKT*)(baseAddress + 0x5CB0D0);
		break;

		case 1070:
			mTimeCycle = (Timecycle*)(baseAddress + 0xDF6080);
			mHour = (int32_t*)(baseAddress + 0xDD5300);
			mMinutes = (int32_t*)(baseAddress + 0xDD52FC);
			mTimerLength = (uint32_t*)(baseAddress + 0xDD5304);
			FORCE_WEATHER_NOW = (FORCE_WEATHER_NOWT*)(baseAddress + 0x5E41D0);
			RELEASE_WEATHER = (RELEASE_WEATHERT*)(baseAddress + 0x5E4240);
			SET_TIME_ONE_DAY_FORWARD = (SET_TIME_ONE_DAY_FORWARDT*)(baseAddress + 0x765060);
			SET_TIME_ONE_DAY_BACK = (SET_TIME_ONE_DAY_BACKT*)(baseAddress + 0x7650A0);
		break;

		case 1080:
			mTimeCycle = (Timecycle*)(baseAddress + 0xFF1150);
			mHour = (int32_t*)(baseAddress + 0xD51694);
			mMinutes = (int32_t*)(baseAddress + 0xD51690);
			mTimerLength = (uint32_t*)(baseAddress + 0xD51698);
			FORCE_WEATHER_NOW = (FORCE_WEATHER_NOWT*)(baseAddress + 0x5A0910);
			RELEASE_WEATHER = (RELEASE_WEATHERT*)(baseAddress + 0x5A0980);
			SET_TIME_ONE_DAY_FORWARD = (SET_TIME_ONE_DAY_FORWARDT*)(baseAddress + 0x711E50);
			SET_TIME_ONE_DAY_BACK = (SET_TIME_ONE_DAY_BACKT*)(baseAddress + 0x711E90);
		break;

		case 1200:
			mTimeCycle = (Timecycle*)(baseAddress + 0x11E8D30);
			mHour = (int32_t*)(baseAddress + 0xE95848);
			mMinutes = (int32_t*)(baseAddress + 0xE9584C);
			mTimerLength = (uint32_t*)(baseAddress + 0xE95840);
			FORCE_WEATHER_NOW = (FORCE_WEATHER_NOWT*)(baseAddress + 0x62E640);
			RELEASE_WEATHER = (RELEASE_WEATHERT*)(baseAddress + 0x62E9E0);
			SET_TIME_ONE_DAY_FORWARD = (SET_TIME_ONE_DAY_FORWARDT*)(baseAddress + 0x7D7530);
			SET_TIME_ONE_DAY_BACK = (SET_TIME_ONE_DAY_BACKT*)(baseAddress + 0x7D74F0);
		break;
	}
	
	// TODO Fix up for menu loading/saving
	//LoadSettings();
}

//------------------ ImGui Setup ------------------/

/// <summary>
/// Setup the imgui fonts
/// </summary>
/// <param name="io"></param>
void
SetupFonts(ImGuiIO& io)
{
	float fontScale = 1.0f;
	io.Fonts->AddFontDefault();

	// First, attempt to load the cheat menu font file
	if (FileFunctions::DoesFileExist(fontFile))
	{
		io.Fonts->AddFontFromFileTTF(fontFile.c_str(), 1.15f);
	}
	// Fall back to loading built in fonts if the file isn't found, I would rather this not just crash.
	else 
	{
		ImFontConfig conf = {};
		conf.SizePixels = 13;
		conf.OversampleH = 2;
		conf.OversampleV = 2;

		static const ImWchar ranges[] =
		{
			0x0020, 0x00FF, // Basic Latin + Latin Supplement
			0,
		};
		const ImWchar* glyph_ranges = ranges;

		io.Fonts->AddFontFromMemoryCompressedBase85TTF(gCousineRegularCompressedDataBase85, conf.SizePixels, &conf, glyph_ranges);
		io.FontGlobalScale = fontScale;
	}
	
}

/// <summary>
/// Setup the ImGui context
/// </summary>
void
SetupContext()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

	SetupImGuiStyle(io);

	SetupFonts(io);
}


/// <summary>
/// Setup the ImGui style
/// </summary>
/// <param name="io">The IO for ImGui</param>
void SetupImGuiStyle(ImGuiIO &io)
{
	io.IniFilename = NULL;

	//ImGui Style
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		style->FrameRounding = 1;
		style->WindowPadding.x = 50;
		style->WindowPadding.y = 10;
		style->FramePadding.x = 1;
		style->FramePadding.y = 3;
		style->ItemSpacing.x = 10;
		style->ItemSpacing.x = 5;
		style->ScrollbarSize = 20;
		style->ScrollbarRounding = 1;
		style->GrabMinSize = 15;

		style->WindowBorderSize = 0;
		style->WindowRounding = 1;

		style->WindowTitleAlign.x = 0.5;
		style->WindowTitleAlign.y = 0.5;
		style->WindowMenuButtonPosition = 0;

		style->SeparatorTextBorderSize = 3;
	}
	//ImGui Color
	{
#define NEW_COLORS
		// Colors/Style from Cheat Menu: 
		// Credit to user-grinch on github for the style code here.
		// https://github.com/user-grinch/Cheat-Menu/blob/master/src/cheatmenu.cpp#L271-L335
#ifdef NEW_COLORS
		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;

		style->WindowPadding = ImVec2(8, 8);
		style->WindowRounding = 5.0f;
		style->FramePadding = ImVec2(8, 8);
		style->FrameRounding = 5.0f;
		style->PopupRounding = 5.0f;
		style->ItemSpacing = ImVec2(7, 7);
		style->ItemInnerSpacing = ImVec2(7, 7);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 12.0f;
		style->ScrollbarRounding = 10.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 3.0f;

		style->ChildBorderSize = 0;
		style->WindowBorderSize = 0;
		style->FrameBorderSize = 0;
		style->TabBorderSize = 0;
		style->PopupBorderSize = 0;

		style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.35f, 0.33f, 0.3f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.06f, 0.95f);
		style->Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.06f, 0.05f, 0.06f, 0.95f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.95f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.12f, 0.12f, 0.12f, 0.94f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.95f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.95f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.5f, 0.5f, 0.5f, 0.3f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.7f, 0.7f, 0.7f, 0.3f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.9f, 0.9f, 0.9f, 0.3f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_Separator] = ImVec4(0.15f, 0.15f, 0.15f, 0.95f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.15f, 0.15f, 0.15f, 0.95f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		style->Colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.15f, 0.95f);
		style->Colors[ImGuiCol_TabHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style->Colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.12f, 0.12f, 0.12f, 0.00f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.06f, 0.05f, 0.06f, 0.95f);
		style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.6f);
#else
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.98f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.84f, 0.84f, 0.84f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.72f, 0.72f, 0.72f, 1.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.77f, 0.77f, 0.77f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.43f, 0.43f, 0.43f, 0.98f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
#endif
	}
}

/// <summary>
/// Initialize and setup ImGui if not already initialized, otherwise do nothing.
/// </summary>
/// <param name="d3d9Device"></param>
void KCTrainerIV::InitializeImGui(IDirect3DDevice9 *d3d9Device)
{
	if(!mIsImGuiInitialized)
	{
		D3DDEVICE_CREATION_PARAMETERS creationParams;
		d3d9Device->GetCreationParameters(&creationParams);

		// Oops, I forgot this.
		SetupContext();

		ImGui_ImplWin32_Init(creationParams.hFocusWindow);
		ImGui_ImplDX9_Init(d3d9Device);

		mIsImGuiInitialized = true;
	}
}

//------------------ End ImGui Setup ------------------/

#ifdef DISABLED_CODE
void KCTrainerIV::InitializeColors()
{
	for(uint32_t time = 0; time < NUM_HOURS; time++)
	{
		for(uint32_t weather = 0; weather < NUM_WEATHERS; weather++)
		{
			Utils::U32ToFloat4(mTimeCycle->mParams[time][weather].mAmbient0Color, mAmbient0ColorFloat4[time][weather]);
			Utils::U32ToFloat4(mTimeCycle->mParams[time][weather].mAmbient1Color, mAmbient1ColorFloat4[time][weather]);
			Utils::U32ToFloat4(mTimeCycle->mParams[time][weather].mDirLightColor, mDirLightColorFloat4[time][weather]);
			Utils::U32ToFloat4(mTimeCycle->mParams[time][weather].mSkyBottomColorFogDensity, mSkyBottomColorFogDensityFloat4[time][weather]);
			Utils::U32ToFloat4(mTimeCycle->mParams[time][weather].mSunCore, mSunCoreFloat4[time][weather]);
			Utils::U32ToFloat4(mTimeCycle->mParams[time][weather].mLowCloudsColor, mLowCloudsColorFloat4[time][weather]);
			Utils::U32ToFloat4(mTimeCycle->mParams[time][weather].mBottomCloudsColor, mBottomCloudsColorFloat4[time][weather]);
			Utils::U32ToFloat4(mTimeCycle->mParams[time][weather].mWater, mWaterFloat4[time][weather]);
			Utils::U32ToFloat4(mTimeCycle->mParams[time][weather].mColorCorrection, mColorCorrectionFloat4[time][weather]);
			Utils::U32ToFloat4(mTimeCycle->mParams[time][weather].mColorAdd, mColorAddFloat4[time][weather]);
		}
	}
}
#endif

//------------------ Loading/Saving ------------------/

/// <summary>
/// Save trainer settings to file, this is incomplete
/// TODO Set this up
/// </summary>
void KCTrainerIV::SaveSettings()
{
	// TODO Why is this .bin?
	std::ofstream file("KCTrainerIV.bin", std::ios::binary);

	// Make this do nothing if the file doesn't exist
	if(!file.good())
	{
		return;
	}
	
	file.write((char*)&mSettingsFileMajorVersion, sizeof(char));
	file.write((char*)&mSettingsFileMinorVersion, sizeof(char));
	file.write((char*)&mWindowPos.x, sizeof(float));
	file.write((char*)&mWindowPos.y, sizeof(float));
	file.write((char*)&mWindowSize.x, sizeof(float));
	file.write((char*)&mWindowSize.y, sizeof(float));
	file.write((char*)&mFontScale, sizeof(float));
	file.write((char*)&mOpenWindowKey, sizeof(ImGuiKey));
	file.write((char*)&mToggleCameraControlKey, sizeof(ImGuiKey));
	file.write((char*)&mItemInnerSpacing, sizeof(float));
}

/// <summary>
/// Load trainer settings from file, this is incomplete
/// TODO Set this up
/// </summary>
void KCTrainerIV::LoadSettings()
{
	// TODO Why is this .bin?
	std::ifstream file("KCTrainerIV.bin", std::ios::binary);

	// Make this do nothing if the file doesn't exist
	if(!file.good())
	{
		return;
	}
	
	file.seekg(2);
	file.read((char*)&mWindowPos.x, sizeof(float));
	file.read((char*)&mWindowPos.y, sizeof(float));
	file.read((char*)&mWindowSize.x, sizeof(float));
	file.read((char*)&mWindowSize.y, sizeof(float));
	file.read((char*)&mFontScale, sizeof(float));
	file.read((char*)&mOpenWindowKey, sizeof(ImGuiKey));
	file.read((char*)&mToggleCameraControlKey, sizeof(ImGuiKey));
	file.read((char*)&mItemInnerSpacing, sizeof(float));

	// If ImGui is initialized, set the mFontScale and mItemInnerSpacing variables.
	if(mIsImGuiInitialized)
	{
		ImGui::GetIO().FontGlobalScale = mFontScale;
		ImGui::GetStyle().ItemInnerSpacing.x = mItemInnerSpacing;
	}

	// Set the gamewindow, if not found exit.
	HWND gameWindow = FindWindow(L"grcWindow", L"GTAIV");
	if(!gameWindow)
	{
		return;
	}

	// Get the game rect, I think this is the window size.
	RECT gameWindowRect = {};
	GetWindowRect(gameWindow, &gameWindowRect);

	// Set the window height and width variables
	int32_t gameWindowWidth = gameWindowRect.right - gameWindowRect.left;
	int32_t gameWindowHeight = gameWindowRect.bottom - gameWindowRect.top;

	if(mWindowPos.x >= gameWindowWidth || mWindowPos.x < 0.0f)
	{
		mWindowPos.x = 0.0f;
	}
	if(mWindowPos.y >= gameWindowHeight || mWindowPos.y < 0.0f)
	{
		mWindowPos.y = 0.0f;
	}
	//
}

//------------------ End Loading/Saving ------------------/

/// <summary>
/// WndProc for ImGui usage, I'm quite sure this overrides the mouse in GTA IV.
/// </summary>
/// <param name="hWnd"></param>
/// <param name="uMsg"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
bool KCTrainerIV::OnWndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(mIsImGuiInitialized)
	{
		if(ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		{
			return true;
		}
	}

	return false;
}

//------------------ Main ImGui update thread ------------------/

/// <summary>
/// Run all ImGui logic
/// Check if the window open key is pressed, if so show/hide the ImGui window.
/// Disable mouse control if ImGui is open.
/// </summary>
void KCTrainerIV::Update()
{
	static bool prevShowWindow = 0;
	prevShowWindow = mShowWindow;
	
	if(ImGui::IsKeyPressed(mOpenWindowKey))
	{
		mShowWindow = !mShowWindow;

		if(mShowWindow)
		{
			LoadSettings();
			ImGui::GetIO().FontGlobalScale = mFontScale;

			mDisableMouseControl = true;
			ImGui::GetIO().MouseDrawCursor = 1;
		}
		else
		{
			mDisableMouseControl = false;
			ImGui::GetIO().MouseDrawCursor = 0;
		}
	}
	
	if(mShowWindow)
	{
		if(ImGui::IsKeyPressed(mToggleCameraControlKey))
		{
			mDisableMouseControl = !mDisableMouseControl;
		}

		
		//if(mLockTimeAndWeather)
		//{
		//	*mHour = mSelectedHour;
		//	*mMinutes = mSelectedMinutes;

		//	FORCE_WEATHER_NOW(mSelectedWeather);
		//}
	}
	
	bool windowWasJustClosed = prevShowWindow && !mShowWindow;
	if(windowWasJustClosed)
	{
		//RELEASE_WEATHER();
		*mTimerLength = 2000;
	}

	bool windowWasJustOpened = !prevShowWindow && mShowWindow;
	//if(windowWasJustOpened && mLockTimeAndWeather)
	if(windowWasJustOpened)
	{
		*mTimerLength = 30000;
	}
}

//------------------ D3D9 Device ------------------/

/// <summary>
/// Initalize ImGui, and invalidate device objects for D3D9.
/// </summary>
/// <param name="d3d9Device"></param>
void KCTrainerIV::OnBeforeD3D9DeviceReset(IDirect3DDevice9 *d3d9Device)
{
	InitializeImGui(d3d9Device);

	ImGui_ImplDX9_InvalidateDeviceObjects();
}

/// <summary>
/// Create ImGui device objects for D3D9.
/// </summary>
void KCTrainerIV::OnAfterD3D9DeviceReset()
{
	ImGui_ImplDX9_CreateDeviceObjects();
}

/// <summary>
/// Initalize ImGui, update menu.
/// Setup ImGui new frames.
/// If mShowWindow is true, draw the ImGui window: DrawMainWindow();
/// Also runs ImGui render.
/// </summary>
/// <param name="d3d9Device"></param>
void KCTrainerIV::OnBeforeD3D9DeviceEndScene(IDirect3DDevice9 *d3d9Device)
{
	// I think this is a bad idea to run before the game is even started.
	//int playerPed;
	//GET_PLAYER_CHAR(CONVERT_INT_TO_PLAYERINDEX(GET_PLAYER_ID()), &playerPed);

	//int playerId = GET_PLAYER_ID();

	InitializeImGui(d3d9Device);

	Update();

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if(mShowWindow)
	{

		// TODO Setup to disable mouse movement, stop the camera from constantly spinning if the menu is open.

		// Oops, this just crashes the game..
		DrawMainWindow();
		
#ifdef DISABLED_CODE
		// TODO Possibly re-use these
		DrawSaveWindow();
		DrawLoadWindow();
		DrawSettingsWindow();
		DrawSetParamForAllHoursAndWeathersWindow();
#endif
	}

	// TODO Test this.
	//EnablePlayerControl();
		//SET_PLAYER_CONTROL(playerPed, 1);

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

//------------------ End D3D9 Device ------------------/


/// <summary>
/// Get the player coords as a Vector3
/// </summary>
/// <returns>A Vector3 of the player coords</returns>
Vector3 GetPlayerCoords()
{
	int playerPed;
	GET_PLAYER_CHAR(CONVERT_INT_TO_PLAYERINDEX(GET_PLAYER_ID()), &playerPed);

	int playerId = GET_PLAYER_ID();
	float posX, posY, posZ;

	GET_CHAR_COORDINATES(playerPed, &posX, &posY, &posZ);

	Vector3* playerPos = new Vector3{posX, posY, posZ};

	return *playerPos;
}

float GetPlayerHeading()
{
	int playerPed;
	GET_PLAYER_CHAR(CONVERT_INT_TO_PLAYERINDEX(GET_PLAYER_ID()), &playerPed);

	int playerId = GET_PLAYER_ID();

	float playerHeading;

	GET_CHAR_HEADING(playerPed, &playerHeading);
	
	return playerHeading;

}

/// <summary>
/// Load vehicle model
/// TODO Figure this out in IVSDK.
/// </summary>
/// <param name="modelHash"></param>
void RequestModel(int modelHash)
{
	// First, check if the model exists
	if (IS_MODEL_IN_CDIMAGE(modelHash))
	{
		REQUEST_MODEL(modelHash);
		
		// Wait on the model to load
		while (!HAS_MODEL_LOADED(modelHash))
		{
			WAIT(0);
		}

		//NATIVE_REQUEST_MODEL
	}
}

/// <summary>
/// Get the player char
/// </summary>
/// <returns>The player char as an int</returns>
int GetPlayerChar()
{
	int playerPed;
	GET_PLAYER_CHAR(CONVERT_INT_TO_PLAYERINDEX(GET_PLAYER_ID()), &playerPed);
	return playerPed;
}

/// <summary>
/// Get the player ID
/// TODO Test this
/// </summary>
/// <returns>The player ID as an int.</returns>
int GetPlayerId()
{
	int playerId = GET_PLAYER_ID();
	return playerId;
}

/// <summary>
/// Enable the player movement and control.
/// Check if the player isn't dead and is playing.
/// </summary>
void EnablePlayerControl()
{
	if (IS_PLAYER_PLAYING(GetPlayerId()))
	{
		SET_PLAYER_CONTROL(GetPlayerId(), true);
	}
}

/// <summary>
/// Disable the player movement and control.
/// Check if the player isn't dead and is playing.
/// </summary>
void DisablePlayerControl()
{
	if (IS_PLAYER_PLAYING(GetPlayerId()))
	{
		SET_PLAYER_CONTROL(GetPlayerId(), false);
	}
	
}

/// <summary>
/// Check if the player is in a vehicle.
/// </summary>
/// <returns>If the player is in a vehicle.</returns>
bool IsPlayerInVehicle()
{
	int playerPed;
	GET_PLAYER_CHAR(CONVERT_INT_TO_PLAYERINDEX(GET_PLAYER_ID()), &playerPed);

	int playerId = GET_PLAYER_ID();

	if (IS_CHAR_SITTING_IN_ANY_CAR(playerPed))
	{
		return true;
	}

	return false;
}

/// <summary>
/// Get the players current vehicle if they are in one.
/// </summary>
/// <returns>Players current vehicle, 0 if not in one.</returns>
int GetPlayerVehicle()
{
	int playerVeh;
	if (IsPlayerInVehicle())
	{
		GET_PLAYERS_LAST_CAR_NO_SAVE(&playerVeh);
		return playerVeh;
	}

	return 0;
}

/// <summary>
/// Fade the screen out for the specified amount of time.
/// </summary>
/// <param name="ms">The time to fade out using miliseconds</param>
void FadeScreenOut(int ms)
{
	DO_SCREEN_FADE_OUT(ms);
}

/// <summary>
/// Fade the screen in for the specified amount of time.
/// </summary>
/// <param name="ms">The time to fade in using miliseconds</param>
void FadeScreenIn(int ms)
{
	DO_SCREEN_FADE_IN(ms);
}

bool godMode = false;

/// <summary>
/// Draw all the player menu functions
/// TODO Make this into KCTrainerIV::DrawPlayerMenu or something
/// </summary>
void DrawPlayerMenu()
{
	//auto ped = CPlayerInfo::GetPlayerInfo(CWorld::PlayerInFocus)->m_pPlayerPed;
	int playerPed;
	GET_PLAYER_CHAR(CONVERT_INT_TO_PLAYERINDEX(GET_PLAYER_ID()), &playerPed);

	int playerId = GET_PLAYER_ID();

	//auto playerChar = Scripting::GET_PLAYER_CHAR();
	// Well these seem to break it..
	//auto playerID = Scripting::GET_PLAYER_ID();

	// I got this working!!!!
	// Also I added a namespace for scripting in the top of this file.

	ImGui::Checkbox("God Mode", &godMode);

	// TODO Come up with a better solution, I think this will override the other mod menus if I have it off.
	if (godMode)
	{
		SET_CHAR_PROOFS(playerPed, true, true, true, true, true);
	}
	else 
	{
		SET_CHAR_PROOFS(playerPed, false, false, false, false, false);
	}

	if (ImGui::Button("Heal player"))
	{
		// I think 100 was too little for the health, this was killing me lol.
		SET_CHAR_HEALTH(playerPed, 250);
	}

	if (ImGui::Button("Give player armor"))
	{
		ADD_ARMOUR_TO_CHAR(playerPed, 100);
	}

	// Get player coords and heading.
	Vector3 playerCoords = GetPlayerCoords();
	float playerX = playerCoords.x;
	float playerY = playerCoords.y;
	float playerZ = playerCoords.z;

	float playerHeading = GetPlayerHeading();

	std::string playerCoordsText = "X: " + std::to_string(playerX) 
		+ " Y: " + std::to_string(playerY)
		+ " Z: " + std::to_string(playerZ);
	
	std::string playerHeadingText = "Heading: " + std::to_string(playerHeading);

	
	ImGui::NewLine();

	ImGui::Text("Player coords: ");
	ImGui::Text(playerCoordsText.c_str());

	ImGui::Text("Player Heading: ");
	ImGui::Text(playerHeadingText.c_str());

	//if (ImGui::Button("Enable god mode"))
	//{
	//	//Scripting::SET_CHAR_PROOFS(ped, true, true, true, true, true);
	//	//Scripting::SET_CHAR_PROOFS(playerID, true, true, true, true, true);
	//	SET_CHAR_PROOFS(playerPed, true, true, true, true, true);

	//}

	//if (ImGui::Button("Disable god mode"))
	//{
	//	//Scripting::SET_CHAR_PROOFS(ped, true, true, true, true, true);
	//	//Scripting::SET_CHAR_PROOFS(playerID, false, false, false, false, false);
	//	SET_CHAR_PROOFS(playerPed, false, false, false, false, false);
	//}
}

bool warpIntoVehicle = false;
/// <summary>
/// Draw the vehicle menu, this is incomplete.
/// If doing vehicle functions, always check if player is in a vehicle.
/// The game will freeze and crash if you don't check for the player being in a vehicle.
/// </summary>
void DrawVehicleMenu()
{
	int playerPed;
	GET_PLAYER_CHAR(CONVERT_INT_TO_PLAYERINDEX(GET_PLAYER_ID()), &playerPed);

	int playerId = GET_PLAYER_ID();

	// Get player coords and heading.
	Vector3 playerCoords = GetPlayerCoords();
	float playerX = playerCoords.x;
	float playerY = playerCoords.y;
	float playerZ = playerCoords.z;

	float playerHeading = GetPlayerHeading();

	//int playerVeh;

	int playerVeh = GetPlayerVehicle();

	// TODO Test this, I don't think this worked or at least I didn't see it fix the car tires
	if (ImGui::Button("Repair vehicle"))
	{
		if (IsPlayerInVehicle()) 
		{
			SET_CAR_HEALTH(playerVeh, 1000);
			SET_ENGINE_HEALTH(playerVeh, 1000.0f);
		}
		else {
			std::cout << "Not in a vehicle" << std::endl;
		}
		//if (IsPlayerInVehicle())
		//{
		//	//GET_PLAYERS_LAST_CAR_NO_SAVE(&playerVeh);
		//}
	}

	// Well this seems to work
	if (ImGui::Button("Launch vehicle in air"))
	{
		if (IsPlayerInVehicle()) 
		{
			SET_CAR_COORDINATES(playerVeh, playerX, playerY, playerZ + 50);
		}

	}

	// Extras, TODO Setup
	/*
	* Car lights
	FORCE_CAR_LIGHTS
	SET_VEH_HAZARDLIGHTS
	SET_VEH_INDICATORLIGHTS

	* Make vehicle strong
	SET_CAR_STRONG
	SET_VEH_HAS_STRONG_AXLES

	SET_CAR_COLLISION

	* Make vehicle not be able to be damaged
	SET_CAR_CAN_BE_DAMAGED
	SET_CAR_CAN_BE_VISIBLY_DAMAGED
	SET_CAR_PROOFS
	
	SET_CAR_WATERTIGHT

	SET_VEHICLE_DIRT_LEVEL
	SET_CAR_VISIBLE

	SET_CAR_AS_MISSION_CAR
	*/

	// These don't work yet, crashes the game.
	// TODO Fix these to work.

	//int vehicle;

	//ImGui::Checkbox("Warp into vehicle", &warpIntoVehicle);

	//ImGui::Text("Spawn vehicles");
	//if (ImGui::Button("Infernus"))
	//{
	//	RequestModel(MODEL_INFERNUS);
	//	if (warpIntoVehicle)
	//	{
	//		CREATE_CAR(MODEL_INFERNUS, playerX, playerY, playerZ + 3, &vehicle, true);
	//		WARP_CHAR_INTO_CAR(playerPed, vehicle);
	//	}
	//	else 
	//	{
	//		CREATE_CAR(MODEL_INFERNUS, playerX, playerY, playerZ + 3, &vehicle, true);
	//	}
	//	
	//}
}

// Moved outside the draw menu, I think that breaks it.
float posX, posY, posZ;
float heading;

/// <summary>
/// Draw the teleport menu.
/// </summary>
void DrawTeleportMenu()
{

	// TODO Add a save option for saving current coordinates to a file.
	ImGui::InputFloat("X: ", &posX);
	ImGui::InputFloat("Y: ", &posY);
	ImGui::InputFloat("Z: ", &posZ);

	// This works for a basic teleporter
	if (ImGui::Button("Teleport to coords"))
	{
		if (IS_PLAYER_PLAYING(GetPlayerId()))
		{
			// Check if the values are valid and not null
			if (posX && posY && posZ)
			{
				//DO_SCREEN_FADE_OUT(500);
				FadeScreenOut(500);

				// Well this crashes it.
				//LOAD_SCENE(posX, posY, posZ);

				SET_CHAR_COORDINATES(GetPlayerChar(), posX, posY, posZ);
				
				//DO_SCREEN_FADE_IN(500);

				FadeScreenIn(500);
				
			}
		}
	}

}


/// <summary>
/// Draw the test menu, I will have misc testing features in here.
/// </summary>
void DrawTestMenu()
{
	int playerPed;
	GET_PLAYER_CHAR(CONVERT_INT_TO_PLAYERINDEX(GET_PLAYER_ID()), &playerPed);

	int playerId = GET_PLAYER_ID();

	// TODO Does this bring up a keyboard in game?
	// This didn't do anything
	// Ohh, this is a network function, no wonder it didn't work..
	//if (ImGui::Button("Text input test"))
	//{
	//	SET_TEXT_INPUT_ACTIVE(true);
	//}

}

void DrawAboutMenu()
{
	ImGui::Text("Welcome to the KCNet IV ImGui mod menu");
	ImGui::Text("I plan on adding plenty of features to this.");
	ImGui::Text("\n\nI have made this with the MIT license.");
	ImGui::Text("Menu base: \nhttps://github.com/akifle47/InGameTimecycEditor");

}

void KCTrainerIV::DrawMainWindow()
{

	//int playerPed;
	//GET_PLAYER_CHAR(CONVERT_INT_TO_PLAYERINDEX(GET_PLAYER_ID()), &playerPed);

	//int playerId = GET_PLAYER_ID();

	// This should disable the pause menu if the menu is being drawn
	//DISABLE_PAUSE_MENU(true);


	// Well, this works but it doesn't get turned off lol.
	//SET_PLAYER_CONTROL(GetPlayerId(), false);

	//DisablePlayerControl();

	const char *timeOfDayNames[NUM_HOURS] = {"Midnight", "5AM", "6AM", "7AM", "9AM", "Midday", "18PM", "19PM", "20PM", "21PM", "22PM"};
	const char *timeOfDayName = timeOfDayNames[mSelectedHourIndex];
	const char *weatherNames[NUM_WEATHERS - 1] = {"EXTRASUNNY", "SUNNY", "SUNNY_WINDY", "CLOUDY", "RAIN", "DRIZZLE", "FOGGY", "LIGHTNING"};
	const char *weatherName = weatherNames[mSelectedWeather];

	ImGui::Begin("KCNet Trainer IV", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus);
	ImGui::SetWindowPos(mWindowPos);
	ImGui::SetWindowSize(mWindowSize);


	ImGui::Text("KCNet Trainer IV");

	// Contains all the menu drawing logic, each menu is separated into it's own function to make this a bit neater.
	if (ImGui::BeginTabBar("Functions")) {

		// Player
		if (ImGui::BeginTabItem("Player")) {
			DrawPlayerMenu();

			ImGui::EndTabItem();
		}

		// Vehicle
		if (ImGui::BeginTabItem("Vehicle")) {
			DrawVehicleMenu();

			ImGui::EndTabItem();
		}

		// Teleport
		if (ImGui::BeginTabItem("Teleport")) {
			DrawTeleportMenu();

			ImGui::EndTabItem();
		}
		

		// Test
		if (ImGui::BeginTabItem("Test")) {
			DrawTestMenu();

			ImGui::EndTabItem();
		}

		// About
		if (ImGui::BeginTabItem("About")) {
			DrawAboutMenu();

			ImGui::EndTabItem();
		}
		

		ImGui::EndTabBar();
	}

	
	
	//if(ImGui::BeginMenuBar())
	//{
	//	if(ImGui::BeginMenu("Load"))
	//	{
	//		if(ImGui::MenuItem("timecyc.dat##Load"))
	//		{
	//			mTimeCycle->Load("pc/data/timecyc.dat", NULL, 0);
	//			InitializeColors();
	//		}
	//		if(ImGui::MenuItem("Load From"))
	//		{
	//			mShowLoadWindow = true;
	//		}

	//		ImGui::EndMenu();
	//	}

	//	if(ImGui::BeginMenu("Save"))
	//	{
	//		if(ImGui::MenuItem("timecyc.dat##Save"))
	//		{
	//			mTimeCycle->Save("pc/data/timecyc.dat", NULL, 0);
	//		}

	//		if(ImGui::MenuItem("Save As"))
	//		{
	//			mShowSaveWindow = true;
	//		}

	//		ImGui::EndMenu();
	//	}

	//	if(ImGui::BeginMenu("Settings"))
	//	{
	//		mShowSettingsWindow = true;

	//		ImGui::EndMenu();
	//	}

	//	if(ImGui::BeginMenu("Tools"))
	//	{
	//		if(ImGui::MenuItem("Set Param For All Hours And Weathers"))
	//		{
	//			mShowSetParamForAllHoursAndWeathersWindow = true;
	//		}

	//		ImGui::EndMenu();
	//	}

	//	ImGui::EndMenuBar();
	//}
	
	ImGui::End();
}


// Leaving these in place for references, I can possibly use it later.
// Original TimecycEditor code
#ifdef DISABLED_CODE
void KCTrainerIV::DrawSaveWindow()
{
	if(mShowSaveWindow)
	{
		static char errorMessage[256];
		static char fileName[256] = "filename.dat";

		ImGui::Begin("Save As");
		{
			ImGui::InputText("##fileName", fileName, 256);

			if(ImGui::Button("Save"))
			{
				if(mTimeCycle->Save(fileName, errorMessage, 256))
				{
					mShowSaveWindow = false;
				}
			}

			ImGui::SameLine();

			if(ImGui::Button("Cancel"))
			{
				mShowSaveWindow = false;
			}

			ImGui::Text(errorMessage);
		}
		ImGui::End();
	}
}

void KCTrainerIV::DrawLoadWindow()
{
	if(mShowLoadWindow)
	{
		static char errorMessage[256];
		static char fileName[256] = "filename.dat";

		ImGui::Begin("Load From");
		{
			ImGui::InputText("##fileName", fileName, 256);

			if(ImGui::Button("Load"))
			{
				if(mTimeCycle->Load(fileName, errorMessage, 256))
				{
					InitializeColors();
					mShowLoadWindow = false;
				}
			}

			ImGui::SameLine();

			if(ImGui::Button("Cancel"))
			{
				mShowLoadWindow = false;
			}

			ImGui::Text(errorMessage);
		}
		ImGui::End();
	}
}

void KCTrainerIV::DrawSettingsWindow()
{
	if(mShowSettingsWindow)
	{
		ImGui::Begin("Settings", NULL, ImGuiWindowFlags_NoResize);
		{
			ImGui::SetWindowSize(ImVec2(330.0f, 345.0f));
			
			ImGui::Text("Position");
			ImGui::DragFloat2("##Position", (float*)&mWindowPos, 1.0f, 0.0f, FLT_MAX);
			ImGui::Text("Size");
			ImGui::DragFloat2("##Size", (float*)&mWindowSize, 1.0f, 1.0f, FLT_MAX);
			ImGui::Text("Font Scale");
			ImGui::DragFloat("##Font Scale", &mFontScale, 0.001f, 0.1f, 2.0f);
			ImGui::Text("Item Inner Spacing");
			ImGui::DragFloat("##Item Inner Spacing", &mItemInnerSpacing, 0.1f, 0.1f, 100.0f);

			ImGui::GetIO().FontGlobalScale = mFontScale;
			ImGui::GetStyle().ItemInnerSpacing.x = mItemInnerSpacing;

			//open window key setting
			ImGui::NewLine();

			static ImGuiKey newOpenWindowKey = mOpenWindowKey;
			std::string openEditorKeyStr = "Open Editor Window Key: " + std::string(ImGui::GetKeyName(newOpenWindowKey));
			ImGui::Text(openEditorKeyStr.c_str());

			static bool showChangeWindowKeyPrompt = false;
			if(ImGui::Button("Change"))
			{
				showChangeWindowKeyPrompt = true;
			}

			if(showChangeWindowKeyPrompt)
			{
				ImGui::TextWrapped("Press the Key You Want to Use to Open the Editor Window");

				for(uint32_t i = ImGuiKey_Tab; i < ImGuiKey_KeypadEqual; i++)
				{
					if(ImGui::IsKeyPressed((ImGuiKey)i) && i != mToggleCameraControlKey)
					{
						newOpenWindowKey = (ImGuiKey)i;
						showChangeWindowKeyPrompt = false;
						break;
					}
				}
			}

			ImGui::NewLine();
			
			//toggle mouse control setting
			static ImGuiKey newCameraToggleKey = mToggleCameraControlKey;
			std::string toggleCameraKeyStr = "Toggle Camera Control Key: " + std::string(ImGui::GetKeyName(newCameraToggleKey));
			ImGui::Text(toggleCameraKeyStr.c_str());

			static bool showChangeCameraToggleKeyPrompt = false;
			if(ImGui::Button("Change##2"))
			{
				showChangeCameraToggleKeyPrompt = true;
			}

			if(showChangeCameraToggleKeyPrompt)
			{
				ImGui::TextWrapped("Press the Key You Want to Use to Toggle Camera Control");

				for(uint32_t i = ImGuiKey_Tab; i < ImGuiKey_KeypadEqual; i++)
				{
					if(ImGui::IsKeyPressed((ImGuiKey)i) && i != mOpenWindowKey)
					{
						newCameraToggleKey = (ImGuiKey)i;
						showChangeCameraToggleKeyPrompt = false;
						break;
					}
				}
			}

			ImGui::NewLine();

			if(ImGui::Button("Save"))
			{
				mOpenWindowKey = newOpenWindowKey;
				mToggleCameraControlKey = newCameraToggleKey;
				SaveSettings();
				mShowSettingsWindow = false;
				showChangeWindowKeyPrompt = false;
				showChangeCameraToggleKeyPrompt = false;
			}

			ImGui::SameLine();

			if(ImGui::Button("Cancel"))
			{
				newOpenWindowKey = mOpenWindowKey;
				newCameraToggleKey = mToggleCameraControlKey;
				LoadSettings();
				mShowSettingsWindow = false;
				showChangeWindowKeyPrompt = false;
				showChangeCameraToggleKeyPrompt = false;
			}
		}
		ImGui::End();
	}
}

#endif //DISABLED_CODE

#ifdef DISABLED_CODE
void KCTrainerIV::DrawSetParamForAllHoursAndWeathersWindow()
{
	if(mShowSetParamForAllHoursAndWeathersWindow)
	{
		ImGui::Begin("Set Param For All Hours And Weathers", NULL, ImGuiWindowFlags_NoResize);
		{
			ImGui::SetWindowSize(ImVec2(330.0f, 165.0f));

			static int currentItemIndex = 0;

			ImGui::Text("Parameter");
			if(ImGui::BeginCombo("##Parameter", std::get<0>(mTimecycParamNameOffsetAndType[currentItemIndex]).c_str(), 0))
			{
				for(uint32_t i = 0; i < mTimecycParamNameOffsetAndType.size(); i++)
				{
					const bool isSelected = currentItemIndex == i;
					if(ImGui::Selectable(std::get<0>(mTimecycParamNameOffsetAndType[i]).c_str(), isSelected))
					{
						currentItemIndex = i;
					}
				}
				ImGui::EndCombo();
			}

			static float valueFloat = 0.0f;
			static int32_t valueInt = 0;
			static float valueColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};

			switch(std::get<2>(mTimecycParamNameOffsetAndType[currentItemIndex]))
			{
				case TIMECYCPARAMTYPE_COLOR_U32:
					ImGui::Text("Value");
					ImGui::ColorEdit4("##Value", valueColor);

					if(ImGui::Button("Apply"))
					{
						uint32_t colorU32;
						Utils::Float4ToU32(valueColor, colorU32);

						for(uint32_t weather = 0; weather < NUM_WEATHERS; weather++)
						{
							for(uint32_t hour = 0; hour < NUM_HOURS; hour++)
							{
								uint32_t address = (uint32_t)&mTimeCycle->mParams[hour][weather];
								*(uint32_t *)(address + std::get<1>(mTimecycParamNameOffsetAndType[currentItemIndex])) = colorU32;
							}
						}

						InitializeColors();
						mShowSetParamForAllHoursAndWeathersWindow = false;
					}
				break;

				case TIMECYCPARAMTYPE_COLOR_FLOAT3:
					ImGui::Text("Value");
					ImGui::ColorEdit4("##Value", valueColor, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_Float);

					if(ImGui::Button("Apply"))
					{
						for(uint32_t weather = 0; weather < NUM_WEATHERS; weather++)
						{
							for(uint32_t hour = 0; hour < NUM_HOURS; hour++)
							{
								uint32_t address = (uint32_t)&mTimeCycle->mParams[hour][weather];
								*(float *)(address + std::get<1>(mTimecycParamNameOffsetAndType[currentItemIndex]) + 0x0) = valueColor  [0];
								*(float *)(address + std::get<1>(mTimecycParamNameOffsetAndType[currentItemIndex]) + 0x4) = valueColor  [1];
								*(float *)(address + std::get<1>(mTimecycParamNameOffsetAndType[currentItemIndex]) + 0x8) = valueColor  [2];
							}
						}

						mShowSetParamForAllHoursAndWeathersWindow = false;
					}
				break;

				case TIMECYCPARAMTYPE_FLOAT:
					ImGui::Text("Value");
					ImGui::DragFloat("##Value", &valueFloat, 0.005f);

					if(ImGui::Button("Apply"))
					{
						for(uint32_t weather = 0; weather < NUM_WEATHERS; weather++)
						{
							for(uint32_t hour = 0; hour < NUM_HOURS; hour++)
							{
								uint32_t address = (uint32_t)&mTimeCycle->mParams[hour][weather];
								*(float *)(address + std::get<1>(mTimecycParamNameOffsetAndType[currentItemIndex])) = valueFloat;
							}
						}

						mShowSetParamForAllHoursAndWeathersWindow = false;
					}
				break;

				case TIMECYCPARAMTYPE_INT:
					ImGui::Text("Value");
					ImGui::DragInt("##Value", &valueInt, 0.5f);

					if(ImGui::Button("Apply"))
					{
						for(uint32_t weather = 0; weather < NUM_WEATHERS; weather++)
						{
							for(uint32_t hour = 0; hour < NUM_HOURS; hour++)
							{
								uint32_t address = (uint32_t)&mTimeCycle->mParams[hour][weather];
								*(int32_t *)(address + std::get<1>(mTimecycParamNameOffsetAndType[currentItemIndex])) = valueInt;
							}
						}

						mShowSetParamForAllHoursAndWeathersWindow = false;
					}
				break;
			}
		}
		ImGui::End();
	}
}
#endif //DISABLED_CODE

int32_t KCTrainerIV::TimecycTimeIndexToGameTime(const int32_t timeIndex)
{
	const int32_t timecycTimeIndexToGameTime[NUM_HOURS] = {0, 5, 6, 7, 9, 12, 18, 19, 20, 21, 22};

	return timecycTimeIndexToGameTime[timeIndex];
}
int32_t KCTrainerIV::GameTimeToTimecycTimeIndex(const int32_t gameTime)
{
	const int32_t gameTimeToTimecycTimeIndex[24] = {0, 0, 0, 0, 0, 1, 2, 3, 3, 4, 4, 4, 5, 5, 5, 5, 5, 5, 6, 7, 8, 9, 10, 10};

	return gameTimeToTimecycTimeIndex[gameTime];
}