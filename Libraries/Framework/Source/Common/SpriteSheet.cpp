#include "FrameworkPCH.h"
#include "../Libraries/cJSON/cJSON.h"
#include "../Common/spritesheet.h"

SpriteSheet::SpriteSheet()
{
    m_TextureHandle = 0;
    m_TextureSize.Set( 0, 0 );
}

SpriteSheet::~SpriteSheet()
{
    if( m_TextureHandle != 0 )
        glDeleteTextures( 1, &m_TextureHandle );
}

void SpriteSheet::LoadFromJSONFile(std::string path, std::string filename)
{
    assert( m_Sprites.size() == 0 && m_TextureHandle == 0 );

    // Load and .json file from disk and parse it into a cJSON object.
    char* buffer = LoadCompleteFile( (path + filename).c_str(), 0 );
    assert( buffer );

    cJSON* jFile = cJSON_Parse( buffer );
    assert( jFile );

    {
        // First, get the filename of the png and create the texture
        cJSON* jTextureFilename = cJSON_GetObjectItem( jFile, "file" );
        m_TextureHandle = LoadTexture( (path + jTextureFilename->valuestring).c_str() );

        // Grab the width and height on the texture.
        cJSON* jTexWidth = cJSON_GetObjectItem( jFile, "width" );
        cJSON* jTexHeight = cJSON_GetObjectItem( jFile, "height" );
        m_TextureSize.Set( (float)jTexWidth->valueint, (float)jTexHeight->valueint );

        // Grab the array of file which hold the info for each sprite
        cJSON* jFiles = cJSON_GetObjectItem( jFile, "Files" );

        int numfiles = cJSON_GetArraySize( jFiles );
        for( int i=0; i<numfiles; i++ )
        {
            cJSON* jSprite = cJSON_GetArrayItem( jFiles, i );
            assert( jSprite );

            SpriteInfo sprite;

            // grab each sprites filename, position and size in the sprite sheet
            cJSON* jFilename = cJSON_GetObjectItem( jSprite, "filename" );
            sprite.m_Filename = jFilename->valuestring;

            cJSON* jPosX = cJSON_GetObjectItem( jSprite, "posx" );
            cJSON* jPosY = cJSON_GetObjectItem( jSprite, "posy" );
            sprite.m_Offset.Set( (float)jPosX->valueint, (float)jPosY->valueint );

            cJSON* jWidth = cJSON_GetObjectItem( jSprite, "origw" );
            cJSON* jHeight = cJSON_GetObjectItem( jSprite, "origh" );
            sprite.m_Size.Set( (float)jWidth->valueint, (float)jHeight->valueint );

            m_Sprites.push_back( sprite );
        }
    }

    // Clean up allocations
    cJSON_Delete( jFile );
    delete[] buffer;
}

SpriteInfo* SpriteSheet::GetSpriteInfoByFilename(std::string filename)
{
    for( unsigned int i=0; i<m_Sprites.size(); i++ )
    {

        if( m_Sprites.at(i).m_Filename == filename )
            return &m_Sprites.at(i);
    }

    return 0;
}
