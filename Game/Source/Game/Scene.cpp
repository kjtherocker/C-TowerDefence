#include "GamePCH.h"
#include "Tower.h"

Scene::Scene(Framework * a_pFramework)
{
    // Initialization 


    m_Rows = 12;
    m_Columns = 16;
    m_pScore = 0;

    m_pSpritesheet = 0;
    
    m_pTilemap = 0;
    m_pTilemap1 = 0;
    m_pTilemap2 = 0;
    m_pTilemap3 = 0;
    m_pTilemap4 = 0;

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
    m_TextureCursor = 0;
    m_TextureTower = 0;
    m_TextureBullet = 0;
    m_TextureCursor = 0;
    m_TextureWall = 0;
    m_TextureGround = 0;
    m_TextureRed = 0;
    m_TextureGray = 0;

    m_EnemySpawn = true;
    m_EnemyTimer = 0;

    SCREEN_WIDTH = WINDOWSIZE / m_Columns;
    SCREEN_HEIGHT = WINDOWSIZE / m_Rows;

    m_EnemyTimer = ENEMYSPAWNRATE;

    m_Tile = 0;

    vector<Tower*> m_VecTowers = vector<Tower*>();
    vector<Enemy*> m_VecEnemy = vector<Enemy*>();

    //	AddTower();
}

Scene::~Scene()
{
 
    delete m_pScore;
    delete m_pTilemap;
    delete m_pTilemap1;
    delete m_pTilemap2;
    delete m_pTilemap3;
    delete m_pTilemap4;

    delete m_pTowerCursor;

    delete m_pPathfinder1;
    delete m_pPathfinder2;
    delete m_pPathfinder3;
    delete m_pPathfinder4;


    delete m_pMeshEnemyHealth;
    delete m_pBulletMesh;
    delete m_pMeshEnemy;
    delete m_pSpritesheet;


    for (int i = 0; i < (int)m_VecEnemy.size(); i++)
    {
        delete  m_VecEnemy.at(i);
    }
    m_VecEnemy.clear();

    for (int i = 0; i < (int)m_VecTowers.size(); i++)
    {
        delete  m_VecTowers.at(i);
    }
    m_VecTowers.clear();


    glDeleteTextures(1, &m_TextureRed);
    glDeleteTextures(1, &m_TextureGray);

    glDeleteTextures(1, &m_TextureEnemy);
    glDeleteTextures(1, &m_TextureTower);
    glDeleteTextures(1, &m_TextureBullet);
    glDeleteTextures(1, &m_TextureWall);
    glDeleteTextures(1, &m_TextureGround);
    glDeleteTextures(1, &m_TextureCursor);
}

void Scene::LoadContent(Game* pGame)
{
    // Create our shaders.
    m_pShader = pGame->GetShader();
    m_pShaderHealth = pGame->GetShaderHealth();

    SCREEN_WIDTH = WINDOWSIZE / m_Columns;
    SCREEN_HEIGHT = WINDOWSIZE / m_Rows;

    m_TextureRed = pGame->GetTextureRed();
    m_TextureGray = pGame->GetTextureGray();

	m_pSpritesheet = pGame->GetSpriteSheet();
 
    // Create our meshes.  Sprite pivot is set to bottom center.
    m_pMeshEnemy = pGame->GetMeshEnemy();

    m_pMeshEnemyHealth = pGame->GetMeshEnemyHealth();

    m_pBulletMesh = pGame->GetMeshBullet();

    m_pMeshTower = pGame->GetMeshTower();

    // Load our textures.
	m_TextureEnemy = pGame->GetTextureEnemy();
	m_TextureTower = pGame->GetTextureTower();
	m_TextureBullet = pGame->GetTextureBullet();
    m_TextureWall = pGame->GetTextureWall();
    m_TextureGround = pGame->GetTextureFloor();
    m_TextureCursor = m_TextureTower;

    m_pCamera = new Camera();


    m_pTilemap1 = new Tilemap();
    m_pTilemap1->SetGamePtr(pGame);
    m_pTilemap1->Create(16, 12);
    m_pTilemap1->SetCamera(m_pCamera);

    m_pTilemap2 = new Tilemap();
    m_pTilemap2->SetGamePtr(pGame);
    m_pTilemap2->Create(18, 14);
    m_pTilemap2->SetCamera(m_pCamera);

    m_pTilemap3 = new Tilemap();
    m_pTilemap3->SetGamePtr(pGame);
    m_pTilemap3->Create(20, 16);
    m_pTilemap3->SetCamera(m_pCamera);

    m_pTilemap4 = new Tilemap();
    m_pTilemap4->SetGamePtr(pGame);
    m_pTilemap4->Create(22, 18);
    m_pTilemap4->SetCamera(m_pCamera);



    m_pTilemap = m_pTilemap1;

    m_pScore = new Score();
    m_pScore->SetCamera(m_pCamera);

    m_pTowerCursor = new Tower(m_pMeshTower, m_pShader, m_TextureCursor, vec2(3, 3), vec2(0.5f, 0.5f), m_pBulletMesh, m_pBulletMesh, m_TextureBullet);
    m_pTowerCursor->SetCamera(m_pCamera);



    m_pPathfinder1 = new Pathfinder(m_pTilemap1, 16, 12);
    m_pPathfinder2 = new Pathfinder(m_pTilemap2, 17, 13);
    m_pPathfinder3 = new Pathfinder(m_pTilemap3, 20, 16);
    m_pPathfinder4 = new Pathfinder(m_pTilemap4, 22, 18);
    m_pPathfinder = m_pPathfinder1;


    SceneAddEnemy();
}

