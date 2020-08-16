#ifndef __Enemy_H__
#define __Enemy_H__


class Tilemap;
class SpriteInfo;
class Pathfinder;


class Enemy : public GameObject
{
protected:

	PlayerController* m_pController;
	Tilemap* m_pTilemap;
	SpriteSheet* m_pSpriteSheet;

	vec2 m_PlayerSize;   
    vec2 m_Target;

	float m_AccumlatedTime;
	bool m_Found;
	int CurrentPath[400];
	int m_CurrentFrameNumber;

public:
	Enemy(Mesh* pMesh, Mesh* healthmesh, ShaderProgram* pShaderhealth, ShaderProgram* pShader, GLuint texture, vec2 position, vec2 size);
	virtual ~Enemy();


	void SetTilemap(Tilemap* pTilemap);
	
	float GetHealth();
    void Spawn();
    bool GetIsAlive();
    bool GetFound();
    void SetPathfinder(int a_Columns, int a_Rows, Vector2 endp, Pathfinder* a_Pathfinder);
	void SetSpriteSheet(SpriteSheet* Sprite);
    void SetPath(int * a_Path, int length);
	bool WouldPlayerFitOnMap(vec2 worldpos);
	virtual void Update(float deltatime);
};

#endif 