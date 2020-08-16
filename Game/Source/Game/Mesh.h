#ifndef __Mesh_H__
#define __Mesh_H__

class ShaderProgram;
class Camera;
struct VertexFormat
{
    float m_Position[2];
    float m_UV[2];
};

class Mesh
{
protected:
    GLuint m_VBO;
    GLenum m_PrimitiveType;
    int m_NumVerts;
	

public:
    void GenerateSprite(float width, float height);

public:
    Mesh();
    virtual ~Mesh();

	void Draw(ShaderProgram* pShader, GLuint texture, vec2 pos, vec2 UVoffset, vec2 UVscale, Camera * cam);
	void DrawHealthbar(ShaderProgram* pShader, GLuint texture, vec2 pos, vec2 UVoffset, vec2 UVscale, float Health, float maxHealth, Camera * cam ,float Red, float Green, float Blue , float Alpha);
	void DrawScore(ShaderProgram* pShader, GLuint texture,  vec2 pos, int Texturewidth, int TextureHeight, int FPerRow, int FPerCol, int currentnumber, Camera * cam);
};

#endif //__Mesh_H__