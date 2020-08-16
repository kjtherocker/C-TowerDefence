#include "GamePCH.h"

SceneInfection::SceneInfection(Framework * a_pFramework)
{
	// Initialization 


	m_Rows = 12;
	m_Columns = 16;


	m_pSpritesheet = 0;
	m_pTilemap = 0;
	m_pTowerCursor = 0;
	m_pPathfinder = 0;

	m_TextureEnemy = 0;
	m_TextureTower = 0;

	m_pShaderHealth = 0;
	m_pShader = 0;

	m_pMeshEnemy = 0;
	m_pMeshEnemyHealth = 0;
	m_pBulletMesh = 0;


	m_pFramework = a_pFramework;

	m_tilexindex = 0;
	m_tileyindex = 0;
	m_TextureBullet = 0;
	m_TextureWall = 0;
	m_TextureGround = 0;

	m_EnemySpawn = true;
	m_EnemyTimer = 0;

	m_pPlayer = 0;

	SCREEN_WIDTH = WINDOWSIZE / m_Columns;
	SCREEN_HEIGHT = WINDOWSIZE / m_Rows;

	m_EnemyTimer = ENEMYSPAWNRATE;

	m_Tile = 0;

	vector<Enemy*> m_VecEnemy = vector<Enemy*>();
}

SceneInfection::~SceneInfection()
{

	delete m_pTilemap;
	delete m_pTowerCursor;

    delete m_pPlayer;
	delete m_pPathfinder;
    delete m_pMeshEnemyHealth;
    delete m_pBulletMesh;
    delete m_pMeshEnemy;
    delete m_pSpritesheet;


	for (int i = 0; i < (int)m_VecEnemy.size(); i++)
	{
		delete  m_VecEnemy.at(i);
	}
	m_VecEnemy.clear();


	glDeleteTextures(1, &m_TextureEnemy);
	glDeleteTextures(1, &m_TextureBullet);
	glDeleteTextures(1, &m_TextureWall);
	glDeleteTextures(1, &m_TextureGround);
	glDeleteTextures(1, &m_TextureCursor);
}

void SceneInfection::LoadContent(Game* pGame)
{
	// Create our shaders.
	m_pShader = pGame->GetShader();
	m_pShaderHealth = pGame->GetShaderHealth();

	SCREEN_WIDTH = WINDOWSIZE / m_Columns;
	SCREEN_HEIGHT = WINDOWSIZE / m_Rows;


	m_pSpritesheet = pGame->GetSpriteSheet();

	// Create our meshes.  Sprite pivot is set to bottom center.
	m_pMeshEnemy = pGame->GetMeshEnemy();

	m_pMeshEnemyHealth  = pGame->GetMeshEnemyHealth();

	m_pBulletMesh = pGame->GetMeshBullet();


	// Load our textures.
	m_TextureEnemy = pGame->GetTextureEnemy();
	m_TextureBullet = pGame->GetTextureBullet();
	m_TextureWall = pGame->GetTextureWall();
	m_TextureGround = pGame->GetTextureFloor();

	m_pCamera = new Camera();

	m_pTilemap = new Tilemap();
	m_pTilemap->SetGamePtr(pGame);
	m_pTilemap->Create(16, 12);
	m_pTilemap->SetCamera(m_pCamera);

	m_pPathfinder = new Pathfinder(m_pTilemap, 16, 12);

	m_pPlayer = new Player(m_pMeshEnemy, m_pShader, m_pMeshEnemyHealth, m_pShaderHealth, m_TextureEnemy, vec2(3, 3), vec2(0.5f, 0.5f), m_pBulletMesh, m_pBulletMesh, m_TextureBullet);
	m_pPlayer->SetSpriteSheet(m_pSpritesheet);
	m_pPlayer->SetTilemap(m_pTilemap);
	m_pPlayer->SetCamera(m_pCamera);
	m_pPlayer->SetController(&pGame->m_Controllers[0]);
	SceneInfectionAddEnemy();
}

