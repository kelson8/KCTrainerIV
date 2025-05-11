# KCNet IV ImGui Mod Menu
Custom mod menu using InGameTimeCycEditor as a base. I will be adding plently of features to this, I don't see very many open source GTA 4 mods, or even mod menus.

This is in early development and certain features may crash your game, I don't have a project release for the asi file just yet but I will eventually add one once I add more features to this.

I have setup a console that attaches to GTA IV for logging values to it using cout in C++, if you don't understand what that means, it just means I can display text in a console window from the trainer.

Future items for the project (I may add more to this list later.):
* Config ini or some type of config.
* Save/Load trainer settings
* Vehicle spawner
* More vehicle options
* Ped spawner
* Teleport list using a `KCTrainerTeleports.ini` file or something like that

# Screenshots
<details>
<summary> Player Menu </summary>
<img src=screenshots/PlayerMenu.png?raw=true>
</details>

<details>
<summary> Vehicle Menu </summary>
<img src=screenshots/VehicleMenu.png?raw=true>
</details>

<details>
<summary> Teleport Menu </summary>
<img src=screenshots/TeleportMenu.png?raw=true>
</details>

<details>
<summary> About Menu </summary>
<img src=screenshots/AboutMenu.png?raw=true>
</details>



# About
This project will become a new open source mod menu for GTA 4 once I get it all working.

# Project Setup and Debugging
This is mostly for other developers, if you want to just use this menu just ignore this.

#### To debug this easier, first setup the debugging tab in Visual Studio 2022:
1. Set Working Directory to your GTA 4 directory with steam, example: `C:\Program Files (x86)\Steam\steamapps\common\Grand Theft Auto IV\GTAIV\`
2. Set Command the GTAIV.exe, example: `C:\Program Files (x86)\Steam\steamapps\common\Grand Theft Auto IV\GTAIV\GTAIV.exe`
3. Optional, to run the game in windowed mode, set this in Command Arguments: -windowed

#### To copy this to the GTA IV Directory:
* Setup Build Events -> Post-Build Event, set the command line to this (Set the directory to copy to as your GTA IV directory)

``` 
:xcopy /y /d "$(SolutionDir)build\$(Configuration)\bin\KCTrainerIV.*" "C:\Program Files (x86)\Steam\steamapps\common\Grand Theft Auto IV\GTAIV\plugins"
```

Optional: Set description to Post-Build Event to whatever you want, I name mine this: `Copy dll to GTA IV plugins directory`


# Credit
Credits to akifle47 on GitHub for the original InGameTimeCycEditor mod: https://github.com/akifle47/InGameTimecycEditor/

# License
This project is licensed under the MIT license.

