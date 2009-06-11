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
AUX_SOURCE_DIRECTORY(${sr}/ExternalAI/ aifiles)
INCLUDE_DIRECTORIES(${sr}/System/Net/ ${sr}/System/ ${sr}/lib/lua/include)
INCLUDE_DIRECTORIES(${sr}/ ${sr}/lib/7zip ${sr}/System)

#AUX_SOURCE_DIRECTORY(${sr}/System/FileSystem/ fsfiles)
#SET(system_files
#	${fsfiles}
#	${netfiles}
#	${sr}/System/TdfParser
#	${sr}/System/ConfigHandler
#	${sr}/System/LogOutput
#	${sr}/System/TimeUtil
#	${sr}/System/BaseNetProtocol
#	${sr}/System/Demo
#	${sr}/System/DemoReader
#	${sr}/System/DemoRecorder
#	${sr}/System/AutohostInterface
#	${sr}/System/UnsyncedRNG)
AUX_SOURCE_DIRECTORY(${sr}/System system_files)
AUX_SOURCE_DIRECTORY(${sr}/System/creg system_files)
AUX_SOURCE_DIRECTORY(${sr}/System/Net/ system_files)
AUX_SOURCE_DIRECTORY(${sr}/System/FileSystem/ system_files)
AUX_SOURCE_DIRECTORY(${sr}/System/Sync/ system_files)

AUX_SOURCE_DIRECTORY(${sr}/Sim simfiles)
AUX_SOURCE_DIRECTORY(${sr}/Sim/Features simfiles)
AUX_SOURCE_DIRECTORY(${sr}/Sim/Misc simfiles)
AUX_SOURCE_DIRECTORY(${sr}/Sim/MoveTypes simfiles)
AUX_SOURCE_DIRECTORY(${sr}/Sim/MoveTypes/MoveMath simfiles)
AUX_SOURCE_DIRECTORY(${sr}/Sim/Objects simfiles)
AUX_SOURCE_DIRECTORY(${sr}/Sim/Path simfiles)
AUX_SOURCE_DIRECTORY(${sr}/Sim/Projectiles simfiles)
AUX_SOURCE_DIRECTORY(${sr}/Sim/Projectiles/WeaponProjectiles simfiles)
AUX_SOURCE_DIRECTORY(${sr}/Sim/Projectiles/Unsynced simfiles)
AUX_SOURCE_DIRECTORY(${sr}/Sim/Units simfiles)
AUX_SOURCE_DIRECTORY(${sr}/Sim/Units/COB simfiles)
AUX_SOURCE_DIRECTORY(${sr}/Sim/Units/CommandAI simfiles)
AUX_SOURCE_DIRECTORY(${sr}/Sim/Units/Groups simfiles)
AUX_SOURCE_DIRECTORY(${sr}/Sim/Units/UnitTypes simfiles)
AUX_SOURCE_DIRECTORY(${sr}/Sim/Weapons simfiles)

AUX_SOURCE_DIRECTORY(${sr}/Game gamefiles)
AUX_SOURCE_DIRECTORY(${sr}/Game/Server serverfiles)
AUX_SOURCE_DIRECTORY(${sr}/Game/Server luafiles)

SET(springHeadless_files
	${system_files}
	${serverfiles}
	${aifiles}
	${simfiles}
	${gamefiles}
	${luafiles}
	${sr}/Map/MapParser)

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
