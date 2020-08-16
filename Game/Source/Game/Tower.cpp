#include "GamePCH.h"

Tower::Tower(Mesh* pMesh,ShaderProgram* pShader, GLuint texture, vec2 position, vec2 size, Mesh * a_Bulletmesh, Mesh * a_BulletHealmMesh, GLuint a_BulletTexture)
	: GameObject(pMesh, pShader, texture)
{
    //INTALIZING everything 
	m_pBulletMesh = a_Bulletmesh;
	m_pMeshEnemyHealth = a_BulletHealmMesh;
	m_TextureBullet = a_BulletTexture;
    m_Texture = texture;

	m_Position = position;
	m_towerSize = size;
	m_pCamera = 0;
	m_Radius = TOWERRADIUS;
	m_Timer = SHOT_DELAY;


	vector<Bullet*> m_VecBullet = vector<Bullet*>();


    m_TimerBuilding = TOWER_BUILD_TIME;
    m_IsBuilding = true;


	m_TowerState = (TowerState)&Tower::Building;
	(this->*m_TowerState)(0);


   


    m_JustShot = false;
    m_TargetEnemy = 0;
}

Tower::~Tower()
{
    //DELETE everything
    for (int i = 0; i < (int)m_VecBullet.size(); i++)
    {
        delete  m_VecBullet.at(i);
    }
	m_VecBullet.clear();


	glDeleteTextures(1, &m_TextureBullet);
	glDeleteTextures(1, &m_Texture);
	 m_TowerState = nullptr;
}

bool Tower::Shoot()
{

    //If the bool is not true then fire
    if (!m_JustShot)
    {
        m_JustShot = true;
        return true;
    }
    return false;
}



void Tower::SetIdle()
{
    //SET the tower state to idle
    m_TowerState = &Tower::idle;
}

void Tower::idle(float delta)
{
    //Do nothing
}

void Tower::Attack(float delta)
{
    //if the bool m_JustShot is true then
   if (m_JustShot)
   {
       //m_timer will be minused by delta
       m_Timer -= delta;
       //if the m_Timer is less then 0 then
       if (m_Timer <= 0)
       {
           //SET the timer back to the delay
           m_Timer = SHOT_DELAY;
           //SET the just shot to false
           m_JustShot = false;
       }
   }
 
}

void Tower::Building(float delta)
{
    //IF the bool m_IsBuilding is true
    if (m_IsBuilding)
    {
        //m_TimerBuilding is now minused by delta
        m_TimerBuilding -= delta;
        //if the m_Timer building is less then 0 then
        if (m_TimerBuilding <= 0)
        {
            //RESET the  m_TimerBuilding to the building time;
            m_TimerBuilding = TOWER_BUILD_TIME;
            //SET the state of  m_TowerState to idle;
            m_TowerState = &Tower::idle;
            //Set m_IsBuilding to false;
            m_IsBuilding = false;
        }
    }
}

void Tower::SetIsBuilding()
{
    //SET the m_TowerState to Building;
    m_TowerState = &Tower::Building;
}

void Tower::StartBuilding()
{
    //SET the bool to true
   m_IsBuilding = true;
}

void Tower::Update(float deltatime)
{
    //If the bool m_IsLIfe is true then
	if (m_IsLife)
	{
        //Update the m_TowerState
		(this->*m_TowerState)(deltatime);
        //FOR every bullet in the vector loop through it 
		for (int i = 0; i < (int)m_VecBullet.size(); i++)
		{
            // IF there is a bullet that is alive then
			if (m_VecBullet.at(i)->GetIsAlive() == true)
			{
                //CHECK Get the bullets collision with the enemy
				m_VecBullet.at(i)->CheckCollisionWithBulletAndEnemy(m_TargetEnemy);

			}
            //SET the bullets update to deltatime
			m_VecBullet.at(i)->Update(deltatime);

		}
	}
	else
	{
        //FOR every bullet in the vector loop through it 
		for (int i = 0; i < (int)m_VecBullet.size(); i++)
		{
            //DESTROY all the bullet
				m_VecBullet.at(i)->DestroyIsAlive();
		}
	}
}

void Tower::DrawBullets()
{
    //FOR every bullet in the vector loop through it 
    for (int i = 0; i < (int)m_VecBullet.size(); i++)
    {
        //DRAW all the bullets
        m_VecBullet.at(i)->Draw();
    }
}

void Tower::TowerAddBullet(vec2 TurretPos, Enemy* enemy)
{
    // Set the SpawnBullet bool to true
    bool SpawnBullet = true;

    //FOR every bullet in the vector loop through it 
    for (int i = 0; i < (int)m_VecBullet.size(); i++)
    {
        //if there is any bullets in the vector that are dead then
        if (m_VecBullet.at(i)->GetIsAlive() == false)
        {
            //RENABLE the bullet
            m_VecBullet.at(i)->SetPositionToTurret(TurretPos);
            m_VecBullet.at(i)->SetCamera(m_pCamera);
            m_VecBullet.at(i)->SetTarget(enemy);
            m_VecBullet.at(i)->SpawnIsALive();
            SpawnBullet = false;
            m_TargetEnemy = enemy;
            break;
        }
    }
   // if the bool SpawnBullet is true then
    if (SpawnBullet)
    {
        //SPAWN a new bullet
        Bullet* m_bullet = new Bullet(m_pBulletMesh,  m_pShader, m_TextureBullet, TurretPos, vec2(0.5f, 0.5f));
        m_bullet->SetCamera(m_pCamera);
        m_bullet->SetTarget(enemy);
        m_VecBullet.push_back(m_bullet);
        m_TargetEnemy = enemy;
    }
   
}

void Tower::SetCamera(Camera * cam)
{
    //SET m_pCamera to the argument cam
	m_pCamera = cam;
}

void Tower::SetTexture(GLuint a_text)
{
    //Set the texture to what is needed
    m_Texture = a_text;
}

void Tower::SetPosition(float mouseX, float mouseY)
{
    //SET the position to the arguments
    m_Position = Vector2(mouseX, mouseY);

}

vec2 Tower::GetPosition()
{
    //RETURN the position
	return m_Position;
}

Tower::TowerState Tower::GetTowerState()
{
    //RETURN towestate
    return Tower::TowerState(m_TowerState);
}

bool Tower::CheckCollision(Enemy * a_enemy)
{
    
    //SET radii to this>GetRadius() plus m_VecEnemys.at(i)->GetRadius();
        float distance = sqrt(pow(this->GetPosition().x - a_enemy->GetPosition().x, 2) + pow(this->GetPosition().y - a_enemy->GetPosition().y, 2));
        //SET radii = thispointer to the method GetRadius() plus a_enemy to the method GetRadius();
        float radii = this->GetRadius() + a_enemy->GetRadius();

        //IF the radius and distance overlap then
        if (radii > distance)
        {
            //IF the enemy is alive then
            if (a_enemy->GetIsAlive())
            {
                //SET The tower state to Attack
                m_TowerState = &Tower::Attack;
                //IF the Shoot method returns true then
                if (Shoot())
                {
                    //Add bullets from the turrets position and the target is the a_enemy
                    TowerAddBullet(this->GetPosition(), a_enemy);
                }
                return true;
            }
        }
        return false;
    
}
