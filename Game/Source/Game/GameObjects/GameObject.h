#ifndef __GameObject_H__
#define __GameObject_H__

class Mesh;
class ShaderProgram;
class SpriteSheet;
class Camera;
class GameObject


{
protected:
	Camera* m_pCamera;

    Mesh* m_pMesh;
	Mesh* m_pHealthMesh;

    ShaderProgram* m_pShader;
	ShaderProgram* m_pHealthShader;
    GLuint m_Texture;

    vec2 m_Position;
	vec2 m_UVoffset;
	vec2 m_UVScale;


    float m_MaxHealth;
	float m_Health;
	float m_Radius;
	float m_Red;
	float m_Blue;
	float m_Green;
	float m_Alpha;

    bool m_IsLife;
	int m_CurrentFrameNumber;
	

public:
	GameObject::GameObject(Mesh* pMesh, ShaderProgram* pShader, GLuint texture);
    virtual ~GameObject();

    virtual void Update(float deltatime) = 0;
    virtual void Draw();
    bool GetIsAlive();
	void DealDamage(int Dam);
	void SetCamera(Camera* aCamera);
	virtual float GetRadius();
    void SpawnIsALive();
    void DestroyIsAlive();

    vec2 GetPosition();
};

#endif //__GameObject_H__