#pragma once
#include "../../Libraries/Framework/Source/Common/spritesheet.h"

class ShaderProgram;
class GameObject;
class SpriteSheet;
#define SPRITE_COLUMNS = 4
#define SPRITE_ROWS = 4

#define SPRITE_ 0

enum WalkingDirection 
{
	idle,
	left,
	right,
	up,
	down 
};

struct SpriteFrames
{
    const char* DownString = "LinkWalkDown";
    const char* LeftString = "LinkWalkLeft";
    const char* RightString = "LinkWalkRight";
    const char* UpString = "LinkWalkUp";

    int AnimationFrames = 2;
};


class Sprite :public GameObject
{
protected:
 
    SpriteFrames m_SpriteFrames;

	SpriteSheet * m_pSpriteSheet;

	GLuint texture;
	float m_AccumlatedTime;
	int m_CurrentFrameNumber;
	ShaderProgram* pShader;
	WalkingDirection m_WalkingDirection;
	Vector2 m_UVScale;
	Vector2 m_UVOffset;
    Mesh * pMesh;

	void Move(enum WalkingDirection walking);
    void UseTexture(SpriteSheet * a_pSpriteSheet);

public:

	Sprite();
	~Sprite();
	void Update(double deltatime);

};