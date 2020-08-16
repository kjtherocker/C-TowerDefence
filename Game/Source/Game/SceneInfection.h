#ifndef __SceneInfection_H__
#define __SceneInfection_H__


#include <time.h> 
#include <vector>
using namespace std;
class Game;
class Tilemap;
class Enemy;
class Pathfinder;
class SpriteSheet;
class Score;
class Camera;
class Player;

class SceneInfection
{
public:
	Tilemap* m_pTilemap;
	SpriteSheet* m_pSpritesheet;

	Camera* m_pCamera;
	Tower* m_pTowerCursor;


	Pathfinder* m_pPathfinder;

	vector<Enemy*> m_VecEnemy;

	Player* m_pPlayer;

	Framework * m_pFramework;


	ShaderProgram* m_pShader;
	ShaderProgram* m_pShaderHealth;

	Mesh* m_pMeshEnemyHealth;
	Mesh* m_pMeshEnemy;
	Mesh* m_pMeshTower;
	Mesh* m_pBulletMesh;

	GLuint m_TextureBullet;
	GLuint m_TextureEnemy;
	GLuint m_TextureTower;
	GLuint m_TextureWall;
	GLuint m_TextureGround;
	GLuint m_TextureCursor;

	int m_Rows;
	int m_Columns;
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int m_tilexindex;
	int m_tileyindex;
	int m_Tile;

	float m_EnemyTimer;
	bool m_EnemySpawn;

public:
	SceneInfection(Framework * a_pFramework);
	virtual ~SceneInfection();
	void LoadContent(Game* pGame);
	void Update(float deltatime);
	void SceneInfectionAddEnemy();
	void Draw();
	void Reset();
	void HandleEvent(Event* aeven);

}; 

#endif __SceneInfection_H__