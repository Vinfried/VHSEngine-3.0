#pragma once
#include "VHSEngine/CoreMinimal.h"
#include "VHSEngine/Math/Transformations.h"

class Collision {
public:
	Collision(Vector3 Location, Vector3 Offset);
	~Collision();

	//detect if two collisions are overlapping
	virtual bool IsOverLapping(Collision& OtherCol) { return false; }

	//get the center of the collision
	virtual Vector3 FindCenter() { return Vector3(0.0f); }
	
	//draw the collision for debbuging
	virtual void DebugDraw(Vector3 Colour) {}

	//Transform functions
	//set the location of the collision
	virtual void SetLocation(Vector3 NewLocation) { Transform.Location = NewLocation; }
	//update the offset
	virtual void SetOffset(Vector3 NewOffset) { Offset = NewOffset; }
	//set the rotation of the collision x
	virtual void SetRotationX(float Value) { Transform.Rotation.x = Value; };
	//set the rotation of the collision y
	virtual void SetRotationY(float Value) { Transform.Rotation.y = Value; };
	//set the rotation of the collision z
	virtual void SetRotationZ(float Value) { Transform.Rotation.z = Value; };

protected:
	//transform for the collision in world space
	CTransform Transform;

	//offset for the center location
	Vector3 Offset;

	//visual mesh for debbuging
	MeshPtr DebugMesh;
	ShaderPtr DebugShader;
	MaterialPtr DebugMaterial;
};

class BoxCollision :
	public Collision {
public:
	BoxCollision(Vector3 Location, Vector3 Offset, Vector3 Dimentions);

	bool IsOverLapping(Collision& OtherCol) override;
	Vector3 FindCenter() override;
	//draw mesh to the dimentions of the box
	//turn the mesh into a wireframe view to not obscure the actual object
	//create the mesh and a shader only if we need to debugg a collision
	void DebugDraw(Vector3 Colour) override;
	void SetLocation(Vector3 NewLocation) override;

	void GetDimentions(Vector3 NewDimentions);

	//set the dimentions of the box
	void SetDimentions(Vector3 NewDimentions);
protected:
	//bounding box values
	Vector3 Dimentions;

	//the bounding vectors between the minimum and maximum size of the collision
	Vector3 Min;
	Vector3 Max;

};