#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>

namespace ODP {

	class Editor 
	{
	public:
		Editor() = default;
		~Editor();

		bool init();

	private:
		SDL_Window* m_Window = nullptr;
		SDL_Renderer* SDL_Renderer = nullptr;
	};

}