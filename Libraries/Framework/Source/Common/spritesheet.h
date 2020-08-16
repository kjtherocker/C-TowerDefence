#ifndef __SpriteSheet_H__
#define __SpriteSheet_H__

class SpriteInfo
{
public:
    std::string m_Filename;
    Vector2 m_Size;
    Vector2 m_Offset;

    void Setup(std::string filename, Vector2 size, Vector2 offset)
    {
        m_Filename = filename;
        m_Size = size;
        m_Offset = offset;
    }
};

class SpriteSheet
{
protected:
    std::vector<SpriteInfo> m_Sprites;

    GLuint m_TextureHandle;
    vec2 m_TextureSize;

public:
    SpriteSheet();
    virtual ~SpriteSheet();

    void LoadFromJSONFile(std::string path, std::string filename);

    GLuint GetTextureHandle() { return m_TextureHandle; }
    vec2 GetTextureSize() { return m_TextureSize; }

    SpriteInfo* GetSpriteInfoByFilename(std::string filename);
};

#endif //__SpriteSheet_H__