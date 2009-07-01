ADD_DEFINITIONS(-DHEADLESS -DNO_AVI -DUSING_CREG)
REMOVE_DEFINITIONS(-fPIC)



# Place executables and shared libs under "build-dir/",
# instead of under "build-dir/tools/HeadlessClient/"
# This way, we have the build-dir structure more like the install-dir one,
# which makes testing spring in the builddir easier, eg. like this:
# cd build-dir
# SPRING_DATADIR=$(pwd) ./spring
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../..")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

### Spring defines
SET(USE_NEDMALLOC FALSE CACHE BOOL "Use nedmalloc memory allocator")
if (USE_NEDMALLOC)
	ADD_DEFINITIONS(-DREPLACE_SYSTEM_ALLOCATOR)
	SET (nedmalloc_obj "../nedmalloc/nedmalloc.c")
endif (USE_NEDMALLOC)

SET(USE_MMGR FALSE CACHE BOOL "Use memory manager?")
if (USE_MMGR)
	ADD_DEFINITIONS(-DUSE_MMGR)
endif (USE_MMGR)

SET(TRACE_SYNC FALSE CACHE BOOL "Enable sync tracker")
if (TRACE_SYNC)
	ADD_DEFINITIONS(-DTRACE_SYNC)
endif (TRACE_SYNC)

SET(SYNCDEBUG FALSE CACHE BOOL "Enable sync debugger (needs SYNCCHECK=true)")
if (SYNCDEBUG)
	ADD_DEFINITIONS(-DSYNCDEBUG)
endif (SYNCDEBUG)

### Find include directories and add platform specific libraries
IF (MINGW)
	FIND_PACKAGE(Win32Libs REQUIRED)
	LIST(APPEND springHeadless_libraries ${WIN32_LIBRARIES} mingw32)
ENDIF (MINGW)

LIST(APPEND springHeadless_libraries ${SDL_LIBRARY} ${Boost_REGEX_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY})

### build all libraries in lib (has its own CMakeLists.txt)
#ADD_SUBDIRECTORY(../../rts/lib)
LIST(APPEND springHeadless_libraries lua 7zip hpiutil2 oscpack minizip streflop)



SET(sr ../../rts)

#AUX_SOURCE_DIRECTORY(${sr}/System/Net/ netfiles)
AUX_SOURCE_DIRECTORY(${sr}/ExternalAI/ ai_files)
INCLUDE_DIRECTORIES(${sr}/System/Net/ ${sr}/System/ ${sr}/lib/lua/include)
INCLUDE_DIRECTORIES(${sr}/ ${sr}/lib/7zip ${sr}/System)

SET(system_files
	${sr}/System/AutohostInterface.cpp
	${sr}/System/BaseNetProtocol.cpp
	${sr}/System/ConfigHandler.cpp
	${sr}/System/Demo.cpp
	${sr}/System/DemoReader.cpp
	${sr}/System/DemoRecorder.cpp
	${sr}/System/EventClient.cpp
	${sr}/System/EventHandler.cpp
	${sr}/System/float3.cpp
	${sr}/System/float4.cpp
	${sr}/System/GlobalUnsynced.cpp
	${sr}/System/Info.cpp
	${sr}/System/LoadInterface.cpp
	${sr}/System/LoadSaveHandler.cpp
	${sr}/System/LoadSaveInterface.cpp
	${sr}/System/LogOutput.cpp
#	${sr}/System/Main.cpp
	${sr}/System/Matrix44f.cpp
	${sr}/System/MemPool.cpp
	${sr}/System/Messages.cpp
	${sr}/System/mmgr.cpp
#	${sr}/System/MouseInput.cpp
	${sr}/System/myMath.cpp
	${sr}/System/NetProtocol.cpp
	${sr}/System/Object.cpp
	${sr}/System/Option.cpp
	${sr}/System/SaveInterface.cpp
	${sr}/System/SpringApp.cpp
	${sr}/System/StdAfx.cpp
	${sr}/System/TdfParser.cpp
	${sr}/System/TimeProfiler.cpp
	${sr}/System/TimeUtil.cpp
	${sr}/System/UnsyncedRNG.cpp
	${sr}/System/Util.cpp
	)
