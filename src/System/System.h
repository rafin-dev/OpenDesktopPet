#pragma once

#include <iostream>
#include <memory>

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_keyboard.h>

#include "Editor/Editor.h"
#include "PetMode/PetManager.h"
#include "TrayIcon.h"

namespace ODP {

	class System
	{
	public:
		System() = default;
		~System();

		bool init();
		void run();

		static constexpr uint32_t get_events_start() { return s_UserEventsStart; }

	private:
		void quit();

		PetManager* m_PetManager = nullptr;
		Editor* m_Editor = nullptr;
		TrayIcon* m_TrayIcon = nullptr;

		bool m_ImGuiInitialized = false;
		bool m_ShouldQuit = false;

		static uint32_t s_UserEventsStart;
	};

}