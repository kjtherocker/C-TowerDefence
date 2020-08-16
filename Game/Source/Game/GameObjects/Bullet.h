#pragma once
class Camera;
class Enemy;

class Bullet : public GameObject
{
public:
	Bullet(Mesh* pMesh, ShaderProgram* pShader, GLuint texture, vec2 position, vec2 size);
	virtual ~Bullet();

	void Update(float deltatime);
	void SetCamera(Camera* cam);

	void SetPositionToTurret(Vector2 a_TurretPos);
	void SetTarget(Enemy* enemy);
	void SetTarget(vec2 vec);
	vec2 GetPosition();
    void CheckCollisionWithBulletAndEnemy(Enemy * enemy);
    
protected:
	Enemy* m_pTargetEnemy;
	vec2 m_BulletSize;
	vec2 TargetPos;
	float m_Timer;



};