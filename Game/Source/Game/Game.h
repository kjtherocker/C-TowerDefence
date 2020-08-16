#ifndef __Game_H__
#define __Game_H__

class ShaderProgram;
class SpriteSheet;
class SceneInfection;

class Game : public GameCore
{
    friend class Scene;
    friend class Tilemap;

protected:
    unsigned int m_WindowWidth;
    unsigned int m_WindowHeight;

	SpriteSheet* m_pSpritesheet;

	Mesh* m_pMeshTile;
	Mesh* m_pMeshEnemyHealth;
	Mesh* m_pMeshEnemy;
	Mesh* m_pMeshTower;
	Mesh* m_pBulletMesh;

	GLuint m_TextureFloor;
	GLuint m_TextureWall;
	GLuint m_TextureBox;
	GLuint m_TextureBullet;
	GLuint m_TextureEnemy;
	GLuint m_TextureTower;
    GLuint m_TextureRed;
    GLuint m_TextureGray;

	ShaderProgram* m_pShaderHealth;
	ShaderProgram* m_pShader;
    std::queue<Event*> m_EventQueue;

    Scene* m_pScene;
	SceneInfection * m_pSceneInfection;

    bool m_SceenSwitch; 

public:
    Game(Framework* pFramework);
    virtual ~Game();

    ShaderProgram* GetShader() { return m_pShader; }
	ShaderProgram* GetShaderHealth() { return m_pShaderHealth; }

    Mesh* GetMeshTile() { return m_pMeshTile; }
	Mesh* GetMeshEnemy() { return m_pMeshEnemy; }
	Mesh* GetMeshEnemyHealth() { return m_pMeshEnemyHealth; }
	Mesh* GetMeshTower() { return m_pMeshTower; }
	Mesh* GetMeshBullet() { return m_pBulletMesh; }

	SpriteSheet* GetSpriteSheet() { return m_pSpritesheet; }

    GLuint GetTextureFloor() { return m_TextureFloor; }
    GLuint GetTextureWall() { return m_TextureWall; }
    GLuint GetTextureBox() { return m_TextureBox; }
	GLuint GetTextureTower() { return m_TextureTower; }
	GLuint GetTextureEnemy() { return m_TextureEnemy; }
	GLuint GetTextureBullet() { return m_TextureBullet; }
    GLuint GetTextureGray() { return m_TextureGray; }
    GLuint GetTextureRed() { return m_TextureRed; }

    virtual void OnSurfaceChanged(int width, int height);
    virtual void LoadContent();

    virtual void QueueEvent(Event* pEvent);
    void ProcessEvents();
    void OnEvent(Event* pEvent);

    virtual void Update(float deltatime);
    virtual void Draw();

	PlayerController m_Controllers[4];

};

#endif //__Game_H__