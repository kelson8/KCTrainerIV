#include "ImGuiSetup.h"

// TODO Move ImGui setup into here once I figure out where to place mIsImGuiInitalized.
// Possibly move variable into here or make a getter/setter for it in the KCTrainerIV files.

#ifdef MOVE_IMGUI_SETUP

/// <summary>
/// Initialize and setup ImGui if not already initialized, otherwise do nothing.
/// </summary>
/// <param name="d3d9Device"></param>

void ImGuiSetup::InitializeImGui(IDirect3DDevice9* d3d9Device)
{
	if (!mIsImGuiInitialized)
	{
		D3DDEVICE_CREATION_PARAMETERS creationParams;
		d3d9Device->GetCreationParameters(&creationParams);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		SetupImGuiStyle(io);

		ImGui_ImplWin32_Init(creationParams.hFocusWindow);
		ImGui_ImplDX9_Init(d3d9Device);

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
		io.FontGlobalScale = mFontScale;

		mIsImGuiInitialized = true;
	}
}

#endif