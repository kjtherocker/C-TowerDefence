#ifndef __Tilemap_H__
#define __Tilemap_H__

class Game;
class Camera;

enum TileTypes
{
    TileType_Empty,
    TileType_Wall,
    TileType_Box,
    TileType_Tower,
    TileType_NumTypes,
    TileType_Invalid,
};

struct TileInfo
{
    bool m_Walkable;

    Mesh* m_pMesh;
    ShaderProgram* m_pShader;
    GLuint m_TextureHandle;


    void Setup(bool walkable, Mesh* pMesh, ShaderProgram* pShader, GLuint texturehandle)
    {
        m_Walkable = walkable;
        m_pMesh = pMesh;
        m_pShader = pShader;
        m_TextureHandle = texturehandle;
    }
};

class Tilemap
{
protected:
    TileTypes* m_Tiles;
    TileInfo m_TileInfo[TileType_NumTypes];
	Camera * m_pCamera;
	Game * m_pGame;
    vec2 m_TileSize;

    int m_Width;
    int m_Height;

public:
    Tilemap();
    virtual ~Tilemap();
	void SetGamePtr(Game* A_Game);
    void Create(int width, int height);
    void Update(float deltatime);
    void Draw();	
	void SetCamera(Camera * aCamera);
    int GetTileIndexForTilePos(ivec2 tilepos);
    int GetTileIndexForWorldPos(vec2 worldpos);
    ivec2 GetTilePosForTileIndex(int tileindex);
    ivec2 GetTilePosForWorldPos(vec2 worldpos);
    vec2 GetWorldPosForTileIndex(int tileindex);
    vec2 GetWorldPosForTilePos(ivec2 tilepos);

    bool IsTileWalkableForWorldPos(vec2 worldpos);
    bool IsTileWalkableForTilePos(int x, int y);
    bool IsTileWalkableForTilePos(ivec2 tilepos);

    TileInfo* GetTileInfo(int x, int y);
    TileTypes GetTileType(int x, int y);

    void SetTileType(vec2 position, TileTypes type);
    virtual int GetWidth() { return m_Width; }
    virtual int GetHeight() { return m_Height; }
};

#endif //__Tilemap_H__