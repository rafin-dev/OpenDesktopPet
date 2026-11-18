#pragma once
#include <vector>
#include <string>

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>

namespace ODP {

	class TrayIcon 
	{
	public:
		TrayIcon() = default;
		~TrayIcon();

		bool init(const std::string& title, SDL_Surface* icon);

	private:
		void destroy();

		SDL_Tray* m_Tray = nullptr;
		SDL_TrayMenu* m_Menu = nullptr;
		SDL_TrayEntry* m_OpenEditor = nullptr;
		SDL_TrayEntry* m_QuitButton = nullptr;
	};

}