void Scene::Update(float deltatime)
{
    SCREEN_WIDTH = WINDOWSIZE / m_Columns;
    SCREEN_HEIGHT = WINDOWSIZE / m_Rows;


    m_pTowerCursor->Update(deltatime);
    m_pScore->Update(deltatime);

    for (int i = 0; i < (int)m_VecTowers.size(); i++)
    {
        m_VecTowers.at(i)->Update(deltatime);
    }
    for (int i = 0; i < (int)m_VecEnemy.size(); i++)
    {
        m_VecEnemy.at(i)->Update(deltatime);
    }
    // if the mouse is clicked, take the return value of these 2 functions
    // and do math to them to convert them into world space. 

    m_pFramework->GetMouseCoordinates(&m_tilexindex, &m_tileyindex);

    m_tilexindex /= SCREEN_WIDTH;

    m_tileyindex = (WINDOWSIZE - m_tileyindex) / SCREEN_HEIGHT;

    m_pTowerCursor->SetPosition((float)m_tilexindex, (float)m_tileyindex);


    for (int i = 0; i < (int)m_VecTowers.size(); i++)
    {
        if (m_VecTowers.at(i)->GetTowerState() == &Tower::idle)
        {
            m_VecTowers.at(i)->SetTexture(m_TextureGray);
        }
        if (m_VecTowers.at(i)->GetTowerState() == &Tower::Building)
        {
            m_VecTowers.at(i)->SetTexture(m_TextureRed);
        }
        if (m_VecTowers.at(i)->GetTowerState() == &Tower::Attack)
        {
            m_VecTowers.at(i)->SetTexture(m_TextureTower);
        }

    }



    for (int i = 0; i < (int)m_VecTowers.size(); i++)
    {
        if (m_VecTowers.at(i)->GetTowerState() != &Tower::Building)
        {

            bool foundSomebody = false;
            for (int j = 0; j < (int)m_VecEnemy.size(); j++)
            {
                if (m_VecTowers.at(i)->CheckCollision(m_VecEnemy.at(j)))
                {
                    foundSomebody = true;
                    break;
                }
            }
        
           if (!foundSomebody)
           {
            m_VecTowers.at(i)->SetIdle();
           }
        }
    }


    if (m_Tile == AMOUNTOFTILESTYPES)
    {
        m_Tile = 0;
    }

    m_EnemyTimer -= deltatime;
    if (m_EnemyTimer <= 0)
    {
        m_EnemyTimer = ENEMYSPAWNRATE;
        SceneAddEnemy();
    }


    if (m_Tile == 0)
    {
        m_pTowerCursor->SetTexture(m_TextureTower);
    }
    else if (m_Tile == 1)
    {
        m_pTowerCursor->SetTexture(m_TextureWall);
    }
    else if (m_Tile == 2)
    {
        m_pTowerCursor->SetTexture(m_TextureGround);
    }

    for (int i = 0; i < (int)m_VecEnemy.size(); i++)
    {
        m_VecEnemy.at(i)->SetPathfinder(m_Columns, m_Rows,Vector2(0.0f,0.0f),m_pPathfinder);
    }

}

