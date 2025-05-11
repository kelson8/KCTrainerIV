#pragma once

#include "../Utils.h"

#include "imgui.h"


class ImGuiSetup 
{
public:
#ifdef MOVE_IMGUI_SETUP
	void InitializeImGui(IDirect3DDevice9* d3d9Device);
#endif
};