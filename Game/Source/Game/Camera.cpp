#include "GamePCH.h"
#include "Camera.h"

Camera::Camera()
{
	//Initalizing variables
	//Initialize camera's position 
	Object_Position = vec2(16.0f / 2.0f - 0.5f, 12.0f / 2.0f);
    StartingPos = vec2(0.0f, 0.0f);
	Camera_Position = vec2(16.0f / 2.0f - 0.5f, 12.0f / 2.0f);
	//Initialize camera's Scale position 
	scale = vec2(1.0f / (16 / 2), 1.0f / (12 / 2));
	angle = 0.0f;
}

Camera::~Camera()
{
}

void Camera::SetObjectTranslation(Vector2 vector)
{
	//SET object_Position to the SetObjectTranslation Argument vector
	Object_Position = vector;


}

void Camera::SetCameraTranslation(Vector2 Vector)
{
	//SET The camera’s positon to the Set CameraTranslation argument Vector
	Camera_Position = Vector;
}

void Camera::SetCameraScale(Vector2 Scale)
{
	// SET the scale to the SetCamerScale Argurment Scale
	scale = Scale;
}

Vector2 Camera::GetObjectTranslation()
{
	//Returning Objects Translation using the variable Object_Positon
	return Vector2(Object_Position);
}

Vector2 Camera::GetCameraTranslation()
{
	//Return GetCameraTranslate using the variable Camera_Position;
	return Vector2(Camera_Position);
}

Vector2 Camera::GetCameraScale()
{
	//Return GetCameraScale using the variable scale.
	return Vector2(scale);
}

void Camera::SetToStart()
{
	//Setting the camera to a StartingPosition vector2(0.0f,0.0f);
    Camera_Position = StartingPos;
}


