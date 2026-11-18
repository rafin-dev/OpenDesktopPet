#include <ODPpch.h>
#include "TrayIcon.h"

#include "UserEvents.h"
#include "System.h"

namespace ODP {

	TrayIcon::~TrayIcon()
	{
		destroy();
	}

	static void send_event(uint32_t eventType)
	{
		SDL_Event e;
		e.type = eventType;
		SDL_PushEvent(&e);
	}

	bool TrayIcon::init(const std::string& title, SDL_Surface* icon)
	{
		bool success = true;
		m_Tray = SDL_CreateTray(icon, title.c_str());
		if (!m_Tray) { destroy(); return false; }

		m_Menu = SDL_CreateTrayMenu(m_Tray);
		if (!m_Menu) { destroy(); return false; }

		m_OpenEditor = SDL_InsertTrayEntryAt(m_Menu, -1, "Open Editor", SDL_TRAYENTRY_BUTTON);
		if (!m_OpenEditor) { destroy(); return false; }
		SDL_SetTrayEntryCallback(m_OpenEditor, 
			[](void*, SDL_TrayEntry*) { send_event(System::get_events_start() + UserEvent::Event_OpenEditor); }, nullptr);

		m_QuitButton = SDL_InsertTrayEntryAt(m_Menu, -1, "Quit", SDL_TRAYENTRY_BUTTON);
		if (!m_QuitButton) { destroy(); return false; }
		SDL_SetTrayEntryCallback(m_QuitButton, [](void*, SDL_TrayEntry*) { send_event(SDL_EVENT_QUIT); }, nullptr);
		
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