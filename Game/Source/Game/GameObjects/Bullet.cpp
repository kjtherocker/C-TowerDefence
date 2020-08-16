#include "GamePCH.h"



Bullet::Bullet(Mesh* pMesh, ShaderProgram* pShader, GLuint texture, vec2 position, vec2 size)
	: GameObject(pMesh, pShader, texture)
{
    //INTALIZE
	m_Position = position;
	m_BulletSize = size;
	TargetPos = m_Position;
	m_pTargetEnemy = 0;
	m_pCamera = 0;
	m_Radius = BULLETRADIUS;
	m_Timer = BULLET_LIFETIME;
}

Bullet::~Bullet()
{
    //DELETE
}

void Bullet::Update(float deltatime)
{
    if (m_IsLife)
    {
        // if the variable m_TargetPlayer is not equal to nullptr then do what is inside
        if (m_pTargetEnemy != nullptr)
        {
            //Set TargetPos to the pointer m_TargetPlayer and then get the method called //GetPosition()
           TargetPos = m_pTargetEnemy->GetPosition();
         }
        //Set the StartingPosition to the StartingPosition + (the TargetPos - Startingpositon) * //(deltatime/5.0f)
        m_Position = m_Position + (TargetPos - m_Position)*(deltatime * 3);
    }

    //DEREMENT the timer by deltatime
	m_Timer -= deltatime;

    //IF m_Timer is equal or lesser then 0 the
	if (m_Timer <= 0)
	{
        //Destroy this
		this->DestroyIsAlive();
        //RESET the timer to m_Bullet lifetime
		m_Timer = BULLET_LIFETIME;
	}
}

void Bullet::SetCamera(Camera * cam)
{
    //SET m_pCamera to cam argument
	m_pCamera = cam;
}

void Bullet::SetPositionToTurret(Vector2 a_TurretPos)
{
    // SET m_Position to the TurretsPos
	m_Position = a_TurretPos;
}

void Bullet::SetTarget(Enemy * enemy)
{
    //SET the Target enemy to the enemy
	m_pTargetEnemy = enemy;
}

void Bullet::SetTarget(vec2 vec)
{
    //SET the targetpos to the argument
	TargetPos = vec;
}

vec2 Bullet::GetPosition()
{
    //RETURN m_Position
	return m_Position;
}

void Bullet::CheckCollisionWithBulletAndEnemy(Enemy * enemy)
{
    //IF the enemy is alive then
    if (enemy->GetIsAlive())
    {
        //SET radii to enemy->GetRadius() plus this->GetRadius();
        float distance = sqrt(pow(this->GetPosition().x - enemy->GetPosition().x, 2) + pow(this->GetPosition().y - enemy->GetPosition().y, 2));
        //SET radii = Target pointer to the method GetRadius() plus m_Player pointer to the method GetRadius();
        float radii = this->GetRadius() + enemy->GetRadius();

        //IF the radius is greater then the distance then
        if (radii > distance)
        {
            this->DestroyIsAlive();
            enemy->DealDamage(10);
            
        }
    }
	else
	{
        //DESTROY this
		this->DestroyIsAlive();

	}
}




