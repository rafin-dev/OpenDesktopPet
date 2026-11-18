#include <ODPpch.h>
#include "System.h"

#include "UserEvents.h"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlgpu3.h"

namespace ODP {

	uint32_t System::s_UserEventsStart = 0;

	System::~System()
	{
		quit();
	}

	bool System::init()
	{
		if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO)) { quit(); return false; }
		
		s_UserEventsStart = SDL_RegisterEvents(UserEvent::EventCount);
		m_TrayIcon = new TrayIcon();
		if (!m_TrayIcon->init("OpenDesktopPet", nullptr)) { quit(); return false; }

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::StyleColorsDark();
		io.ConfigDpiScaleFonts = true;          
		io.ConfigDpiScaleViewports = true;      

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		m_ImGuiInitialized = true;

		return true;
	}

	void System::run()
	{
		while (!m_ShouldQuit)
		{
			if (m_Editor)
			{
				m_Editor->update();
			}

			SDL_Event event;
			while (SDL_PollEvent(&event)) 
			{
				if (m_Editor)
				{
					ImGui_ImplSDL3_ProcessEvent(&event);
				}

				if (event.type == SDL_EVENT_QUIT) 
				{
					m_ShouldQuit = true;
				}
				else if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
				{
					if (m_Editor)
					{
						delete m_Editor;
						m_Editor = nullptr;
					}
				}
				else if (event.type == s_UserEventsStart + UserEvent::Event_OpenEditor && !m_Editor)
				{
					m_Editor = new Editor();
					if (!m_Editor->init())
					{
						delete m_Editor;
						m_Editor = nullptr;
					}
				}
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