void SceneInfection::Update(float deltatime)
{
	for (int i = 0; i < (int)m_VecEnemy.size(); i++)
	{
		m_VecEnemy.at(i)->Update(deltatime);
	}
	// if the mouse is clicked, take the return value of these 2 functions
	// and do math to them to convert them into world space. 
	SCREEN_WIDTH = WINDOWSIZE / m_Columns;
	SCREEN_HEIGHT = WINDOWSIZE / m_Rows;

	m_pFramework->GetMouseCoordinates(&m_tilexindex, &m_tileyindex);

	m_tilexindex /= SCREEN_WIDTH;

	m_tileyindex = (WINDOWSIZE - m_tileyindex) / SCREEN_HEIGHT;

	

	m_EnemyTimer -= deltatime;
	if (m_EnemyTimer <= 0)
	{
		m_EnemyTimer = INFECTEDENEMYSPAWNRATE;
		SceneInfectionAddEnemy();
	}


	for (int i = 0; i < (int)m_VecEnemy.size(); i++)
	{
		m_VecEnemy.at(i)->SetPathfinder(m_Columns, m_Rows, m_pPlayer->GetPosition(), m_pPathfinder);

	}	
	
	m_pPlayer->Update(deltatime);

	for (int j = 0; j < (int)m_VecEnemy.size(); j++)
	{
		m_pPlayer->CheckCollisionBullet(m_VecEnemy.at(j), deltatime);
	}
	for (int j = 0; j < (int)m_VecEnemy.size(); j++)
	{
		m_pPlayer->CheckCollisionPlayer(m_VecEnemy.at(j));
	}

	if (m_pPlayer->GetIsAlive() == false)
	{
		Reset();
	}
	

}

void SceneInfection::SceneInfectionAddEnemy()
{
	bool SpawnEnemy = true;
	for (int i = 0; i < (int)m_VecEnemy.size(); i++)
	{
		if (m_VecEnemy.at(i)->GetIsAlive() == false)
		{

			m_VecEnemy.at(i)->SetTilemap(m_pTilemap);
			m_VecEnemy.at(i)->SetCamera(m_pCamera);
			m_VecEnemy.at(i)->SpawnIsALive();
			m_VecEnemy.at(i)->SetSpriteSheet(m_pSpritesheet);
			m_VecEnemy.at(i)->Spawn();
			m_VecEnemy.at(i)->SetPathfinder(m_Columns, m_Rows, m_pPlayer->GetPosition(), m_pPathfinder);
			SpawnEnemy = false;

			break;
		}
	}
	if (SpawnEnemy)
	{

		Enemy* m_pEnemy = new Enemy(m_pMeshEnemy, m_pMeshEnemyHealth, m_pShaderHealth, m_pShader, m_TextureEnemy, vec2(1, 1), vec2(0.5f, 0.5f));
		m_pEnemy->SetTilemap(m_pTilemap);
		m_pEnemy->SetCamera(m_pCamera);
		m_pEnemy->SetSpriteSheet(m_pSpritesheet);
		m_pEnemy->SetPathfinder(m_Columns, m_Rows, m_pPlayer->GetPosition(), m_pPathfinder);
		m_VecEnemy.push_back(m_pEnemy);
	}


}


void SceneInfection::Draw()
{
	m_pTilemap->Draw();

	for (int i = 0; i < (int)m_VecEnemy.size(); i++)
	{

		m_VecEnemy.at(i)->Draw();
	}

	m_pPlayer->DrawBullets();
	m_pPlayer->Draw();

}

void SceneInfection::Reset()
{
	m_pPlayer->Spawn();
	m_VecEnemy.clear();
}




void SceneInfection::HandleEvent(Event * aeven)
{
	if (aeven->m_EventType == EventType_Input)
	{
		InputEvent* pInputEvent = (InputEvent*)aeven;
		if (pInputEvent->m_InputDeviceType == InputDeviceType_Mouse)
		{
			if (pInputEvent->m_InputState == InputState_Pressed)
			{
				if (pInputEvent->m_ID == 0)
				{
					m_pPlayer->PlayerAddBullets(Vector2((float)m_tilexindex, (float)m_tileyindex));
				}
			}
		}
	}


}

