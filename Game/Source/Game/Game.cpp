 #include "GamePCH.h"
#include "../../Libraries/Framework/Source/Common/spritesheet.h"

Game::Game(Framework* pFramework)
    : GameCore(pFramework)
{
    // Initialization

    m_WindowWidth = -1;
    m_WindowHeight = -1;

	m_TextureWall = 0;
	m_TextureBox = 0;
	m_TextureFloor = 0;
	m_TextureBullet = 0;
	m_TextureEnemy = 0;
	m_TextureTower = 0;

	m_pMeshEnemyHealth = 0;
	m_pMeshEnemy = 0;
	m_pMeshTower = 0;
	m_pBulletMesh = 0;
	m_pSpritesheet = 0;
    m_TextureRed = 0;
    m_TextureGray = 0;
	m_pShaderHealth = 0;
	m_pShader = 0;

    m_SceenSwitch = false;

	m_pSceneInfection = new SceneInfection(pFramework);
    m_pScene = new Scene(pFramework);
}

Game::~Game()
{
    // deleting dynamically allocated data

   // delete m_pScene;
   delete m_pShader;
   delete m_pShaderHealth;

   delete m_pMeshEnemyHealth;
   delete m_pMeshEnemy;
   delete m_pMeshTower;
   delete m_pBulletMesh;
   
    glDeleteTextures(1, &m_TextureGray);
    glDeleteTextures(1, &m_TextureRed);
	glDeleteTextures(1, &m_TextureEnemy);
	glDeleteTextures(1, &m_TextureTower);
	glDeleteTextures(1, &m_TextureBullet);
	glDeleteTextures(1, &m_TextureWall);
	glDeleteTextures(1, &m_TextureFloor);

    while (m_EventQueue.size() != 0)
    {
        Event* pEvent = m_EventQueue.front();
        m_EventQueue.pop();
        delete pEvent;
    }
}

void Game::OnSurfaceChanged(int width, int height)
{
    glViewport(0, 0, width, height);

    m_WindowWidth = width;
    m_WindowHeight = height;
}

void Game::LoadContent()
{
    // Seed random.
    srand((unsigned int)GetSystemTime());

    // Turn on depth buffer testing.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    //LOAD ALL 

	m_pSpritesheet = new SpriteSheet();
	m_pSpritesheet->LoadFromJSONFile("Data/SpriteSheets/", "Zelda.json");

	m_pShaderHealth = new ShaderProgram("Data/Shaders/Healtbar.vert", "Data/Shaders/Healtbar.frag");
	m_pShader = new ShaderProgram("Data/Shaders/texture.vert", "Data/Shaders/texture.frag");

	m_TextureEnemy = LoadTexture("Data/SpriteSheets/Zelda.png");
	m_TextureTower = LoadTexture("Data/Textures/Warp.png");
	m_TextureBullet = LoadTexture("Data/Textures/Ground.png");
	m_TextureFloor = LoadTexture("Data/Textures/Floor.png");
	m_TextureWall = LoadTexture("Data/Textures/Wall.png");
	m_TextureBox = LoadTexture("Data/Textures/Box.png");

    m_TextureGray = LoadTexture("Data/Textures/Gray.png");
    m_TextureRed = LoadTexture("Data/Textures/Red.png");

	m_pMeshEnemy = new Mesh();
	m_pMeshEnemy->GenerateSprite(1.0f, 1.0f);
	m_pMeshTile = new Mesh();

	m_pMeshEnemyHealth = new Mesh();
	m_pMeshTile->GenerateSprite(1.0f, 1.0f);
	m_pMeshEnemyHealth->GenerateSprite(0.9f, 0.25f);

	m_pBulletMesh = new Mesh();
	m_pBulletMesh->GenerateSprite(0.4f, 0.4f);// Turn on alpha blending.

	
	m_pMeshTower = new Mesh();
	m_pMeshTower->GenerateSprite(1.0f, 1.0f);

  
    m_pScene->LoadContent(this);
	m_pSceneInfection->LoadContent(this);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    CheckForGLErrors();
}

void Game::QueueEvent(Event* pEvent)
{
    m_EventQueue.push(pEvent);
}

void Game::ProcessEvents()
{
    while (m_EventQueue.size() != 0)
    {
        Event* pEvent = m_EventQueue.front();
        m_EventQueue.pop();

        OnEvent(pEvent);
        delete pEvent;
    }
}

void Game::OnEvent(Event* pEvent)
{

    //IF m_ScreenSwitch is not true then

    if (!m_SceenSwitch)
    {
        //m_Scene will have events
        m_pScene->HandleEvent(pEvent);
    }
    else
        //m_pSceneInfected will handle event
    {
        m_pSceneInfection->HandleEvent(pEvent);
    }
    //Send m_Controller to on Event
    m_Controllers[0].OnEvent(pEvent);

    if (pEvent->m_EventType == EventType_Input)
    {
        InputEvent* pInputEvent = (InputEvent*)pEvent;
        if (pInputEvent->m_InputDeviceType == InputDeviceType_Keyboard)
        {
            if (pInputEvent->m_InputState == InputState_Pressed)
            {
                if (pInputEvent->m_ID == 'I')
                {
                    m_SceenSwitch = true;
                }
                if (pInputEvent->m_ID == 'O')
                {
                    m_SceenSwitch = false;
                }
            }
        }
    }

}

void Game::Update(float deltatime)
{
    // Process events at the start of the frame before updating anything else in the game.
    ProcessEvents();
    //IF m_ScreenSwitch is not true then
    if (!m_SceenSwitch)
    {
        //m_pScene is update
        m_pScene->Update(deltatime);
    }
    else
    {
        //m_SceneInfected is updated
        m_pSceneInfection->Update(deltatime);
    }
	
}

void Game::Draw()
{
    // Clear the screen to dark blue.
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //IF m_ScreenSwitch is not true then
    if (!m_SceenSwitch)
    {
        //m_pScene is drawn
        m_pScene->Draw();
    }
    else
    {
      //  m_pSceneInfection - is drawn
        m_pSceneInfection->Draw();
    }
    // Draw the scene
    CheckForGLErrors();
}

