#include <ODPpch.h>

#include "System/System.h"

int main(int argc, const char* argv)
{
	auto* system = new ODP::System();
	if (system->init()) 
	{
		system->run();
	}
	delete system;

#if 0
	SDL_Log("Initializing\n");
	if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO)) 
	{
		SDL_Log("Failed to initialize SDL3\n");
		return -1;
	}

	SDL_Window* window = SDL_CreateWindow("Title", 400, 400, 
		SDL_WINDOW_UTILITY | SDL_WINDOW_BORDERLESS | SDL_WINDOW_TRANSPARENT | SDL_WINDOW_ALWAYS_ON_TOP);
	if (!window) {
		SDL_Log("Failed to create window");
		return -1;
	}

	
	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
	if (!renderer) {
		SDL_Log(SDL_GetError());
		return -1;
	}
	SDL_SetRenderLogicalPresentation(renderer, 400, 400, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);

	SDL_SetRenderDrawColorFloat(renderer, 1.0, 1.0, 1.0, 0.5);

	bool should_close = false;
	while (!should_close) 
	{
		SDL_RenderClear(renderer);

		SDL_RenderPresent(renderer);

		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT || 
				(event.type == SDL_EVENT_KEY_UP && event.key.scancode == SDL_SCANCODE_ESCAPE)) {
				should_close = true;
			}
		}
	}

	SDL_DestroyWindow(window);

	SDL_Quit();
#endif
}