cmake_minimum_required(VERSION 3.5...4.0)

project(pvz-portable LANGUAGES C CXX ASM)

option(BUILD_STATIC "Static link for a standalone exe (Windows, non-MSVC)" OFF)
if(BUILD_STATIC)
	if(WIN32 AND NOT MSVC)
		set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
		set(BUILD_SHARED_LIBS OFF)
		add_link_options(-static)
	elseif(MSVC)
		message(WARNING "BUILD_STATIC is not supported for MSVC. Use a vcpkg -static triplet instead.")
		set(BUILD_STATIC OFF)
	else()
		message(WARNING "BUILD_STATIC is only supported on Windows with MinGW-based toolchains.")
		set(BUILD_STATIC OFF)
	endif()
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if (CMAKE_GENERATOR MATCHES "Ninja")
	set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
endif()

if(MSVC)
	if(DEFINED VCPKG_TARGET_TRIPLET AND VCPKG_TARGET_TRIPLET MATCHES "-static")
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	elseif(DEFINED VCPKG_TARGET_TRIPLET)
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL$<$<CONFIG:Debug>:Debug>")
	endif()
endif()

file(GLOB SOURCES
	src/*.cpp
	src/SexyAppFramework/*.cpp
	src/SexyAppFramework/graphics/*.cpp
	src/SexyAppFramework/imagelib/*.cpp
	src/SexyAppFramework/misc/*.cpp
	src/SexyAppFramework/paklib/*.cpp
	src/SexyAppFramework/sound/*.cpp
	src/SexyAppFramework/widget/*.cpp
	src/SexyAppFramework/fcaseopen/*.c
	src/Sexy.TodLib/*.cpp
	src/Lawn/*.cpp
	src/Lawn/Widget/*.cpp
	src/Lawn/System/*.cpp
)

# these files are included and must not be built in the project
list(REMOVE_ITEM SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/Sexy.TodLib/TodDrawTriangle.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/Sexy.TodLib/TodDrawTriangleInc.cpp
)

set(LOW_MEMORY OFF CACHE BOOL "Enable low memory mode")
set(DOWNSCALE_COUNT 1 CACHE STRING "Image downscale count")
if(NINTENDO_SWITCH)
	find_library(OGG_LIBRARY NAMES ogg libogg REQUIRED)
	find_library(VORBIS_LIBRARY NAMES vorbis libvorbis REQUIRED)
	find_library(VORBISFILE_LIBRARY NAMES vorbisfile libvorbisfile REQUIRED)
	find_library(OPENMPT_LIBRARY NAMES openmpt libopenmpt REQUIRED)
	find_library(MPG123_LIBRARY NAMES mpg123 libmpg123 REQUIRED)

	set(VORBIS_LIB ${VORBISFILE_LIBRARY} ${VORBIS_LIBRARY})
	set(OGG_LIB ${OGG_LIBRARY})
	set(OPENMPT_LIB ${OPENMPT_LIBRARY})
	set(MPG123_LIB ${MPG123_LIBRARY})

	list(APPEND SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/src/SexyAppFramework/platform/switch/Window.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/SexyAppFramework/platform/switch/Input.cpp
	)
	list(APPEND PLAT_INCLUDES
		${CMAKE_CURRENT_SOURCE_DIR}/src/SexyAppFramework/platform/switch
	)
elseif(NINTENDO_3DS)
	set(LOW_MEMORY ON)
	set(DOWNSCALE_COUNT 2)
	ctr_add_shader_library(colored ${CMAKE_CURRENT_SOURCE_DIR}/src/SexyAppFramework/platform/3ds/graphics/colored.v.pica)
	ctr_add_shader_library(textured ${CMAKE_CURRENT_SOURCE_DIR}/src/SexyAppFramework/platform/3ds/graphics/textured.v.pica)
	dkp_add_embedded_binary_library(shaders colored textured)
	list(APPEND SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/src/SexyAppFramework/platform/3ds/graphics/Window.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/SexyAppFramework/platform/3ds/graphics/GLInterface.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/SexyAppFramework/platform/3ds/Input.cpp
	)
	list(APPEND PLAT_INCLUDES
		${CMAKE_CURRENT_SOURCE_DIR}/src/SexyAppFramework/platform/3ds
	)
elseif(EMSCRIPTEN)
	# libopenmpt has no Emscripten port; must be pre-built with emmake.
	find_library(OPENMPT_LIBRARY NAMES openmpt libopenmpt REQUIRED)
	set(OPENMPT_LIB ${OPENMPT_LIBRARY})

	list(APPEND SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/src/SexyAppFramework/platform/emscripten/Window.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/SexyAppFramework/platform/default/Input.cpp
	)
	list(APPEND PLAT_INCLUDES
		${CMAKE_CURRENT_SOURCE_DIR}/src/SexyAppFramework/platform/emscripten
	)
else()
	if(VCPKG_TOOLCHAIN)
		find_package(Vorbis CONFIG REQUIRED)
		find_package(Ogg CONFIG REQUIRED)
		find_package(libopenmpt CONFIG REQUIRED)
		find_package(mpg123 CONFIG REQUIRED)
		set(VORBIS_LIB Vorbis::vorbis Vorbis::vorbisfile)
		set(OGG_LIB Ogg::ogg)
		set(OPENMPT_LIB libopenmpt::libopenmpt)
		set(MPG123_LIB MPG123::libmpg123)
	else()
		find_library(OGG_LIBRARY NAMES ogg libogg REQUIRED)
		find_library(VORBIS_LIBRARY NAMES vorbis libvorbis REQUIRED)
		find_library(VORBISFILE_LIBRARY NAMES vorbisfile libvorbisfile REQUIRED)
		find_library(OPENMPT_LIBRARY NAMES openmpt libopenmpt REQUIRED)
		find_library(MPG123_LIBRARY NAMES mpg123 libmpg123 REQUIRED)

		set(VORBIS_LIB ${VORBISFILE_LIBRARY} ${VORBIS_LIBRARY})
		set(OGG_LIB ${OGG_LIBRARY})
		set(OPENMPT_LIB ${OPENMPT_LIBRARY})
		set(MPG123_LIB ${MPG123_LIBRARY})
	endif()
	list(APPEND SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/src/SexyAppFramework/platform/default/Window.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/SexyAppFramework/platform/default/Input.cpp
	)
	list(APPEND PLAT_INCLUDES
		${CMAKE_CURRENT_SOURCE_DIR}/src/SexyAppFramework/platform/default
	)
endif()

if(CMAKE_BUILD_TYPE AND CMAKE_BUILD_TYPE MATCHES "^[Dd][Ee][Bb][Uu][Gg]$")
	set(WIN_CONSOLE_DEFAULT ON)
	set(PVZ_DEBUG_DEFAULT ON)
else()
	set(WIN_CONSOLE_DEFAULT OFF)
	set(PVZ_DEBUG_DEFAULT OFF)
endif()

option(PVZ_DEBUG "Enable _PVZ_DEBUG macro to enable cheats, hidden features, etc." ${PVZ_DEBUG_DEFAULT})
option(LIMBO_PAGE "Enable limbo page to access hidden levels" ON)
option(CONSOLE "Show console on Windows" ${WIN_CONSOLE_DEFAULT})
option(DO_FIX_BUGS "Define DO_FIX_BUGS macro (Community fixes for original game bugs of 1.2.0.1073 GOTY Edition)" OFF)

if(EMSCRIPTEN)
	list(PREPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -sUSE_SDL=2 -sUSE_ZLIB=1 -sUSE_LIBPNG=1 -sUSE_LIBJPEG=1 -sUSE_OGG=1 -sUSE_VORBIS=1 -sUSE_MPG123=1")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sUSE_SDL=2 -sUSE_ZLIB=1 -sUSE_LIBPNG=1 -sUSE_LIBJPEG=1 -sUSE_OGG=1 -sUSE_VORBIS=1 -sUSE_MPG123=1")
else()
	find_package(Threads REQUIRED)
	find_package(ZLIB REQUIRED)
	find_package(JPEG REQUIRED)
	find_package(PNG REQUIRED)
	find_package(SDL2 REQUIRED)
endif()

function(_pvz_patch_sdl2_iface_libs target)
	get_target_property(_libs ${target} INTERFACE_LINK_LIBRARIES)
	if(NOT _libs)
		return()
	endif()
	set(_patched)
	foreach(_lib IN LISTS _libs)
		if(_lib STREQUAL "SDL2::SDL2")
			list(APPEND _patched SDL2::SDL2-static)
		else()
			list(APPEND _patched "${_lib}")
		endif()
	endforeach()
	set_target_properties(${target} PROPERTIES INTERFACE_LINK_LIBRARIES "${_patched}")
endfunction()

if(BUILD_STATIC AND TARGET SDL2::SDL2-static)
	if(TARGET SDL2::SDL2main)
		_pvz_patch_sdl2_iface_libs(SDL2::SDL2main)
	endif()
	set(SDL2_LINK_TARGET SDL2::SDL2-static)
else()
	set(SDL2_LINK_TARGET SDL2::SDL2)
endif()

set(SDL_MIXER_X_STATIC ON CACHE BOOL "Build static library of SDL Mixer X")
set(SDL_MIXER_X_SHARED OFF CACHE BOOL "Build shared library of SDL Mixer X")
set(USE_MIDI OFF CACHE BOOL "SDL Mixer X midi")
set(USE_MODPLUG OFF CACHE BOOL "SDL Mixer X modplug")
set(USE_XMP OFF CACHE BOOL "SDL Mixer X xmp")
set(USE_OPUS OFF CACHE BOOL "SDL Mixer X opus")
set(USE_DRFLAC OFF CACHE BOOL "SDL Mixer X flac")
set(USE_PXTONE OFF CACHE BOOL "SDL Mixer X pxtone")
add_subdirectory(src/SexyAppFramework/sound/SDL-Mixer-X)

# Patch SDL-Mixer-X's transitive SDL2 dep without touching vendored CMake files
if(BUILD_STATIC AND TARGET SDL2::SDL2-static)
	_pvz_patch_sdl2_iface_libs(SDL2_mixer_ext_Static)
endif()

# Android requires a shared library loaded by SDLActivity; other platforms use an executable.
if(ANDROID)
	add_library(pvz-portable SHARED ${SOURCES})
elseif(IOS)
	add_executable(pvz-portable MACOSX_BUNDLE ${SOURCES})
else()
	add_executable(pvz-portable ${SOURCES})
endif()

target_include_directories(pvz-portable PRIVATE
	${PLAT_INCLUDES}
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/src/SexyAppFramework
	${PROJECT_SOURCE_DIR}/src/SexyAppFramework/sound/SDL-Mixer-X/include
	${SDL2_INCLUDE_DIRS}
)
target_compile_definitions(pvz-portable PRIVATE
	$<$<BOOL:${PVZ_DEBUG}>:_PVZ_DEBUG>
	$<$<BOOL:${LIMBO_PAGE}>:_PVZ_LIMBO_PAGE>
	$<$<BOOL:${DO_FIX_BUGS}>:DO_FIX_BUGS>
)
target_compile_features(pvz-portable PRIVATE cxx_std_20)

if (LOW_MEMORY)
	target_compile_definitions(pvz-portable PRIVATE LOW_MEMORY)
endif()
target_compile_definitions(pvz-portable PRIVATE IMG_DOWNSCALE=${DOWNSCALE_COUNT})

if(EMSCRIPTEN)
	set_property(TARGET pvz-portable APPEND PROPERTY LINK_DEPENDS
		"${CMAKE_CURRENT_SOURCE_DIR}/wasm/shell.html"
	)
	target_link_libraries(pvz-portable PRIVATE
		SDL2_mixer_ext_Static
		${OPENMPT_LIB}
	)
	target_link_options(pvz-portable PRIVATE
		"SHELL:-sUSE_SDL=2"
		"SHELL:-sUSE_ZLIB=1"
		"SHELL:-sUSE_LIBPNG=1"
		"SHELL:-sUSE_LIBJPEG=1"
		"SHELL:-sUSE_OGG=1"
		"SHELL:-sUSE_VORBIS=1"
		"SHELL:-sUSE_MPG123=1"
		"SHELL:-sASYNCIFY"
		"SHELL:-sALLOW_MEMORY_GROWTH=1"
		"SHELL:-sMAXIMUM_MEMORY=1073741824"
		"SHELL:-sFORCE_FILESYSTEM=1"
		"SHELL:-sEXPORTED_RUNTIME_METHODS=[\"FS\",\"callMain\"]"
		"SHELL:-sINVOKE_RUN=0"
		"SHELL:-lidbfs.js"
		"SHELL:--shell-file ${CMAKE_CURRENT_SOURCE_DIR}/wasm/shell.html"
	)

	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
		target_link_options(pvz-portable PRIVATE "SHELL:--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/assets@/")
	endif()

	set_target_properties(pvz-portable PROPERTIES SUFFIX ".html")
else()
	target_link_libraries(pvz-portable PRIVATE
		SDL2_mixer_ext_Static 
		${OPENMPT_LIB} 
		${MPG123_LIB} 
		${VORBIS_LIB} 
		${OGG_LIB} 
		PNG::PNG
		JPEG::JPEG
		ZLIB::ZLIB
		SDL2::SDL2main
		${SDL2_LINK_TARGET}
		Threads::Threads
	)
endif()

if (WIN32)
	if(MSVC)
		target_compile_options(pvz-portable PRIVATE /utf-8)
	endif()

	target_sources(pvz-portable PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/SexyAppFramework/LawnProject.rc)

	set_source_files_properties(
		${CMAKE_CURRENT_SOURCE_DIR}/src/SexyAppFramework/LawnProject.rc
		PROPERTIES
			COMPILE_DEFINITIONS "APP_ICON_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/icon.ico\";EMBED_MAIN_PAK=1"
			OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/icon.ico;${CMAKE_CURRENT_SOURCE_DIR}/assets/main.pak"
	)

	target_link_libraries(pvz-portable PRIVATE ws2_32 user32 gdi32 winmm imm32 shlwapi)
	target_compile_definitions(pvz-portable PRIVATE WINDOWS)

	if(NOT CONSOLE)
		if(MSVC)
			set_target_properties(pvz-portable PROPERTIES WIN32_EXECUTABLE TRUE)
		else()
			target_link_options(pvz-portable PRIVATE -mwindows)
		endif()
	endif()
elseif (NINTENDO_SWITCH)
	target_link_libraries(pvz-portable PRIVATE GLESv2 EGL glapi drm_nouveau)
	target_compile_options(pvz-portable PRIVATE -g -O0) # Keep debug info and optimize
	target_link_options(pvz-portable PRIVATE -g)

	nx_generate_nacp(pvz-portable.nacp
		NAME "PvZ Portable"
		AUTHOR "wszqkzqk, etc., a community-driven port"
		VERSION "0.1"
	)

	nx_create_nro(pvz-portable
		NACP pvz-portable.nacp
		ICON ${CMAKE_CURRENT_SOURCE_DIR}/icon-switch.jpg
		ROMFS ${CMAKE_CURRENT_SOURCE_DIR}/assets
	)
elseif (NINTENDO_3DS)
	target_link_libraries(pvz-portable PRIVATE citro3d shaders)

	ctr_generate_smdh(pvz-portable.smdh
		NAME "PvZ Portable"
		DESCRIPTION "A community-driven re-implementation of Plants vs. Zombies GOTY Edition"
		AUTHOR "wszqkzqk, etc., a community-driven port"
		VERSION "0.1"
		ICON "${CMAKE_CURRENT_SOURCE_DIR}/icon-3ds.png"
	)

	ctr_create_3dsx(pvz-portable
		SMDH pvz-portable.smdh
		ROMFS ${CMAKE_CURRENT_SOURCE_DIR}/assets
	)
elseif (ANDROID)
	target_link_libraries(pvz-portable PRIVATE GLESv2 EGL android log)
	set_target_properties(pvz-portable PROPERTIES OUTPUT_NAME "main")

	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
		add_custom_command(TARGET pvz-portable POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/android/app/src/main/assets"
			COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${CMAKE_CURRENT_SOURCE_DIR}/android/app/src/main/assets"
			COMMENT "Syncing assets to Android project"
		)
	endif()
elseif (IOS)
	target_link_libraries(pvz-portable PRIVATE
		"-framework Foundation"
		"-framework UIKit"
		"-framework OpenGLES"
		"-framework QuartzCore"
		"-framework CoreGraphics"
		"-framework CoreMotion"
		"-framework GameController"
		"-framework AVFoundation"
		"-framework AudioToolbox"
		"-framework Metal"
	)

	set_target_properties(pvz-portable PROPERTIES
		MACOSX_BUNDLE TRUE
		MACOSX_BUNDLE_GUI_IDENTIFIER "io.github.wszqkzqk.pvz-portable"
		MACOSX_BUNDLE_BUNDLE_NAME "PvZ Portable"
		MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
		MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
		MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/ios/Info.plist.in"
		XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"
		XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "15.0"
		XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "io.github.wszqkzqk.pvz-portable"
		XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "-"
		XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon"
	)

	# Add launch storyboard to the bundle
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/ios/LaunchScreen.storyboard")
		target_sources(pvz-portable PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/ios/LaunchScreen.storyboard")
		set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/ios/LaunchScreen.storyboard"
			PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
	endif()

	# Add asset catalog (app icon)
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/ios/Assets.xcassets")
		target_sources(pvz-portable PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/ios/Assets.xcassets")
		set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/ios/Assets.xcassets"
			PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
	endif()

	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
		file(GLOB_RECURSE IOS_ASSETS "${CMAKE_CURRENT_SOURCE_DIR}/assets/*")
		target_sources(pvz-portable PRIVATE ${IOS_ASSETS})
		set_source_files_properties(${IOS_ASSETS} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
	endif()
endif()

# Asset Embedding (Linux/macOS)
if(UNIX AND NOT ANDROID AND NOT IOS AND NOT APPLE) # APPLE bundle case handled above
    target_sources(pvz-portable PRIVATE src/SexyAppFramework/paklib/PakEmbed.S)
    set_source_files_properties(src/SexyAppFramework/paklib/PakEmbed.S PROPERTIES 
        INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/assets")
endif()
# For macOS non-bundle (binary only)
if(APPLE)
     target_sources(pvz-portable PRIVATE src/SexyAppFramework/paklib/PakEmbed.S)
     set_source_files_properties(src/SexyAppFramework/paklib/PakEmbed.S PROPERTIES 
        INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/assets")
endif()

# Asset Automation (Python discovery)
find_package(Python3 COMPONENTS Interpreter QUIET)
if(NOT Python3_Interpreter_FOUND)
	find_program(Python3_EXECUTABLE NAMES python3 python python.exe)
	if(Python3_EXECUTABLE)
		set(Python3_Interpreter_FOUND TRUE)
	endif()
endif()

# Properties Embedding (Cross-platform)
set(PROPERTIES_EMBED_CPP "${CMAKE_CURRENT_BINARY_DIR}/properties_embed.cpp")
if(Python3_Interpreter_FOUND)
	add_custom_command(
		OUTPUT "${PROPERTIES_EMBED_CPP}"
		COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/scripts/embed_folder.py" 
				"${CMAKE_CURRENT_SOURCE_DIR}/assets/properties" "${PROPERTIES_EMBED_CPP}"
		DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/scripts/embed_folder.py"
		COMMENT "Embedding properties folder..."
	)
else()
	# Fallback: create an empty loader if Python is missing
	file(WRITE "${PROPERTIES_EMBED_CPP}" "#include \"PakInterface.h\"\nvoid LoadEmbeddedProperties(PakInterface* p) {}\n")
endif()
target_sources(pvz-portable PRIVATE "${PROPERTIES_EMBED_CPP}")
target_include_directories(pvz-portable PRIVATE src/SexyAppFramework/paklib)

# Asset Automation (Download)
# If 'main.pak' is missing from 'assets', download it from Google Drive.
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets/main.pak")
	if(Python3_Interpreter_FOUND)
		message(STATUS "Asset automation: main.pak not found. Attempting download from Google Drive using ${Python3_EXECUTABLE}...")
		execute_process(
			COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/scripts/download_main_pak.py" "${CMAKE_CURRENT_SOURCE_DIR}/assets/main.pak"
			RESULT_VARIABLE DOWNLOAD_RESULT
			OUTPUT_VARIABLE DOWNLOAD_OUTPUT
			ERROR_VARIABLE DOWNLOAD_ERROR
		)
		if(DOWNLOAD_OUTPUT)
			message(STATUS "Download output: ${DOWNLOAD_OUTPUT}")
		endif()
		if(DOWNLOAD_ERROR)
			message(STATUS "Download error: ${DOWNLOAD_ERROR}")
		endif()
		if(NOT DOWNLOAD_RESULT EQUAL 0)
			message(WARNING "Asset automation: Failed to download main.pak automatically. Exit code: ${DOWNLOAD_RESULT}")
		endif()
	else()
		message(WARNING "Asset automation: Python3 not found. Cannot download main.pak automatically.")
	endif()
endif()

# Asset Automation (Desktop/Consoles)
# If an 'assets' folder exists in the project root, copy its contents to the build directory.
# Android, iOS, and WASM have their own specific bundling logic above.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets" AND NOT ANDROID AND NOT IOS AND NOT EMSCRIPTEN)
	message(STATUS "Asset automation: 'assets' folder found. Implementing post-build deployment.")
	add_custom_command(TARGET pvz-portable POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			"${CMAKE_CURRENT_SOURCE_DIR}/assets"
			"$<TARGET_FILE_DIR:pvz-portable>"
		COMMENT "Copying assets from source root to build directory"
	)
endif()