void Scene::SceneAddEnemy()
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
            m_VecEnemy.at(i)->SetPathfinder(m_Columns, m_Rows,  Vector2(0.0f, 0.0f), m_pPathfinder);
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
        m_pEnemy->SetPathfinder(m_Columns, m_Rows, Vector2(0.0f, 0.0f), m_pPathfinder);
        m_VecEnemy.push_back(m_pEnemy);
    }


}

void Scene::Draw()
{
    m_pTilemap->Draw();

    for (int i = 0; i < (int)m_VecTowers.size(); i++)
    {

        m_VecTowers.at(i)->Draw();
    }
    for (int i = 0; i < (int)m_VecTowers.size(); i++)
    {

        m_VecTowers.at(i)->DrawBullets();
    }
    for (int i = 0; i < (int)m_VecEnemy.size(); i++)
    {

        m_VecEnemy.at(i)->Draw();
    }

    m_pTowerCursor->Draw();
    m_pScore->Draw();


}

void Scene::Reset()
{
    for (int i = 0; i < m_Columns; i++)
    {
        for (int j = 0; j < m_Rows; j++)
        {
            m_pTilemap->SetTileType(Vector2((float)i, (float)j), TileType_Empty);
        }

    }

    for (int i = 0; i < (int)m_VecTowers.size(); i++)
    {
        m_pTilemap->SetTileType(m_VecTowers.at(i)->GetPosition(), TileType_Empty);
    }
    m_VecTowers.clear();
    m_VecEnemy.clear();
}


void Scene::AddTower()
{
    bool spottaken = false;
    for (int i = 0; i < (int)m_VecTowers.size(); i++)
    {
        if (m_VecTowers.at(i)->GetPosition() == Vector2((float)m_tilexindex, (float)m_tileyindex))
        {
            spottaken = true;
            break;

        }
    }
    if (!spottaken)
    {
        bool willBlock = false;
        //	INITIALIZING an  Tower pointer called e 
        for (int j = 0; j < (int)m_VecEnemy.size(); j++)
        {
            m_pTilemap->SetTileType(Vector2((float)m_tilexindex, (float)m_tileyindex), TileType_Tower);
            m_VecEnemy.at(j)->SetPathfinder(m_Columns, m_Rows, Vector2(0.0f, 0.0f), m_pPathfinder);

            if (!m_VecEnemy.at(j)->GetFound())
            {
                m_pTilemap->SetTileType(Vector2((float)m_tilexindex, (float)m_tileyindex), TileType_Empty);
                m_VecEnemy.at(j)->SetPathfinder(m_Columns, m_Rows, Vector2(0.0f, 0.0f), m_pPathfinder);
                willBlock = true;
                break;
            }
        }
        if (!willBlock)
        {
            bool SpawnEnemy = true;

            for (int i = 0; i < (int)m_VecTowers.size(); i++)
            {
                if (m_VecTowers.at(i)->GetIsAlive() == false)
                {

                    m_VecTowers.at(i)->SpawnIsALive();
                    m_VecTowers.at(i)->SetCamera(m_pCamera);
                    m_VecTowers.at(i)->SetPosition((float)m_tilexindex, (float)m_tileyindex);
                    m_VecTowers.at(i)->StartBuilding();
                    m_VecTowers.at(i)->SetIsBuilding();
                    SpawnEnemy = false;

                    break;
                }
            }
            if (SpawnEnemy)
            {

                Tower * m_ptower = new Tower(m_pMeshTower, m_pShader, m_TextureCursor, vec2(3, 3), vec2(0.5f, 0.5f), m_pBulletMesh, m_pBulletMesh, m_TextureBullet);
                m_ptower->SetCamera(m_pCamera);
                m_ptower->SetPosition((float)m_tilexindex, (float)m_tileyindex);
                m_VecTowers.push_back(m_ptower);
            }
        }
    }
}

