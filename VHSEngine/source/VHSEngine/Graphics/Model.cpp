#include "VHSEngine/Graphics/Model.h"
#include "VHSEngine/Graphics/Mesh.h"
#include "VHSEngine/Game.h"
#include "VHSEngine/Graphics/Vertex.h"
//ASSIMP HEADERS
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"


Model::Model()
{
	ModelFilePath = "";
}

Model::~Model()
{
	//clean up the model
	MeshStack.clear();
	MaterialStack.clear();
	ModelShader = nullptr;
}

bool Model::CreateSimpleMesh(GeometricShapes Shape, ShaderPtr ModelShader)
{
	//initialise the mesh
	MeshPtr NewMesh = make_shared<Mesh>();

	//try to create a simple shape
	if (!NewMesh->CreateSimpleShape(Shape, ModelShader, 0))
		return false;
	
	//clear the older stuff if there's any and assign the new mesh data
	MeshStack.clear();
	MaterialStack.clear();
	this->ModelShader = ModelShader;

	//assig th default engine material to the first material slot
	MaterialStack.push_back(Game::GetGameInstance().GetDefaultEngineMaterial());

	//assign the mesh to the model mesh stack
	MeshStack.push_back(NewMesh);

	return true;
}

bool Model::ImportMeshFromFile(const char* ImportFilePath, ShaderPtr ModelShader)
{
	

	//create an importer using assimp
	Assimp::Importer Importer;
	//attempt to import the new mesh based in the file path using the importer
	const aiScene* Scene = Importer.ReadFile(ImportFilePath, aiProcess_Triangulate);

	//check if the file imported correctly
	//!Scene means the file path probalby didn't work
	//AI_SCENE_FLAGS_INCOMPLETE means the file is probably corrupted or cant be read by ASSIMP
	//!Scene->RootNode means it's also either corrupted or not supported by ASSIMP
	if (!Scene || Scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode) {
		cout << "Model | Error importing model from " << ImportFilePath << " - " << Importer.GetErrorString() << endl;
		return false;

	}

	//assign the model shader
	this->ModelShader = ModelShader;

	//save the file path
	ModelFilePath = ImportFilePath;

	//find and import all the meshes using the imported scene
	FindAndImportSceneMeshes(Scene->mRootNode, Scene);

	//run through all the new material slots and assign the default material
	for (vhsuint i = 0; i < MaterialStack.size(); i++) {
		MaterialStack[i] = Game::GetGameInstance().GetDefaultEngineMaterial();
	}

	return true;
}

void Model::Draw()
{
	//cycle through the meshes and draw each e=one
	for (MeshPtr LMesh : MeshStack) {
		//assign the model transformations to the mesh
		LMesh->Transform = this->Transform;

		//draw the mesh using the material slot it gas been assign
		LMesh->Draw(MaterialStack[LMesh->GetMaterialSlot()]);
	}
}

void Model::SetMaterialBySlot(vhsuint SlotIndex, MaterialPtr NewMaterial)
{
	//check if the index is valid to the array
	if (MaterialStack.size() <= SlotIndex) {
		cout << "Model | No material slot of that index (" << SlotIndex << ") exists." << endl;
		return;
	}

	//reassign the array item
	MaterialStack[SlotIndex] = NewMaterial;
}

MaterialPtr Model::GetMaterialBySlot(vhsuint SlotIndex) const
{
	//check if the index is valid to the array
	if (MaterialStack.size() <= SlotIndex) {
		cout << "Model | No material slot of that index (" << SlotIndex << ") exists." << endl;
		return nullptr;
	}
	return MaterialStack[SlotIndex];
}

void Model::FindAndImportSceneMeshes(aiNode* Node, const aiScene* Scene)
{
	//run through all of the meshes in the root node meshes of the scene import those
	for (vhsuint i = 0; i < Node->mNumMeshes; i++) {
		//get the mesh index
		vhsuint ImpMeshIndex = Node->mMeshes[i];
		//get the one of the meshes
		aiMesh* ImpMesh = Scene->mMeshes[ImpMeshIndex];

		//convert the imported mesh to our engines mesh
		MeshPtr ConvertedMesh = ConvertImportMeshToEngineMesh(ImpMesh, Scene);

		//add the new converted mesh into our mesh array
		if (ConvertedMesh != nullptr)
			MeshStack.push_back(ConvertedMesh);
	}

	//check through all meshes that are children if the root meshes
	//won't run if the mesh has no children
	for (vhsuint i = 0; i < Node->mNumChildren; i++) {
		FindAndImportSceneMeshes(Node->mChildren[i], Scene);
	}
}

MeshPtr Model::ConvertImportMeshToEngineMesh(aiMesh* ImportMesh, const aiScene* Scene)
{
	//initialise local versions of the mesh vertices and indices
	vector<Vertex> Vertices;
	vector<vhsuint> Indices;

	//loop through all of the vertices and store their location, normal value and tex cood
	for (vhsuint i = 0; i < ImportMesh->mNumVertices; i++) {
		//initialise a single vertex
		Vertex LVertex;

		//find the position of the vertex
		LVertex.Position = Vector3(
			ImportMesh->mVertices[i].x,
			ImportMesh->mVertices[i].y,
			ImportMesh->mVertices[i].z
		);

		//find the normal value of the vertex (facing direction)
		LVertex.Normal = Vector3(
			ImportMesh->mNormals[i].x,
			ImportMesh->mNormals[i].y,
			ImportMesh->mNormals[i].z
		);

		//find the texture coordinates of the vertex
		LVertex.TexCoord = Vector2(
			ImportMesh->mTextureCoords[0][i].x,
			ImportMesh->mTextureCoords[0][i].y
		);

		//add the vertex into the vertices array
		Vertices.push_back(LVertex);
	}

	//make sure there are more than 3 vertices or stop creating the mesh
	if (Vertices.size() < 3) {
		cout << "Model | One of the imported Mesh doesn't have enough vertices at " << ModelFilePath << endl;
		return nullptr;
	}

	//next we find the indices
	//to find the indices we can use a value called faces
	//faces are basically 3 vertices
	for (vhsuint i = 0; i < ImportMesh->mNumFaces; i++) {
		//store the face
		aiFace Face = ImportMesh->mFaces[i];

		//run through another loop to detect all of the indices in the face
		for (vhsuint j = 0; j < Face.mNumIndices; j++) {
			//add each index to the indices arrray
			Indices.push_back(Face.mIndices[j]);
		}
	}

	//find and assign materials slots
	vhsuint MaterialIndex = ImportMesh->mMaterialIndex;

	//resize the material stack to whatever the largest material slot index is
	if (MaterialStack.size() < MaterialIndex + 1) {
		//change the size of the array to the largest index + 1
		MaterialStack.resize(MaterialIndex + 1);
	}

	//create and ssign the mesh
	MeshPtr ConvertedMesh = make_shared<Mesh>();

	ConvertedMesh->CreateMesh(Vertices, Indices, ModelShader, MaterialIndex);


	return ConvertedMesh;
}
