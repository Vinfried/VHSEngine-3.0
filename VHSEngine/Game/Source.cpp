#include "VHSEngine/Game.h"

int main( int argc, char** argv) {
	Game::GetGameInstance().Start("VHSEngine | An OpenGL Engine", false, 1280, 720);
	
	Game::DestroyGameInstance();

	return 0;
}