#ifndef __Scene_H__
#define __Scene_H__

#include <time.h> 
#include <vector>
using namespace std;
class Game;
class Tilemap;
class Tower;
class Enemy;
class Pathfinder;
class SpriteSheet;
class Score;
class Camera;

class Scene
{
public:
	SpriteSheet* m_pSpritesheet;

	Camera* m_pCamera;
	Score* m_pScore;
    Tower* m_pTowerCursor;	


    Pathfinder* m_pPathfinder;
    Pathfinder* m_pPathfinder1;
    Pathfinder* m_pPathfinder2;
    Pathfinder* m_pPathfinder3;
    Pathfinder* m_pPathfinder4;
    
    Tilemap* m_pTilemap;
    Tilemap* m_pTilemap1;
    Tilemap* m_pTilemap2;
    Tilemap* m_pTilemap3;
    Tilemap* m_pTilemap4;

    vector<Tower*> m_VecTowers;
    vector<Enemy*> m_VecEnemy;

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
    GLuint m_TextureRed;
    GLuint m_TextureGray;
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
    Scene(Framework * a_pFramework);
    virtual ~Scene();

    void LoadContent(Game* pGame);
    void Update(float deltatime);
	void SceneAddEnemy();
    void Draw();
	void Reset();
	void HandleEvent(Event* aeven);
	void AddTower();
    void AddTileType(TileTypes tile);
    void ChangeTile();
};

#endif //__Scene_H__