void Scene::AddTileType(TileTypes tile)
{
    for (int j = 0; j < (int)m_VecEnemy.size(); j++)
    {
        for (int i = 0; i < (int)m_VecTowers.size(); i++)
        {
            if (m_VecTowers.at(i)->GetPosition() == Vector2((float)m_tilexindex, (float)m_tileyindex))
            {
                m_VecTowers.at(i)->DestroyIsAlive();
            }
        }

        m_pTilemap->SetTileType(Vector2((float)m_tilexindex, (float)m_tileyindex), tile);
        m_VecEnemy.at(j)->SetPathfinder(m_Columns, m_Rows, Vector2(0.0f, 0.0f), m_pPathfinder);
        if (m_VecEnemy.at(j)->GetFound())
        {
            m_pTilemap->SetTileType(Vector2((float)m_tilexindex, (float)m_tileyindex), tile);
        }
        else
        {
            m_pTilemap->SetTileType(Vector2((float)m_tilexindex, (float)m_tileyindex), TileType_Empty);
        }

    }
}

void Scene::ChangeTile()
{
    if (m_Tile == 0)
    {
        AddTower();
    }
    if (m_Tile == 1)
    {
        AddTileType(TileType_Wall);
    }
    if (m_Tile == 2)
    {
        AddTileType(TileType_Empty);
    }

}


void Scene::HandleEvent(Event * aeven)
{
    if (aeven->m_EventType == EventType_Input)
    {
        InputEvent* pInputEvent = (InputEvent*)aeven;
        if (pInputEvent->m_InputDeviceType == InputDeviceType_Keyboard)
        {
            if (pInputEvent->m_InputState == InputState_Pressed)
            {
                if (pInputEvent->m_ID == '1')
                {
                    m_pCamera->SetCameraTranslation(vec2(18 / 2.0f - 0.5f, 14.0f / 2.0f));
                    m_pCamera->SetCameraScale(vec2(1.0f / (18 / 2), 1.0f / (14 / 2)));
                    m_Columns = 18;
                    m_Rows = 14;
         
                    m_pTilemap = m_pTilemap2;
                    m_pPathfinder = m_pPathfinder2;
                    Reset();
                }
                if (pInputEvent->m_ID == '2')
                {
                    m_pCamera->SetCameraTranslation(vec2(20 / 2.0f - 0.5f, 16 / 2.0f));
                    m_pCamera->SetCameraScale(vec2(1.0f / (20 / 2), 1.0f / (16 / 2)));
                    m_Columns = 20;
                    m_Rows = 16;
                    
                    m_pTilemap = m_pTilemap3;
                    m_pPathfinder = m_pPathfinder3;
                    Reset();
                }
                if (pInputEvent->m_ID == '3')
                {
                    m_pCamera->SetCameraTranslation(vec2(22 / 2.0f - 0.5f, 18 / 2.0f));
                    m_pCamera->SetCameraScale(vec2(1.0f / (22 / 2), 1.0f / (18 / 2)));
                    m_Columns = 22;
                    m_Rows = 18;
                    
                    m_pTilemap = m_pTilemap4;
                    m_pPathfinder = m_pPathfinder4;
                    Reset();
                }
                if (pInputEvent->m_ID == '4')
                {
                    m_pCamera->SetCameraTranslation(vec2(16 / 2.0f - 0.5f, 12 / 2.0f));
                    m_pCamera->SetCameraScale(vec2(1.0f / (16 / 2), 1.0f / (12 / 2)));
                    m_Columns = 16;
                    m_Rows = 12;
                    
                    m_pTilemap = m_pTilemap1;
                    m_pPathfinder = m_pPathfinder1;
                    Reset();
                }
                if (pInputEvent->m_ID == 'Z')
                {
					m_Tile++;
                }
            }
        }
        if (pInputEvent->m_InputDeviceType == InputDeviceType_Mouse)
        {
            if (pInputEvent->m_InputState == InputState_Pressed)
            {
                if (pInputEvent->m_ID == 0)
                {
                    ChangeTile();

                }
                if (pInputEvent->m_ID == 1)
                {
                    m_Tile++;
                }
            }
        }
    }


}

