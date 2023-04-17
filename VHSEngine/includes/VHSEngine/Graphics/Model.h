#pragma once

#include "VHSEngine/CoreMinimal.h"
#include "VHSEngine/Math/Transformations.h"

struct aiNode;
struct aiScene;
struct aiMesh;

class Model {
public:
	Model();
	~Model();

	//create a simple shape based on the engine shapes
	bool CreateSimpleMesh(GeometricShapes Shape, ShaderPtr ModelShader);

	//import a 3d model from a common file type using ASSIMP
	bool ImportMeshFromFile(const char* ImportFilePath, ShaderPtr ModelShader);

	//draw models to the screen
	void Draw();

	//change athe material relevant to the slot index
	//this will error log if the slot doesn't exist
	void SetMaterialBySlot(vhsuint SlotIndex, MaterialPtr NewMaterial);

	//get a material in the material stach if it exists
	MaterialPtr GetMaterialBySlot(vhsuint SlotIndex) const;

	const char* GetFilePath() const { return ModelFilePath; }

private:
	//find all if the meshes in a import file/scene
	void FindAndImportSceneMeshes(aiNode* Node, const aiScene* Scene);

	//convert a mesh from the importer to our engine mesh class
	MeshPtr ConvertImportMeshToEngineMesh(aiMesh* ImportMesh, const aiScene* Scene);

public:
	//position, rotation and scale of all the meshes in the model
	CTransform Transform;

private:
	//All the meshes related to this model
	MeshPtrStack MeshStack;

	//All the materials relevant to the meshes in this model
	MaterialPtrStack MaterialStack;

	//the model shader
	ShaderPtr ModelShader;

	//imported file path
	const char* ModelFilePath;


};