#include "VHSEngine/Graphics/Mesh.h"
#include "VHSEngine/Graphics/ShaderProgram.h"
#include "VHSEngine/Graphics/Material.h"
#include "VHSEngine/Graphics/VertexArrayObject.h"
#include "glm/gtc/matrix_transform.hpp"
#include "VHSEngine/Game.h"
#include "VHSEngine/Graphics/GraphicsEngine.h"

Mesh::Mesh()
{
	cout << "Initiate mesh" << endl;

}

Mesh::~Mesh()
{
	MeshShader = nullptr;
	MeshVAO = nullptr;
	cout << "Mesh destroyed" << endl;
}

bool Mesh::CreateSimpleShape(GeometricShapes Shape, ShaderPtr MeshShader, vhsuint MaterialSlot)
{
	cout << "Mesh | Creating Mesh" << endl;
	//Create VAO
	MeshVAO = make_shared<VAO>(Shape);

	//validade the mesh was created
	if (MeshVAO == nullptr) {
		cout << "Mesh | Mesh failed to be created" << endl;
		return false;
	}

	//assign the shader and textures
	this->MeshShader = MeshShader;
	this->MaterialSlot = MaterialSlot;

	cout << "Mesh | Mesh Created successfully" << endl;

	return true;
}

bool Mesh::CreateMesh(vector<Vertex> Vertices, vector<vhsuint> Indices, ShaderPtr MeshShader, vhsuint MaterialSlot)
{

	cout << "Mesh | Creating Mesh" << endl;
	//Create VAO
	MeshVAO = make_shared<VAO>(Vertices, Indices);

	//validade the mesh was created
	if (MeshVAO == nullptr) {
		cout << "Mesh | Mesh failed to be created" << endl;
		return false;
	}

	//assign the shader and textures
	this->MeshShader = MeshShader;
	this->MaterialSlot = MaterialSlot;

	cout << "Mesh | Mesh Created successfully" << endl;

	return true;
}

void Mesh::Draw(MaterialPtr MeshMaterial)
{
	//activate the shader that this mesh uses
	MeshShader->RunShader();

	//runthe material for this mesh
	//activate all required textures in the material
	if (MeshMaterial != nullptr) {
		MeshMaterial->Draw(MeshShader);
	}

	//initialise a static variable to check if any changes to transform
	static CTransform OldTransform;

	if (Transform != OldTransform) {
		OldTransform = Transform;

		glm::mat4 MatTransform = glm::mat4(1.0f);

		//translate - rotate - scale --- in this order
		//translate
		MatTransform = glm::translate(MatTransform, Transform.Location);
		//rotate
		MatTransform = glm::rotate(MatTransform, glm::radians(Transform.Rotation.x), Vector3(1.0f, 0.0f, 0.0f));
		MatTransform = glm::rotate(MatTransform, glm::radians(Transform.Rotation.y), Vector3(0.0f, 1.0f, 0.0f));
		MatTransform = glm::rotate(MatTransform, glm::radians(Transform.Rotation.z), Vector3(0.0f, 0.0f, 1.0f));
		//scale
		MatTransform = glm::scale(MatTransform, Transform.Scale);

		//update the shader with the new transform
		MeshShader->SetMat4("model", MatTransform);
	}

	//Create the world and screen transformation for this object
	Game::GetGameInstance().GetGraphicsEngine()->ApplyScreenTransformations(MeshShader);

	//draw the VAO
	MeshVAO->Draw();
}
