#include "PlayerMenu.h"


// TODO Fix the linker errors when making this into a multi file project.

#ifdef MOVE_PLAYER_MENU

// TODO Move these into a pch.h file
// Other needed

// TODO Fix this to work, it gives a bunch of errors trying to use this file....



using namespace Scripting;

/// <summary>
/// Get the player coords as a Vector3
/// </summary>
/// <returns>A Vector3 of the player coords</returns>
Vector3 PlayerMenu::GetPlayerCoords()
{
	int playerPed;
	GET_PLAYER_CHAR(CONVERT_INT_TO_PLAYERINDEX(GET_PLAYER_ID()), &playerPed);

	int playerId = GET_PLAYER_ID();
	float posX, posY, posZ;

	GET_CHAR_COORDINATES(playerPed, &posX, &posY, &posZ);

	Vector3* playerPos = new Vector3{ posX, posY, posZ };

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
/// TODO Figure this out
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

int GetPlayerChar()
{
	int playerPed;
	GET_PLAYER_CHAR(CONVERT_INT_TO_PLAYERINDEX(GET_PLAYER_ID()), &playerPed);
	return playerPed;
}

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

bool godMode = false;

/// <summary>
/// Draw all the player menu functions
/// TODO Make this into KCNetIvImGui::DrawPlayerMenu or something
/// </summary>
void PlayerMenu::DrawPlayerMenu()
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
	Vector3 playerCoords = PlayerMenu::GetPlayerCoords();
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


#endif //MOVE_PLAYER_MENU