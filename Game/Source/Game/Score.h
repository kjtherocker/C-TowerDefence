#pragma once
class Camera;


class Score
{
public:
	Score();
	~Score();
	void Draw();
	void Update(float delta);
	void AddScore(int score);
	void SetCamera(Camera* cam);

	int m_score;
	GLuint m_TextureHandle;
	Camera* m_pCamera;

	ShaderProgram* m_pShader;
	Mesh * m_pMesh;
	
	float m_AccumilatedTime;
	float m_TimeToNextFrame;
	int m_MaxFrameNumber;
	int m_AmountOfFramesInTheRow;
	int m_AmountOfFramesInTheColumn;


	Vector2 m_Position;

   unsigned int m_TextureWidth;
   unsigned int m_TextureHeight;

protected:



};

