#include "GamePCH.h"
#include "../../Libraries/Framework/Source/Common/spritesheet.h"

Player::Player(Mesh* pMesh,ShaderProgram* pShader, Mesh* healthmesh, ShaderProgram* pShaderhealth, GLuint texture, vec2 position, vec2 size, Mesh * a_Bulletmesh, Mesh * a_BulletHealmMesh, GLuint a_BulletTexture)
	: GameObject(pMesh, pShader, texture)
{
    //INTIALIZE ALL
    m_pController = 0;

    m_pTilemap = 0;
    m_Position = position;
    m_PlayerSize = size;
	m_pHealthShader = pShaderhealth;
	m_pHealthMesh = healthmesh;
	m_pBulletMesh = a_Bulletmesh;
	m_pMeshEnemyHealth = a_BulletHealmMesh;
	m_TextureBullet = a_BulletTexture;

	m_Red = 1.0f;
	m_Blue = 0.0f;
	m_Green = 0.0f;
	m_Alpha = 1.0f;

	m_WalkingDirection = WalkingDirection::idle;
	m_Radius = PLAYER_RADIUS;
	m_MaxHealth = MAXHEALTH;
	m_Health = m_MaxHealth;
	m_AccumlatedTime = 0;
	m_CurrentFrameNumber = 0;
	vector<Bullet*> m_VecBullet = vector<Bullet*>();
	m_TimerShot = 0;
	m_JustShot = true;
	m_JustHit = true;
}

Player::~Player()
{
    //DELETE ALL
	for (int i = 0; i < (int)m_VecBullet.size(); i++)
	{
		delete  m_VecBullet.at(i);
	}
	m_VecBullet.clear();
}

void Player::SetController(PlayerController* pController)
{
    //SET m_pController to the argument
    m_pController = pController;
}

void Player::SetTilemap(Tilemap* pTilemap)
{
    //SET m_Tilemap to the arguement
    m_pTilemap = pTilemap;
}

void Player::SetSpriteSheet(SpriteSheet * Sprite)
{
    //SET the spritesheet the arguement Sprite
	m_pSpriteSheet = Sprite;
}

void Player::SetCamera(Camera * cam)
{
    //SET Camera to the argument
	m_pCamera = cam;
}

void Player::DrawBullets()
{
    //FOR How many  bullets there are in the vector 
	for (int i = 0; i < (int)m_VecBullet.size(); i++)
	{
        //Draw Bullets
		m_VecBullet.at(i)->Draw();
	}
}

