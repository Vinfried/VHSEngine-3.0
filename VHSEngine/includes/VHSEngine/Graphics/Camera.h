#pragma once
#include "VHSEngine/Math/Transformations.h"
#include "VHSEngine/CoreMinimal.h"

struct STCameraData {

	//how fast the camera can move
	float Speed = 5.0f;

	//basically the zoom
	//how much of the view space the camera can see - in degree
	//think peripherical vision
	float FOV = 70.0f;

	//how close can models get to the camera before disapering
	float NearClip = 0.01f;

	//opposite of near clip
	float FarClip = 1000.0f;

	//speed which carema turn is multipled
	float LookSensitivity = 0.2f;
};

class Camera {

public:
	Camera();

	//move camera in 3d space to a location
	void Translate(Vector3 Location);

	void AddMovementInput(Vector3 Direction);

	//return the transform
	CTransform GetTransform() const { return Transform; }

	//return the current facing directions
	CDirections GetDirections() const { return Directions; }

	//return the extra camera info
	//Speed, FOV, Near Clip and Far Clip
	STCameraData GetCameraData() const { return CameraData; }

	//return a view matrix that considers position AND rotation
	glm::mat4 GetViewMatrix() const;

	//rotate camera based on PITCH
	void RotatePitch(float Amount);

	//rotate camera based on YAW
	void RotateYaw(float Amount);

	//update camera logic
	void Update();

	//get the collision for camera
	CollisionPtr GetCameraCollision() const { return CameraCollision; }

private:

	//find the current directions vectors based on the rotation of the YAW and PITCH of the camera
	void UpdateDirectionVectors();

private:
	//transform of the camera
	//location, rotation, scale - we don't need that... here
	CTransform Transform;

	//Forward, right and u directions
	CDirections Directions;

	//hold all the extra camera info
	STCameraData CameraData;

	//add a camerac collision
	CollisionPtr CameraCollision;
};