#include <ODPpch.h>
#include "TrayIcon.h"

namespace ODP {

	TrayIcon::~TrayIcon()
	{
		destroy();
	}

	void callback_quit(void* userdata, SDL_TrayEntry* invoker)
	{
		SDL_Event e;
		e.type = SDL_EVENT_QUIT;
		SDL_PushEvent(&e);
	}

	bool TrayIcon::init(const std::string& title, SDL_Surface* icon)
	{
		bool success = true;
		m_Tray = SDL_CreateTray(icon, title.c_str());
		if (!m_Tray) { destroy(); return false; }

		m_Menu = SDL_CreateTrayMenu(m_Tray);
		if (!m_Menu) { destroy(); return false; }

		m_QuitButton = SDL_InsertTrayEntryAt(m_Menu, -1, "Quit", SDL_TRAYENTRY_BUTTON);
		if (!m_QuitButton) { destroy(); return false; }
		SDL_SetTrayEntryCallback(m_QuitButton, callback_quit, nullptr);
		
		return true;
	}

	void TrayIcon::destroy()
	{
		if (m_Tray)
		{
			SDL_DestroyTray(m_Tray);
		}
	}

}