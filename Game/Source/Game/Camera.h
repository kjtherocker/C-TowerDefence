#pragma once


#include "GamePCH.h"

class Camera
{
public:
	Camera();
	~Camera();
	void SetObjectTranslation(Vector2 vector);
	void SetCameraTranslation(Vector2 Vector);
	void SetCameraScale(Vector2 Scale);


	Vector2 GetObjectTranslation();
	Vector2 GetCameraTranslation();
	Vector2 GetCameraScale();
    void SetToStart();
private:

	Vector2 Object_Position;
	Vector2 Camera_Position;
    Vector2 StartingPos;
	Vector2 scale;
	float angle;




};