#include "GamePCH.h"

Score::Score()
{
    //INITALIZE
	m_score = STARTINGSCORE;
	m_pCamera = 0;



	m_AccumilatedTime = 0.0f;
	m_TimeToNextFrame = SCORETIMER;
	m_Position = Vector2(5.0f, 5.0f);

	m_AmountOfFramesInTheRow = 5;
	m_AmountOfFramesInTheColumn = 2;

	m_MaxFrameNumber = m_AmountOfFramesInTheRow * m_AmountOfFramesInTheColumn;

	m_TextureWidth = TEXTUREWIDTH;
	m_TextureHeight = TEXTUREHEIGHT;

	m_pMesh = new Mesh();

	m_pShader = new ShaderProgram("Data/Shaders/Score.vert", "Data/Shaders/Score.frag");
	m_TextureHandle = LoadTexture("Data/Textures/numbers.png", m_TextureWidth, m_TextureHeight);
	m_pMesh->GenerateSprite(0.1f, 0.1f);

}

Score::~Score()
{
    //DELETE
	delete m_pMesh;
	delete m_pShader;

    glDeleteTextures(1, &m_TextureHandle);

}

void Score::Draw()
{
    //DRAW the score
	m_pMesh->DrawScore(m_pShader, m_TextureHandle, m_Position, m_TextureWidth, m_TextureHeight, m_AmountOfFramesInTheRow, m_AmountOfFramesInTheColumn, m_score, m_pCamera);
}

void Score::Update(float delta)
{
    //SET m_AccumilatedTime  to add delta
	m_AccumilatedTime += delta;
    //IF the m_AccumilatedTime  is larger or equal to m_TimeToNextFrame
	if (m_AccumilatedTime >= m_TimeToNextFrame)
	{
        //Set m_AccumilatedTime to 0
		m_AccumilatedTime = 0.0f;
        //ADD m_Score
		m_score++;
	}
}

void Score::AddScore(int score)
{
    //ADD m_Score to the argument
	m_score += score;
}

void Score::SetCamera(Camera * cam)
{
    //SET camera
	m_pCamera = cam;
}
