#pragma once
#include "VHSEngine/CoreMinimal.h"
#include "VHSEngine/Math/Transformations.h"

struct Vertex;

class Mesh
{
public:
	Mesh();
	~Mesh();
	//Create a mesh out of VAO, a Shader and Textures
	bool CreateSimpleShape(GeometricShapes Shape, ShaderPtr MeshShader, vhsuint MaterialSlot);

	//create a mesh VAO that has a predefined model
	bool CreateMesh(vector<Vertex> Vertices, vector<vhsuint> Indices, ShaderPtr MeshShader, vhsuint MaterialSlot);

	//handle the drawing of all the required classes
	void Draw(MaterialPtr MeshMaterial);

	//return material slot assigned by the model to the mesh
	vhsuint GetMaterialSlot() const { return MaterialSlot; }

public:
	//this holds the position, rotation and scale of the mesh
	CTransform Transform;

private:
	//store the shader this mesh requires
	ShaderPtr MeshShader;

	//assign material slot relevant to the model class
	vhsuint MaterialSlot;

	//Create a new VAO for the mesh
	VAOPtr MeshVAO;

};

