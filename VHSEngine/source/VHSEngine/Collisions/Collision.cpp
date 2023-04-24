#include "VHSEngine/Collisions/Collision.h"
#include "VHSEngine/Graphics/ShaderProgram.h"
#include "VHSEngine/Graphics/Mesh.h"
#include "VHSEngine/Graphics/Material.h"
#include "VHSEngine/Graphics/VertexArrayObject.h"
#include"GL/glew.h"

Collision::Collision(Vector3 Location, Vector3 Offset)
{
	Transform.Location = Location;
	this->Offset = Offset;
	DebugMesh = nullptr;
	DebugShader = nullptr;

	cout << "Creating collision..." << endl;
}

Collision::~Collision()
{
	DebugMesh = nullptr;
	DebugShader = nullptr;

	cout << "Collision destroyed" << endl;
}

BoxCollision::BoxCollision(Vector3 Location, Vector3 Offset, Vector3 Dimentions) : 
	Collision(Location, Offset)
{
	this->Dimentions = Dimentions;

	//find the min and max vector for this box
	Min = FindCenter() - (Dimentions / 2.0f);
	Max = FindCenter() + (Dimentions / 2.0f);
}

bool BoxCollision::IsOverLapping(Collision& OtherCol)
{
	const BoxCollision OtherBox = dynamic_cast<BoxCollision&>(OtherCol);

	//return if the box is within the other boz
	//are the minimum values higher than the max and vice versa
	return  (
		OtherBox.Min.x <= Max.x &&
		OtherBox.Max.x <= Min.x &&
		OtherBox.Min.y <= Max.y &&
		OtherBox.Max.y <= Min.y &&
		OtherBox.Min.z <= Max.z &&
		OtherBox.Max.z <= Min.z
		);
}

Vector3 BoxCollision::FindCenter()
{
	return Transform.Location + Offset;
}

void BoxCollision::DebugDraw(Vector3 Colour)
{
	//if there is no debug mesh then create one
	if (DebugMesh == nullptr) {
		DebugShader = make_shared<ShaderProgram>();
		DebugShader->InitVFShader({
			L"Game/Shaders/WireframeShader/WireframeShader.svert",
			L"Game/Shaders/WireframeShader/WireframeShader.sfrag"
		});

		//create a box
		DebugMesh = make_shared<Mesh>();
		if (!DebugMesh->CreateSimpleShape(GeometricShapes::Cube, DebugShader, 0)) {
			DebugMesh = nullptr;
			DebugShader = nullptr;
			return;
		}

		DebugMaterial = make_shared<Material>();
		DebugMaterial->BaseColour.TextureV3 = nullptr;
		DebugMaterial->SpecularColour.TextureV3 = nullptr;
		DebugMaterial->EmissiveColour.TextureV3 = nullptr;
	}

	//convert current rendering mode in a ;inne mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//transform the debug mesh to fit the collision values
		DebugMesh->Transform.Location = FindCenter();
		DebugMesh->Transform.Rotation = Transform.Rotation;
		DebugMesh->Transform.Scale = Dimentions;
		//draw the mesh
		DebugMesh->Draw(DebugMaterial);
		//change the colour of the debug wireframe mesh
		DebugShader->RunShader();
		DebugShader->SetVector3("InColour", Colour);

	//return to the default draw mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void BoxCollision::SetLocation(Vector3 NewLocation)
{
	Collision::SetLocation(NewLocation);

	//update the min and max vector for this box
	Min = FindCenter() - (Dimentions / 2.0f);
	Max = FindCenter() + (Dimentions / 2.0f);
}

void BoxCollision::SetDimentions(Vector3 NewDimentions)
{
	//set the dimentions to the new dimentions
	Dimentions = NewDimentions;

	//update the min and max vector for this box
	Min = FindCenter() - (Dimentions / 2.0f);
	Max = FindCenter() + (Dimentions / 2.0f);
}
