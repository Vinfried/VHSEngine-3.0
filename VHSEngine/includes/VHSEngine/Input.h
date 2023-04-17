#pragma once

#include "SDL2/SDL.h"

enum MouseButtons {
	LEFT = SDL_BUTTON_LEFT,
	MIDDLE = SDL_BUTTON_MIDDLE,
	RIGHT = SDL_BUTTON_RIGHT
};


class Input {
public:
	
	Input();
	
	//detect the input of the user
	void ProcessInput();

	//detect if a specific key is down on an sdl scancode
	//Hint * SDL_SCANCODE_KEY (SDL_SCANCODE_W)
	bool IsKeyDown(SDL_Scancode Key);
	
	//detect if the mouse button inputed is being pressed down
	bool IsMouseButtonDown(MouseButtons Button);

	//change the visibility of the cursor
	void ShowCursor(bool bVisibility);

private:

	//update the mouse button inputed with the current steate of the mouse
	void SetMouseButtonState(Uint8 ButtonIndex, bool CurrentState);

	//handle the variables that rely on mouse motion
	void OnMouseMove(SDL_MouseMotionEvent& MEvent);

	void OnMouseScroll(SDL_MouseWheelEvent& MEvent);

public:
	//Mouse positions
	int MouseX, MouseY;

	//How far has the mouse has moved since last checked
	int MouseXDelta, MouseYDelta;

	//how far the player has scrolled since last frame
	float ScrollDelta;


private:
	//keyboard current states
	const Uint8* KeyBoardState;

	//mouse button current states
	bool MouseButtonStates[10] = { false };
};
