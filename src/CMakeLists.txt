cmake_minimum_required(VERSION 3.5)
project(OpenDesktopPet)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(vendor/SDL/SDL-release-3.4.20 EXCLUDE_FROM_ALL)

set(IMGUI_DIR "vendor/imgui/imgui")

include_directories(./ ${IMGUI_DIR} ${IMGUI_DIR}/backends)

add_executable(OpenDesktopPet
	"main.cpp"
	"ODPpch.h"
	"ODPpch.cpp"
	"Editor/Editor.h" 
	"Editor/Editor.cpp"
	"PetMode/PetManager.h" 
	"System/System.h" 
	"System/System.cpp"
	"System/TrayIcon.h"
	"System/TrayIcon.cpp"

	# ImGui
	${IMGUI_DIR}/imgui.cpp
	${IMGUI_DIR}/imgui_demo.cpp
	${IMGUI_DIR}/imgui_draw.cpp
	${IMGUI_DIR}/imgui_tables.cpp
	${IMGUI_DIR}/imgui_widgets.cpp
	${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
	${IMGUI_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)

set_property(TARGET OpenDesktopPet PROPERTY CXX_STANDARD 23)
target_link_libraries(OpenDesktopPet PRIVATE SDL3::SDL3)

target_precompile_headers(OpenDesktopPet PUBLIC ODPpch.h)