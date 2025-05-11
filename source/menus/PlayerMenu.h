#pragma once

//#define MOVE_PLAYER_MENU

#ifdef MOVE_PLAYER_MENU

#include <Windows.h>

#include "../Utils.h"

#include <vector>
#include <fstream>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <array>

// ImGui
#include <imgui.h>
//#include <imgui_internal.h>
//#include <backends\imgui_impl_win32.h>


// IVSDK
#include "IVSDK.h"
//#include "injector/injector.hpp"
//
//#include "Addresses.h"
#include "Scripting/Scripting.h"
//#include "Hooks.h"




class PlayerMenu
{
public:
	static Scripting::Vector3 GetPlayerCoords();
	static void DrawPlayerMenu();
};

#endif // MOVE_PLAYER_MENU