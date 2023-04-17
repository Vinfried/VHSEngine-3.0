#pragma once
#include "VHSEngine/CoreMinimal.h"
#include "glm/glm.hpp"
class ShaderProgram {

public:
	ShaderProgram();
	~ShaderProgram();

	//this will initialise our shader as a vertex fragment shader
	bool InitVFShader(VFShaderParams ShaderFilePaths);

	//Run the shader program as the current shader to be interacted with in OpenGL
	void RunShader();

	//returns the program ID
	vhsuint GetID() const { return ProgramID; }

	//allow changingg mat 4 variables in shader code
	void SetMat4(const char* ShaderVariable, glm::mat4 Value);
	//allow changing integer values in shader code
	void SetInt(const char* ShaderVarName, int Value);
private:
	//Attach the shader to the VAO in openGL
	bool AttachShader(const wchar_t* ShaderFilePath, ShaderTypes Type);
	
	//Link the shader to OpenGL
	bool Link();

private:
	//openGL will assign an ID to our Shader
	vhsuint ProgramID;

	//array that will hold IDs to our unique shaders
	//0 - Vertex
	//1 = Fragment
	vhsuint VFShaderIDs[2] = { 0 };
};