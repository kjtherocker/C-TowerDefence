#ifndef __Player_H__
#define __Player_H__

class Tilemap;
class SpriteInfo;
class SpriteSheet;
class GameObject;
class PlayerController;
class Camera;
class Enemy;
#include <time.h> 
#include <vector>
using namespace std;

enum WalkingDirection
{
	idle,
	left,
	right,
	up,
	down
};


class Player : public GameObject
{
protected:
    PlayerController* m_pController;
	WalkingDirection m_WalkingDirection;
    Tilemap* m_pTilemap;
    vec2 m_PlayerSize;
	SpriteSheet* m_pSpriteSheet;
	vector<Bullet*> m_VecBullet;
	Mesh * m_pBulletMesh;
	Mesh * m_pMeshEnemyHealth;
	GLuint m_TextureBullet;
	float m_AccumlatedTime;
	int m_CurrentFrameNumber;
	float m_Timer;
	float m_TimerShot;
	bool m_JustShot;
	bool m_JustHit;
public:
	Player(Mesh* pMesh, ShaderProgram* pShader, Mesh* healthmesh, ShaderProgram* pShaderhealth, GLuint texture, vec2 position, vec2 size, Mesh * a_Bulletmesh, Mesh * a_BulletHealmMesh, GLuint a_BulletTexture);
	virtual ~Player();

    void SetController(PlayerController* pController);
    void SetTilemap(Tilemap* pTilemap);
	void SetSpriteSheet(SpriteSheet* Sprite);
	void SetCamera(Camera* cam);
	void DrawBullets();
	void CheckCollisionBullet(Enemy * enemy,float deltatime);
	void CheckCollisionPlayer(Enemy* a_Enemy);
	void PlayerAddBullets(Vector2 a_CursorPos);
	void Spawn();
    bool WouldPlayerFitOnMap(vec2 worldpos);
	bool Shoot();
	bool PlayerIFrames();

    virtual void Update(float deltatime);
};

#endif //__Player_H__