void Player::CheckCollisionBullet(Enemy * enemy, float deltatime)
{
    //If the bool m_IsLIfe is true then
	if (m_IsLife)
	{
        //FOR every bullet in the vector loop through it 
		for (int i = 0; i < (int)m_VecBullet.size(); i++)
		{
            //If there is a bullet that is alive then
			if (m_VecBullet.at(i)->GetIsAlive() == true)
			{
                //CHECK Get the bullets collision with the enemy
				m_VecBullet.at(i)->CheckCollisionWithBulletAndEnemy(enemy);

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

void Player::CheckCollisionPlayer(Enemy * a_Enemy)
{


    //SET distance)to enemy->GetRadius() plus this->GetRadius();
		float distance = sqrt(pow(this->GetPosition().x - a_Enemy->GetPosition().x, 2) + pow(this->GetPosition().y - a_Enemy->GetPosition().y, 2));
        //SET radii = this-pointer to the method GetRadius() plus m_Enemy pointer to the method GetRadius();
        float radii = this->GetRadius() + a_Enemy->GetRadius();
        //If the radius is greater then the distance
		if (radii > distance)
		{
            //IF enemy is alive then
			if (a_Enemy->GetIsAlive() == true)
			{
                //IF the players invencibility frames are off 
				if (PlayerIFrames())
				{
                    //USE DealDamage
					DealDamage(25);
				}
			}
		}
}

void Player::PlayerAddBullets(Vector2 a_CursorPos)
{
    //If Shoot is true then
	if (Shoot())
	{
        //SET bool spawnbullet to true
		bool SpawnBullet = true;
        //For how many bullets that are in the vector loop through
		for (int i = 0; i < (int)m_VecBullet.size(); i++)
		{
            //IF there is a bullet that is alive then
			if (m_VecBullet.at(i)->GetIsAlive() == false)
			{
                //REENABLE
				m_VecBullet.at(i)->SetPositionToTurret(m_Position);
				m_VecBullet.at(i)->SetCamera(m_pCamera);
				m_VecBullet.at(i)->SetTarget(a_CursorPos);
				m_VecBullet.at(i)->SpawnIsALive();
     			SpawnBullet = false;
				break;
			}
		}
        //IF SpawnBullet is true then
		if (SpawnBullet)
		{
            //MAKE A NEW BULLET
			Bullet* m_bullet = new Bullet(m_pBulletMesh, m_pShader, m_TextureBullet, m_Position, vec2(0.5f, 0.5f));
			m_bullet->SetCamera(m_pCamera);
			m_bullet->SetTarget(a_CursorPos);
			m_VecBullet.push_back(m_bullet);
		}
	}
}

void Player::Spawn()
{
    //SET the position to starting positon
	m_Position = Vector2(5.0f, 5.0f);
    //SET the m_Health to MAXHEALTH
	m_Health = MAXHEALTH;
    //SET life to true
	SpawnIsALive();
}

bool Player::WouldPlayerFitOnMap(vec2 worldpos)
{
    vec2 adjustedworldpos;
    ivec2 tilepos;

    // Test bottom left corner.
    adjustedworldpos = worldpos;
    adjustedworldpos.x -= m_PlayerSize.x/2;
    tilepos = m_pTilemap->GetTilePosForWorldPos( adjustedworldpos );
    if( m_pTilemap->IsTileWalkableForTilePos( tilepos.x, tilepos.y ) == false )
        return false;

    // Test bottom right corner.
    adjustedworldpos = worldpos;
    adjustedworldpos.x += m_PlayerSize.x/2;
    tilepos = m_pTilemap->GetTilePosForWorldPos( adjustedworldpos );
    if( m_pTilemap->IsTileWalkableForTilePos( tilepos.x, tilepos.y ) == false )
        return false;

    // Test top left corner.
    adjustedworldpos = worldpos;
    adjustedworldpos.x -= m_PlayerSize.x/2;
    adjustedworldpos.y += m_PlayerSize.y;
    tilepos = m_pTilemap->GetTilePosForWorldPos( adjustedworldpos );
    if( m_pTilemap->IsTileWalkableForTilePos( tilepos.x, tilepos.y ) == false )
        return false;

    // Test top right corner.
    adjustedworldpos = worldpos;
    adjustedworldpos.x += m_PlayerSize.x/2;
    adjustedworldpos.y += m_PlayerSize.y;
    tilepos = m_pTilemap->GetTilePosForWorldPos( adjustedworldpos );
    if( m_pTilemap->IsTileWalkableForTilePos( tilepos.x, tilepos.y ) == false )
        return false;

    return true;
}

bool Player::Shoot()
{
    //If the bool is not true then fire
	if (!m_JustShot)
	{
		m_JustShot = true;
		return true;
	}
	return false;
}

bool Player::PlayerIFrames()
{
    //If the bool is not true then run the timer
	if (!m_JustHit)
	{
		m_JustHit = true;
		return true;
	}
	return false;
}

void Player::Update(float deltatime)
{
    assert( m_pTilemap != 0 );
	assert( m_pSpriteSheet != 0);
    if( m_pController == 0 )
        return;

    //IF the m_Health is lesser then 0 then
	if (m_Health <= 0)
	{
        //DESTROY the player
		DestroyIsAlive();
	}

    //IF JustShot is true then
	if (m_JustShot)
	{
        // DEREMENT TIMER
		m_Timer -= deltatime;
        //IF m_Timer is lesser then 0
		if (m_Timer <= 0)
		{

            //SET m_Timer to Player_Shot_Delay
			m_Timer = PLAYER_SHOT_DELAY;
            //SET JustShot to false
			m_JustShot = false;
		}
	}

	
    //if m_JustHit is true then
	if (m_JustHit)
	{
        //DECREMENT m_TimerShot 
		m_TimerShot -= deltatime;
        //IF m_TimerShot is lesser then 0 then
		if (m_TimerShot <= 0)
		{
            //SET m_TimerShot to Player_HIt_DElay
			m_TimerShot = PLAYER_HIT_DELAY;
			m_JustHit = false;
		}
	}


    //SET speed to 2
    float speed = 2;
    //SET the direction to Vector2(0,0)
    vec2 dir(0,0);

    //SET the Timer
	m_AccumlatedTime += deltatime;

    //IF the Timer is greater then 0.2f Then
	if (m_AccumlatedTime > 0.2f)
    {
        //SET Timer to 0.0f
		m_AccumlatedTime = 0.0f;
        //INCREMENT m_CurrenFrameNumber
		m_CurrentFrameNumber++;
        //If m_CurrentFrameNumber is greater then 1 then

		if (m_CurrentFrameNumber > 1)
		{
            //SET m_CurrentFrameNumber to 0
			m_CurrentFrameNumber = 0;

		}
	}
    //SET the animfilename to nothing
	std::string animfilename = "";

    //SET the actualframe to the CurrentFramenumber
	int actualframe = m_CurrentFrameNumber;
    //IF the Controller sends up
	if (m_pController->IsButtonPressed(PCB_Up))
	{
		m_WalkingDirection = WalkingDirection::up;
		dir.y = 1 * speed * deltatime;
	}
    //IF the Controller sends down
	if (m_pController->IsButtonPressed(PCB_Down))
	{
		m_WalkingDirection = WalkingDirection::down;
		dir.y = -1 * speed * deltatime;
	}
    //IF the Controller sends Left
	if (m_pController->IsButtonPressed(PCB_Left))
	{
		m_WalkingDirection = WalkingDirection::left;
		dir.x = -1 * speed * deltatime;
	}
    //IF the Controller sends Right
	if (m_pController->IsButtonPressed(PCB_Right))
	{
		m_WalkingDirection = WalkingDirection::right;
		dir.x = 1 * speed * deltatime;
	}

	if (m_WalkingDirection == WalkingDirection::right)
	{
        //SET animation frame;
		animfilename = "LinkWalkRight" + std::to_string(actualframe + 1);
	}
	  if (m_WalkingDirection == WalkingDirection::left)
	{
          //SET animation frame;
		animfilename = "LinkWalkLeft" + std::to_string(actualframe + 1);
	}
	  if (m_WalkingDirection == WalkingDirection::down)
	{
          //SET animation frame;
		animfilename = "LinkWalkDown" + std::to_string(actualframe + 1);
	}
	 if (m_WalkingDirection == WalkingDirection::up)
	{
         //SET animation frame;
		animfilename = "LinkWalkUp" + std::to_string(actualframe + 1);
	}	
	 if (m_WalkingDirection == WalkingDirection::idle)
	{
         //SET animation frame;
		 animfilename = "LinkWalkDown1";
	}

    // Check for collisions and set dir to 0 if we can't move.
    {
        // If we can't fit horizontally, zero out dir x.
        if( WouldPlayerFitOnMap( m_Position + vec2( dir.x, 0.0f ) ) == false )
        {
            dir.x = 0;
        }

        // If we can't fit vertically, zero out dir y.
        if( WouldPlayerFitOnMap( m_Position + vec2( 0.0f, dir.y ) ) == false )
        {
            dir.y = 0;
        }
    } 

    animfilename += ".png";

	SpriteInfo* pInfo = m_pSpriteSheet->GetSpriteInfoByFilename(animfilename);
	vec2 texsize = m_pSpriteSheet->GetTextureSize();

	m_UVScale.Set(pInfo->m_Size.x / (float)texsize.x, pInfo->m_Size.y / (float)texsize.y);
	m_UVoffset.Set(pInfo->m_Offset.x / texsize.x, pInfo->m_Offset.y / texsize.y);



    // Move the player
    m_Position += dir;
}
