#include "GamePCH.h"



Sprite::Sprite()
    : GameObject(pMesh, pShader, texture)
{

	m_AccumlatedTime = 0.0f;
	m_CurrentFrameNumber = 0;
	m_WalkingDirection = WalkingDirection::idle;

    pShader = new ShaderProgram("Data/Shaders/color.vert", "Data/Shaders/color.frag");

    m_SpriteFrames = SpriteFrames();
}

Sprite::~Sprite()
{
}

void Sprite::Update(double deltatime)
{
	vec2 direction(0, 0);



	if (direction.y > 0)
	{
		m_WalkingDirection = WalkingDirection::up;
	}

	if (direction.y < 0)
	{
		m_WalkingDirection = WalkingDirection::down;
	}

	if (direction.x < 0)
	{
		m_WalkingDirection = WalkingDirection::left;
	}

	if (direction.x > 0)
	{
		m_WalkingDirection = WalkingDirection::right;
	}
	if (direction == vec2(0, 0))
	{
		m_WalkingDirection = WalkingDirection::idle;

	}


	m_AccumlatedTime += (float)deltatime;


	if (m_AccumlatedTime > 0.2f)
	{
		m_AccumlatedTime = 0.0f;
		m_CurrentFrameNumber++;

		if (m_CurrentFrameNumber > 1)
		{
			m_CurrentFrameNumber = 0;

		}
	}

	std::string animfilename;

	int actualframe = m_CurrentFrameNumber;

	if (m_WalkingDirection == WalkingDirection::idle)
	{
		animfilename = m_SpriteFrames.DownString + std::to_string(1);

	}

	if (m_WalkingDirection == WalkingDirection::down)
	{
		animfilename = m_SpriteFrames.DownString + std::to_string(actualframe + 1);

	}


	if (m_WalkingDirection == WalkingDirection::left)
	{
		animfilename = m_SpriteFrames.LeftString + std::to_string(actualframe + 1);

	}

	if (m_WalkingDirection == WalkingDirection::right)
	{
		

	}

	if (m_WalkingDirection == WalkingDirection::up)
	{
		animfilename = m_SpriteFrames.UpString + std::to_string(actualframe + 1);

	}

	animfilename += ".png";

	SpriteInfo* pInfo = m_pSpriteSheet->GetSpriteInfoByFilename(animfilename);
	vec2 texsize = m_pSpriteSheet->GetTextureSize();

	m_UVScale.Set(pInfo->m_Size.x / (float)texsize.x, pInfo->m_Size.y / (float)texsize.y);
	m_UVOffset.Set(pInfo->m_Offset.x / texsize.x, pInfo->m_Offset.y / texsize.y);
}

void Sprite::Move(WalkingDirection walking)
{

	std::string animfilename;

	if (walking == WalkingDirection::right)
	{
		//animfilename = m_SpriteFrames.RightString + std::to_string(Frame + 1);
	}

	
	animfilename += ".png";

}

void Sprite::UseTexture(SpriteSheet * a_pSpriteSheet)
{
    m_pSpriteSheet = a_pSpriteSheet;
}

