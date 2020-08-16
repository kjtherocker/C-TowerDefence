#include "GamePCH.h"

Tilemap::Tilemap()
{
    m_Tiles = 0;
	m_pCamera = 0;
    m_TileSize.Set( 1, 1 );
	m_pGame = 0;
    m_Width = 0;
    m_Height = 0;
}

Tilemap::~Tilemap()
{
    delete m_Tiles;
}

void Tilemap::SetGamePtr(Game * A_Game)
{
	m_pGame = A_Game;
}

void Tilemap::Create( int width, int height)
{
    assert( width > 0 && height > 0 );

    m_Width = width;
    m_Height = height;

    m_Tiles = new TileTypes[m_Width*m_Height];
    
   //  Make the "classic" map with code rather than a hand-made array
    for( int y=0; y<m_Height; y++ )
    {
        for( int x=0; x<m_Width; x++ )
        {
            int index = y*m_Width + x;

            m_Tiles[index] = TileType_Empty;
        }
    }  

    m_TileInfo[TileType_Empty].Setup( true, m_pGame->GetMeshTile(), m_pGame->GetShader(), m_pGame->GetTextureFloor() );
    m_TileInfo[TileType_Tower].Setup( false, m_pGame->GetMeshTile(), m_pGame->GetShader(), m_pGame->GetTextureWall());
    m_TileInfo[TileType_Wall].Setup(  false, m_pGame->GetMeshTile(), m_pGame->GetShader(), m_pGame->GetTextureWall() );
    m_TileInfo[TileType_Box].Setup(   false, m_pGame->GetMeshTile(), m_pGame->GetShader(), m_pGame->GetTextureBox() );
}

void Tilemap::Update(float deltatime)
{
}

void Tilemap::Draw()
{
    for( int y=0; y<m_Height; y++ )
    {
        for( int x=0; x<m_Width; x++ )
        {
            TileTypes tiletype = m_Tiles[y*m_Width + x];

            Mesh* pMesh = m_TileInfo[tiletype].m_pMesh;
            ShaderProgram* pShader = m_TileInfo[tiletype].m_pShader;
            GLuint texture = m_TileInfo[tiletype].m_TextureHandle;

            vec2 worldpos = vec2( (float)x, (float)y ) * m_TileSize;
	
            pMesh->Draw( pShader, texture, worldpos, Vector2(0.0f, 0.0f) , Vector2(1.0f, 1.0f), m_pCamera);
        }
    }
}

void Tilemap::SetCamera(Camera * aCamera)
{
	m_pCamera = aCamera;
}

int Tilemap::GetTileIndexForTilePos(ivec2 tilepos)
{
    assert( tilepos.x >= 0 && tilepos.x < m_Width && tilepos.y >= 0 && tilepos.y < m_Height );

    return tilepos.y*m_Width + tilepos.x;
}

int Tilemap::GetTileIndexForWorldPos(vec2 worldpos)
{
    ivec2 tilepos = GetTilePosForWorldPos( worldpos );

    return GetTileIndexForTilePos( tilepos );
}

ivec2 Tilemap::GetTilePosForTileIndex(int tileindex)
{
    return ivec2( tileindex % m_Width, tileindex / m_Width );
}

ivec2 Tilemap::GetTilePosForWorldPos(vec2 worldpos)
{
    worldpos.x += (m_TileSize.x * 0.5f);

    int tilex = (int)floor( worldpos.x / m_TileSize.x );
    int tiley = (int)floor( worldpos.y / m_TileSize.y );

    return ivec2( tilex, tiley );
}

vec2 Tilemap::GetWorldPosForTileIndex(int tileindex)
{
    ivec2 tilepos = GetTilePosForTileIndex( tileindex );

    return vec2( tilepos.x * m_TileSize.x, tilepos.y * m_TileSize.y );
}

vec2 Tilemap::GetWorldPosForTilePos(ivec2 tilepos)
{
    return vec2( tilepos.x * m_TileSize.x, tilepos.y * m_TileSize.y );
}

bool Tilemap::IsTileWalkableForWorldPos(vec2 worldpos)
{
    ivec2 tilepos = GetTilePosForWorldPos( worldpos );
    return IsTileWalkableForTilePos( tilepos.x, tilepos.y );
}

bool Tilemap::IsTileWalkableForTilePos(int x, int y)
{
    if( x < 0 || x >= m_Width || y < 0 || y > m_Height )
        return false;

    int index = y*m_Width + x;

    TileTypes tiletype = m_Tiles[index];

    return m_TileInfo[tiletype].m_Walkable;
}

bool Tilemap::IsTileWalkableForTilePos(ivec2 tilepos)
{
    return IsTileWalkableForTilePos( tilepos.x, tilepos.y );
}

TileInfo* Tilemap::GetTileInfo(int x, int y)
{
    TileTypes tiletype = GetTileType( x, y );
    return &m_TileInfo[tiletype];
}

TileTypes Tilemap::GetTileType(int x, int y)
{
    if( x < 0 || x >= m_Width || y < 0 || y > m_Height )
        return TileType_Invalid;

    int index = y*m_Width + x;

    TileTypes tiletype = m_Tiles[index];

    return tiletype;
}

void Tilemap::SetTileType(vec2 position, TileTypes type)
{

    m_Tiles[GetTileIndexForWorldPos(position)] = type;
}
