#include <ODPpch.h>
#include "System.h"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

namespace ODP {

	System::~System()
	{
		quit();
	}

	bool System::init()
	{
		if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO)) { quit(); return false; }
		
		m_TrayIcon = new TrayIcon();
		if (!m_TrayIcon->init("OpenDesktopPet", nullptr)) { quit(); return false; }

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::StyleColorsDark();
		m_ImGuiOn = true;

		return true;
	}

	void System::run()
	{
		SDL_Event event;
		while (SDL_WaitEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				break;
			}
		}
	}

	void System::quit()
	{
		if (m_PetManager) { delete m_PetManager; }
		if (m_Editor) { delete m_Editor; }
		if (m_TrayIcon) { delete m_TrayIcon; }
		
		// Quit ImGui last
		ImGui::DestroyContext();

		SDL_Quit();
	}

}