#pragma once

#include "SDL2/SDL.h"
#include "VHSEngine/CoreMinimal.h"
#include "VHSEngine/Math/Transformations.h"

class GraphicsEngine {
public:
	GraphicsEngine();
	~GraphicsEngine();

	//create the window
	bool InitGE(const char* WTitle, bool bFullScreen, int WWidth, int WHeight);

	//show the newest frame graphics
	void PresentGraphics();

	//clear the old frame graphics
	void ClearGraphics();

	void Draw();

	//return the sdl window
	SDL_Window* GetWindow() const;

	//import a 3d mode based on a file path
	ModelPtr ImportModel(const char* FilePath, ShaderPtr Shader);


	//create a new mesh with a material assigned
	ModelPtr CreateSimpleModelShape(GeometricShapes Shape, ShaderPtr MeshShader);

	//add new Shader
	//@param 1 - vertex shader
	//@param 2 - fragment shader
	ShaderPtr CreateShader(VFShaderParams ShaderFilePaths);

	//create a texture and add ut to the stack
	//avoid duplicates
	TexturePtr CreateTexture(const char* FilePath);

	//Create a texture and add it to the texture pack
	//screen and camera coordinates
	void ApplyScreenTransformations(ShaderPtr Shader);

	//remove model from the model stack
	void RemoveModel(ModelPtr ModelToRemove);

private:
	//this will hold the window
	SDL_Window* SdlWindow;
	//this will allow OpenGL to work with SDL
	SDL_GLContext SdlGLContext;

	//handle wireframe mode
	void HandleWireFrameMode(bool bShowWireframeMode);
	//set wireframe
	bool bWireframeMode;

	//single shader
	ShaderPtr Shader;
	//store a vector of textures
	TexturePtrStack TextureStack;
	//store all models in the game
	ModelPtrStack ModelStack;

public:
	//default camera
	CameraPtr EngineDefaultCam;

	//default texture
	TexturePtr DefaultEngineTexture;

	//default material
	MaterialPtr DefaultEngineMaterial;
};;
