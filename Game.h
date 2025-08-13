#pragma once
#include "RenderSystem.h"
#include "ShaderSystem.h"
#include "InputSystem.h"

#include "SDL3/SDL.h"
#include "glad/glad.h"
#include "imgui.h"
#include "glm.hpp"

class Game
{
public:
	//
	// The usual constructor will create the window according to user's window
	//
	Game();
	Game(int32_t pWinWidth = 1280, int32_t pWinHeight = 720);
	~Game();

	void run();

private:
	int32_t mWinWidth{}, mWinHeight{};

	SDL_Window* mWindow{ nullptr };
	SDL_GLContext mContext{};

};