#AUX_SOURCE_DIRECTORY(${sr}/System system_files)
AUX_SOURCE_DIRECTORY(${sr}/System/creg system_files)
AUX_SOURCE_DIRECTORY(${sr}/System/Net/ system_files)
AUX_SOURCE_DIRECTORY(${sr}/System/FileSystem/ system_files)
AUX_SOURCE_DIRECTORY(${sr}/System/Sync/ system_files)

AUX_SOURCE_DIRECTORY(${sr}/Sim sim_files)
AUX_SOURCE_DIRECTORY(${sr}/Sim/Features sim_files)
AUX_SOURCE_DIRECTORY(${sr}/Sim/Misc sim_files)
AUX_SOURCE_DIRECTORY(${sr}/Sim/MoveTypes sim_files)
AUX_SOURCE_DIRECTORY(${sr}/Sim/MoveTypes/MoveMath sim_files)
AUX_SOURCE_DIRECTORY(${sr}/Sim/Objects sim_files)
AUX_SOURCE_DIRECTORY(${sr}/Sim/Path sim_files)
AUX_SOURCE_DIRECTORY(${sr}/Sim/Projectiles sim_files)
AUX_SOURCE_DIRECTORY(${sr}/Sim/Projectiles/WeaponProjectiles sim_files)
#AUX_SOURCE_DIRECTORY(${sr}/Sim/Projectiles/Unsynced sim_files)
#AUX_SOURCE_DIRECTORY(${sr}/Sim/Units sim_files)
SET(sim_files
	${sr}/Sim/Units/Unit.cpp
	${sr}/Sim/Units/UnitDefHandler.cpp
	${sr}/Sim/Units/UnitHandler.cpp
	${sr}/Sim/Units/UnitLoader.cpp
	${sr}/Sim/Units/UnitSet.cpp
#	${sr}/Sim/Units/UnitTracker.cpp
	}
AUX_SOURCE_DIRECTORY(${sr}/Sim/Units/COB sim_files)
AUX_SOURCE_DIRECTORY(${sr}/Sim/Units/CommandAI sim_files)
AUX_SOURCE_DIRECTORY(${sr}/Sim/Units/Groups sim_files)
AUX_SOURCE_DIRECTORY(${sr}/Sim/Units/UnitTypes sim_files)
AUX_SOURCE_DIRECTORY(${sr}/Sim/Weapons sim_files)

#AUX_SOURCE_DIRECTORY(${sr}/Game game_files)
SET(game_files
	${sr}/Game/Action.cpp
#	${sr}/Game/Camera.cpp
#	${sr}/Game/CameraHandler.cpp
	${sr}/Game/ChatMessage.cpp
	${sr}/Game/ClientSetup.cpp
	${sr}/Game/CommandMessage.cpp
	${sr}/Game/Console.cpp
	${sr}/Game/ConsoleHistory.cpp
	${sr}/Game/GameController.cpp
	${sr}/Game/Game.cpp
	${sr}/Game/GameData.cpp
	${sr}/Game/GameHelper.cpp
	${sr}/Game/GameServer.cpp
	${sr}/Game/GameSetup.cpp
	${sr}/Game/GameVersion.cpp
	${sr}/Game/OSCStatsSender.cpp
	${sr}/Game/PlayerBase.cpp
	${sr}/Game/Player.cpp
	${sr}/Game/PlayerHandler.cpp
	${sr}/Game/PlayerRoster.cpp
	${sr}/Game/PreGame.cpp
	${sr}/Game/SelectedUnitsAI.cpp
	${sr}/Game/SelectedUnits.cpp
#	${sr}/Game/SelectMenu.cpp
	${sr}/Game/WaitCommandsAI.cpp
	${sr}/Game/WordCompletion.cpp
	)

