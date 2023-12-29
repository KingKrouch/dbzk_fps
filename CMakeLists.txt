cmake_minimum_required(VERSION 3.16)
# Set the Project Name.
set(PROJECT_NAME                dbzk_fps)
# Set the name of the target operating system.
set(CMAKE_SYSTEM_NAME           Windows)
if (UNIX) # If compiling on Linux, manually configure MingW-64.
	set(CMAKE_C_COMPILER        x86_64-w64-mingw32-gcc)
	set(CMAKE_CXX_COMPILER      x86_64-w64-mingw32-g++)
	# Set the Toolchain to MinGW on Linux.
	set(CMAKE_FIND_ROOT_PATH    /usr/x86_64-w64-mingw32)
	link_directories(${CMAKE_FIND_ROOT_PATH}/sys-root/mingw/lib) # TODO: Fix to also work with MinGW on Windows.
	# adjust the default behavior of the FIND_XXX() commands:
	# Never search for programs in the host environment
	set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
	# Search for headers and libraries only in the target environment
	set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
	set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endif (UNIX)
# Set our output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Binaries/${CMAKE_SYSTEM_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Binaries/${CMAKE_SYSTEM_NAME})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Binaries/${CMAKE_SYSTEM_NAME})
# Set our intermediate directories.
set(CMAKE_BINARY_DIR               ${CMAKE_CURRENT_SOURCE_DIR}/Intermediate/${CMAKE_BUILD_TYPE})
set(CMAKE_BUILD_FILES_DIRECTORY    ${CMAKE_CURRENT_SOURCE_DIR}/Intermediate/${CMAKE_BUILD_TYPE})
# Set our compile method to Ninja.
set(CMAKE_GENERATOR Ninja)
set(CMAKE_CXX_STANDARD 20)

# Set up our ThirdParty libraries
## IniPP
add_subdirectory(Source/ThirdParty/IniPP) # Use IniPP with it's own CMakeLists.txt

## ModUtils
add_library(ModUtils STATIC
		Source/ThirdParty/ModUtils/MemoryMgr.h
		Source/ThirdParty/ModUtils/ScopedUnprotect.hpp
		Source/ThirdParty/ModUtils/Patterns.cpp
		Source/ThirdParty/ModUtils/Patterns.h
		Source/ThirdParty/ModUtils/Trampoline.h)
set_target_properties(ModUtils PROPERTIES
		LINKER_LANGUAGE CXX
)

# Now finally export our library file.
## Setup the project and set a few variables
project(dbzk_fps)
add_library(dbzk_fps SHARED
		Source/dllmain.cpp
		Source/Managers/ConfigManager.cpp
		Source/Managers/ConfigManager.h
		Source/Managers/ConsoleManager.cpp
		Source/Managers/ConsoleManager.h
		Source/Managers/HookManager.cpp
		Source/Managers/HookManager.h
		Source/Managers/PatchManager.cpp
		Source/Managers/PatchManager.h
		Source/Settings/PlayerSettings.cpp
		Source/Settings/PlayerSettings.h
)
# Define the DLL_EXPORT macro when building the DLL.
target_compile_definitions(dbzk_fps PRIVATE DLL_EXPORT)
## Add the dependencies we set up previously.
add_dependencies(dbzk_fps inipp ModUtils)
target_link_libraries(dbzk_fps inipp ModUtils dinput8)
## Set up our linker flags for compiling.
set(CMAKE_SHARED_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static -municode")
## Set our output file as "dinput8.dll" as we are hooking that.
set_target_properties(dbzk_fps PROPERTIES PREFIX "" OUTPUT_NAME dbzk_fps SUFFIX ".asi")
## Now we delete any miscellaneous .a static library files for release builds, as we don't need them.
if(CMAKE_BUILD_TYPE STREQUAL "Release")
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E remove $<TARGET_FILE_DIR:${PROJECT_NAME}>/*.a)
endif()
