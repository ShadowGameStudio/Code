cmake_minimum_required (VERSION 3.6.2)
set(CRYENGINE_DIR "C:/Program Files (x86)/Crytek/CRYENGINE Launcher/Crytek/CRYENGINE_5.4")
set(TOOLS_CMAKE_DIR "${CRYENGINE_DIR}/Tools/CMake")

set(PROJECT_BUILD 1)
set(PROJECT_DIR "J:/BR")

include("${TOOLS_CMAKE_DIR}/CommonOptions.cmake")

add_subdirectory("${CRYENGINE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/CRYENGINE")

include("${TOOLS_CMAKE_DIR}/Configure.cmake")
start_sources()

sources_platform(ALL)
add_sources("Code_uber.cpp"
    PROJECTS Game
    SOURCE_GROUP "Root"
		"GamePlugin.cpp"
		"GamePlugin.h"
		"StdAfx.cpp"
		"StdAfx.h"
)
add_sources("Components_uber.cpp"
    PROJECTS Game
	    SOURCE_GROUP "Nodes"
		"Nodes/BaseFlowNodes.h"
		"Nodes/BaseFlowNodes.cpp"
	    SOURCE_GROUP "Components"
		"Components/SpawnPoint.cpp"
		"Components/SpawnPoint.h"
		SOURCE_GROUP "Components\\\\AI"
		"Components/AIComponent.cpp"
		"Components/AIComponent.h"
		"Components/AIUpdater.cpp"
		SOURCE_GROUP "Components\\\\AI\\\\Controller"
		"Components/AIControllerComponent.cpp"
		"Components/AIControllerComponent.h"
		"Components/AIViewController.cpp"
		SOURCE_GROUP "Components\\\\Items\\\\Supplies"
		"Components/DrugComponent.cpp"
		"Components/DrugComponent.h"
	    SOURCE_GROUP "Components\\\\Items\\\\Pickups"
		"Components/HealthPackComponent.h"
		"Components/HealthPackComponent.cpp"
		SOURCE_GROUP "Components\\\\Items\\\\Equipment"
		"Components/GasmaskComponent.cpp"
		"Components/GasmaskComponent.h"
	    SOURCE_GROUP "Components\\\\Entities\\\\Living"
		"Components/HealthComponent.h"
		"Components/HealthComponent.cpp"
		"Components/VitalStatusComponent.h"
		"Components/VitalStatusComponent.cpp"
	    SOURCE_GROUP "Components\\\\Player"
		"Components/Player.cpp"
		"Components/PlayerInput.cpp"
		"Components/PlayerUpdate.cpp"
		"Components/Player.h"
		SOURCE_GROUP "Components\\\\Player\\\\PlayerComponents"
		"Components/InventoryComponent.h"
		"Components/InventoryComponent.cpp"
		"Components/InventoryEvents.h"
	    SOURCE_GROUP "Components\\\\Items"
		"Components/ItemComponent.cpp"
		"Components/ItemComponent.h"
		"Components/ItemProperties.h"
	    SOURCE_GROUP "Components\\\\Items\\\\Weapons"
		"Components/WeaponComponent.cpp"
		"Components/WeaponComponent.h"
		"Components/Bullet.h"
	    SOURCE_GROUP "Components\\\\Gameplay"
		"Components/PlayAreaComponent.h"
		"Components/PlayAreaComponent.cpp"
		"Components/InfectedAreaComponent.h"
		"Components/InfectedAreaComponent.cpp"
		SOURCE_GROUP "Components\\\\Gameplay\\\\Loot"
		"Components/LootSpawnerComponent.h"
		"Components/LootSpawnerComponent.cpp"

)

end_sources()

CryEngineModule(Game PCH "StdAfx.cpp" SOLUTION_FOLDER "Project")

target_include_directories(${THIS_PROJECT}
PRIVATE 
    "${CRYENGINE_DIR}/Code/CryEngine/CryCommon"
    "${CRYENGINE_DIR}/Code/CryEngine/CryAction"
	"${CRYENGINE_DIR}/Code/CryEngine/CrySchematyc/Core/Interface"
	"${CRYENGINE_DIR}/Code/CryPlugins/CryDefaultEntities/Module"
)

# Set StartUp project in Visual Studio

add_library(GameLauncher STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
set_target_properties(GameLauncher PROPERTIES LINKER_LANGUAGE CXX)
if (WIN32)
    set_visual_studio_debugger_command(GameLauncher "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"J:/BR/Game.cryproject\"")
endif()

add_library(Sandbox STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
set_target_properties(Sandbox PROPERTIES LINKER_LANGUAGE CXX)
if (WIN32)
    set_visual_studio_debugger_command(Sandbox "${CRYENGINE_DIR}/bin/win_x64/Sandbox.exe" "-project \"J:/BR/Game.cryproject\"")
endif()

add_library(GameServer STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
set_target_properties(GameServer PROPERTIES LINKER_LANGUAGE CXX)
if (WIN32)
    set_visual_studio_debugger_command(GameServer "${CRYENGINE_DIR}/bin/win_x64/Game_Server.exe" "-project \"J:/BR/Game.cryproject\"")
endif()

set_solution_startup_target(GameLauncher)

if (WIN32)
    set_visual_studio_debugger_command( ${THIS_PROJECT} "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"J:/BR/Game.cryproject\"" )
endif()

#BEGIN-CUSTOM
# Make any custom changes here, modifications outside of the block will be discarded on regeneration.
#END-CUSTOM