AUX_SOURCE_DIRECTORY(${sr}/Game/Server server_files)
#AUX_SOURCE_DIRECTORY(${sr}/Lua lua_files)
SET(lua_files
	${sr}/Lua/LuaBitOps.cpp
	${sr}/Lua/LuaCallInCheck.cpp
	${sr}/Lua/LuaConstCMD.cpp
	${sr}/Lua/LuaConstCMDTYPE.cpp
	${sr}/Lua/LuaConstGame.cpp
#	${sr}/Lua/LuaConstGL.cpp
#	${sr}/Lua/LuaFBOs.cpp
	${sr}/Lua/LuaFeatureDefs.cpp
#	${sr}/Lua/LuaFonts.cpp
	${sr}/Lua/LuaGaia.cpp
	${sr}/Lua/LuaHandle.cpp
	${sr}/Lua/LuaHandleSynced.cpp
	${sr}/Lua/LuaInputReceiver.cpp
	${sr}/Lua/LuaIO.cpp
#	${sr}/Lua/LuaMaterial.cpp
	${sr}/Lua/LuaOpenGL.cpp
	${sr}/Lua/LuaParser.cpp
	${sr}/Lua/LuaPathFinder.cpp
#	${sr}/Lua/LuaRBOs.cpp
	${sr}/Lua/LuaRules.cpp
	${sr}/Lua/LuaScream.cpp
#	${sr}/Lua/LuaShaders.cpp
	${sr}/Lua/LuaSyncedCall.cpp
	${sr}/Lua/LuaSyncedCtrl.cpp
	${sr}/Lua/LuaSyncedMoveCtrl.cpp
	${sr}/Lua/LuaSyncedRead.cpp
	${sr}/Lua/LuaSyncedTable.cpp
#	${sr}/Lua/LuaTextures.cpp
	${sr}/Lua/LuaUnitDefs.cpp
#	${sr}/Lua/LuaUnitRendering.cpp
	${sr}/Lua/LuaUnsyncedCall.cpp
	${sr}/Lua/LuaUnsyncedCtrl.cpp
	${sr}/Lua/LuaUnsyncedRead.cpp
	${sr}/Lua/LuaUtils.cpp
	${sr}/Lua/LuaVFS.cpp
	${sr}/Lua/LuaWeaponDefs.cpp
	)
SET(rendering_files
	${sr}/Rendering/InMapDraw.cpp
	${sr}/Rendering/UnitModels/3DModel.cpp
	${sr}/Rendering/UnitModels/3DOParser.cpp
	${sr}/Rendering/UnitModels/IModelParser.cpp
	${sr}/Rendering/UnitModels/s3oParser.cpp
#	${sr}/Rendering/UnitModels/UnitDrawer.cpp
	)

SET(springHeadless_files
	${system_files}
	${server_files}
	${ai_files}
	${sim_files}
	${game_files}
	${lua_files}
	${rendering_files}
	${sr}/Map/MapParser
	)

#ADD_LIBRARY(springmain SHARED EXCLUDE_FROM_ALL ${springHeadless_files})

ADD_EXECUTABLE(spring-headless EXCLUDE_FROM_ALL ${springHeadless_files} main)
#TARGET_LINK_LIBRARIES(spring-headless springmain)
TARGET_LINK_LIBRARIES(spring-headless ${Boost_REGEX_LIBRARY} ${Boost_THREAD_LIBRARY} ${springHeadless_libraries} lua)
if (MINGW)
	TARGET_LINK_LIBRARIES (spring-headless ws2_32)
else (MINGW)
	#set_target_properties(spring-headless PROPERTIES COMPILE_FLAGS "-fvisibility=default")
endif (MINGW)

#install (TARGETS springmain spring-headless RUNTIME DESTINATION ${BINDIR} LIBRARY DESTINATION ${LIBDIR})
