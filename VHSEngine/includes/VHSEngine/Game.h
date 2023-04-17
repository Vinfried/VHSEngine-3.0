#pragma once
#include "VHSEngine/Graphics/GraphicsEngine.h"
#include "CoreMinimal.h"

class Input;

class Game {
public:
	//Get game instance or create one if it doesn't exist
	static Game& GetGameInstance();

	//Destroy the game running the constructor
	static void DestroyGameInstance();

	//Start the game/app
	void Start(const char* WTitle, bool bFullscreen, int WWidth, int WHeight);
	
	//get precise delta time
	double GetDeltaTime() { return DeltaTime; }
	
	//get less precise delta time
	float GetFDeltaTime() { return static_cast<float>(DeltaTime); }

	//return the graphics engine
	GraphicsEnginePtr GetGraphicsEngine() { return Graphics; }

	//set the bIsGameOver to true
	void CloseApp() { bIsGameOver = true; }

	//return the graphics engine default texture
	TexturePtr GetDefaultEngineTexture();

	//return the graphics engine default material
	MaterialPtr GetDefaultEngineMaterial();

private:
	Game();
	~Game();

	//Run the game loop
	void Run();

	//Handle the game logic
	void Update();

	//Handle the Input/Output
	void ProcessInput();

	//Handle the 3D graphics drawing to the screen
	//Will clear and each frame
	void Draw();

	//Handle anything that needs to be deleted when the game ends
	void CloseGame();

private:
	//boolean that ends the game
	bool bIsGameOver;

	GraphicsEnginePtr Graphics;

	//the time between each frame
	double DeltaTime;

	//read the input of the player
	Input* GameInput;

	//temporary mesh variales
	ModelPtr Model2;
	ModelPtr Model;

	ModelPtr Wall;
	ModelPtr Wall2;

	

};