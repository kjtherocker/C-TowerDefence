#pragma once


class Tower;
class Camera;
class Bullet;
class Enemy;
class GameObject;
#include <time.h> 
#include <vector>
using namespace std;

class Tower : public GameObject
{
   typedef void(Tower::*TowerState)(float delta);
   vector<Bullet*> m_VecBullet;
   TowerState m_TowerState = 0;
   Mesh * m_pBulletMesh;
   Mesh * m_pMeshEnemyHealth;
   Enemy * m_TargetEnemy;
   GLuint m_TextureBullet;

   float m_Timer;
   bool m_JustShot;

   float m_TimerBuilding;
   bool m_IsBuilding;
  

public:
	Tower(Mesh* pMesh, ShaderProgram* pShader, GLuint texture, vec2 position, vec2 size, Mesh * a_Bulletmesh, Mesh * a_BulletHealmMesh, GLuint a_BulletTexture);
    virtual ~Tower();
    vec2 m_towerSize;
    void DrawBullets();
    void TowerAddBullet(vec2 TurretPos, Enemy* enemy);
    void Update(float deltatime);
	void SetCamera(Camera* cam);
    void SetTexture(GLuint a_text);
    void SetPosition(float mouseX, float mouseY);
    vec2 GetPosition();
    Tower::TowerState GetTowerState();
    void SetIdle();
    void idle(float delta);
    void Attack(float delta);
    void Building(float delta);
    void SetIsBuilding();
    void StartBuilding();
    bool CheckCollision(Enemy * a_enemy);
    bool Shoot();
   

};
