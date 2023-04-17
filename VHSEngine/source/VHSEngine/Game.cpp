#include "VHSEngine/Game.h"
#include "VHSEngine/Graphics/GraphicsEngine.h"
#include "VHSEngine/Graphics/Model.h"
#include "VHSEngine/Input.h"
#include "VHSEngine/Graphics/Camera.h"
#include "VHSEngine/Graphics/Material.h"

Game& Game::GetGameInstance()
{
	static Game* GameInstance = new Game();
	return *GameInstance;
}

void Game::DestroyGameInstance()
{
	static Game* GameInstance = &GetGameInstance();
	delete GameInstance;
}

void Game::Start(const char* WTitle, bool bFullscreen, int WWidth, int WHeight)
{
	Graphics = make_shared<GraphicsEngine>();

	if (Graphics->InitGE(WTitle, bFullscreen, WWidth, WHeight)) {
		bIsGameOver = false;
	}

	Run();
}

TexturePtr Game::GetDefaultEngineTexture()
{
	return Graphics->DefaultEngineTexture;
}

MaterialPtr Game::GetDefaultEngineMaterial()
{
	return Graphics->DefaultEngineMaterial;
}

Game::Game()
{
	cout << "Game Initialised" << endl;

	Graphics = nullptr;
	bIsGameOver = false;
	
}

Game::~Game()
{
	Graphics = nullptr;
	cout << "Game Over..." << endl;
}

void Game::Run()
{
	if (!bIsGameOver) {

		//create an input class to detect input
		GameInput = new Input();

		ShaderPtr TextureShader = Graphics->CreateShader({
			L"Game/Shaders/TextureShader/TextureShader.svert",
			L"Game/Shaders/TextureShader/TextureShader.sfrag"
			});

		TexturePtr TConcrete = Graphics->CreateTexture("Game/Textures/GreySquare_S.jpg");
		TexturePtr TTech = Graphics->CreateTexture("Game/Textures/seamless-tech-texture.jpg");

		//create the material
		MaterialPtr MConcrete = make_shared<Material>();
		MaterialPtr MTech = make_shared<Material>();

		MConcrete->BaseColour = TConcrete;
		MTech->BaseColour = TTech;

		//create meshes
		Model = Graphics->CreateSimpleModelShape(GeometricShapes::Cube, TextureShader);
		Model2 = Graphics->CreateSimpleModelShape(GeometricShapes::Cube, TextureShader);

		//set materials of the models
		Model->SetMaterialBySlot(0, MConcrete);
		Model2->SetMaterialBySlot(0, MTech);

		//transforming the models location
		Model->Transform.Location = Vector3(5.0f, 0.0f, 1.0f);
		Model2->Transform.Location = Vector3(5.0f, 0.0f, -1.0f);

		//import custom meshes 
		Wall = Graphics->ImportModel("Game/Models/damaged-wall/source/SM_Wall_Damaged.obj", TextureShader);
		Wall2 = Graphics->ImportModel("Game/Models/damaged-wall/source/SM_Wall_Damaged.obj", TextureShader);

		//transform the wall
		Wall->Transform.Scale = Vector3(0.05f);
		Wall->Transform.Rotation.y = 90.0f;
		Wall->Transform.Location = Vector3(2.0f, -2.0f, 0.0f);

		Wall2->Transform.Scale = Vector3(0.05f);
		Wall2->Transform.Rotation.y = 90.0f;
		Wall2->Transform.Location = Vector3(5.0f, -2.0f, 0.0f);

		//create the texture
		TexturePtr TWall = Graphics->CreateTexture("Game/Models/damaged-wall/textures/T_Wall_Damaged_BC.png");

		//create a material
		MaterialPtr MWall = make_shared<Material>();
		MWall->BaseColour = TWall;

		//apply the material
		Wall->SetMaterialBySlot(1, MWall);
		Wall2->SetMaterialBySlot(1, MWall);

	}

	while (!bIsGameOver) {
		//Make sure twe process what the user has done
		ProcessInput();
		
		//Apply the logic base on the inputs and the AI Logic
		Update();

		//Render the screen based on the 2 functions above
		Draw();
	}

	//Clean the game after it ends
	CloseGame();
}

void Game::Update()
{
	//set delta time first always
	static double LastFrameTime = 0.0;
	//set the current time since the game has passed
	double CurrentFrameTime = static_cast<double>(SDL_GetTicks64());
	// find the time difference between the last and current frame
	double NewDeltaTime = CurrentFrameTime - LastFrameTime;
	//set delta time as seconds
	DeltaTime = NewDeltaTime / 1000.0;
	//update the last frame time for the next update
	LastFrameTime = CurrentFrameTime;
	
	Model->Transform.Rotation.x += 50.0f * GetFDeltaTime();
	Model->Transform.Rotation.y += 50.0f * GetFDeltaTime();
	Model->Transform.Rotation.z += 50.0f * GetFDeltaTime();

	Model2->Transform.Rotation.x += -50.0f * GetFDeltaTime();
	Model2->Transform.Rotation.y += -50.0f * GetFDeltaTime();
	Model2->Transform.Rotation.z += -50.0f * GetFDeltaTime();

	Wall->Transform.Rotation.x += -50.0f * GetFDeltaTime();
	Wall->Transform.Rotation.y += -50.0f * GetFDeltaTime();
	Wall->Transform.Rotation.z += -50.0f * GetFDeltaTime();

	Vector3 CameraInput = Vector3(0.0f);
	CDirections CamDirections = Graphics->EngineDefaultCam->GetDirections();

	//move camera forward
	if (GameInput->IsKeyDown(SDL_SCANCODE_W)) {
		CameraInput += CamDirections.Forward;
	}

	//move camera backward
	if (GameInput->IsKeyDown(SDL_SCANCODE_S)) {
		CameraInput += -CamDirections.Forward;
	}

	//move camera left
	if (GameInput->IsKeyDown(SDL_SCANCODE_A)) {
		CameraInput += -CamDirections.Right;
	}

	//move camera right
	if (GameInput->IsKeyDown(SDL_SCANCODE_D)) {
		CameraInput += CamDirections.Right;
	}

	//move camera up
	if (GameInput->IsKeyDown(SDL_SCANCODE_Q)) {
		CameraInput += -CamDirections.Up;
	}

	//move camera down
	if (GameInput->IsKeyDown(SDL_SCANCODE_E)) {
		CameraInput += CamDirections.Up;
	}

	if (GameInput->IsKeyDown(SDL_SCANCODE_LSHIFT)) {

	}
	
	//move the camera according to input
	Graphics->EngineDefaultCam->AddMovementInput(CameraInput);

	if (GameInput->IsMouseButtonDown(MouseButtons::RIGHT)) {
		Graphics->EngineDefaultCam->RotatePitch(-GameInput->MouseYDelta * GetFDeltaTime());
		Graphics->EngineDefaultCam->RotateYaw(GameInput->MouseXDelta * GetFDeltaTime());
		GameInput->ShowCursor(false);
	}
	else {
		GameInput->ShowCursor(true);
	}
}

void Game::ProcessInput()
{
	//Run the input detection for our game input
	GameInput->ProcessInput();
}

void Game::Draw()
{
	Graphics->Draw();
}

void Game::CloseGame()
{
	delete GameInput;
}
