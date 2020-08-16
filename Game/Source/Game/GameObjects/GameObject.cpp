#include "GamePCH.h"

GameObject::GameObject(Mesh* pMesh, ShaderProgram* pShader, GLuint texture)
{
    //Initalize all
    m_pMesh = pMesh;
    m_pShader = pShader;
    m_Texture = texture;

	m_pHealthMesh = 0;
	m_pHealthShader = 0;

	m_Red = 0;
	m_Blue = 0;
	m_Green = 0;
	m_Alpha = 0;

	m_MaxHealth = 0;
	m_Health = m_MaxHealth;
	m_Radius = 0;
	m_IsLife = true;

	m_Position.Set(0, 0);
	m_UVoffset = Vector2(0.0f, 0.0f);
	m_UVScale = Vector2(0.0f, 0.0f);
 
}

GameObject::~GameObject()
{
    //DELETE all
	glDeleteTextures(1, &m_Texture);

}

void GameObject::Draw()
{
    // IF the bool variable m_islife is true then
	if (m_IsLife)
	{
        //DRAW the mesh to the parameters
		m_pMesh->Draw(m_pShader, m_Texture, m_Position, m_UVoffset, m_UVScale, m_pCamera);
        //if the m_Health value is greater then 0 but lesser then 99 then
        if (m_Health > 0 && m_Health <= 99)
		{
            //Draw the healthbarsmesh
			m_pHealthMesh->DrawHealthbar(m_pHealthShader, m_Texture, m_Position + Vector2(0.0f, 1.2f), m_UVoffset, m_UVScale, m_Health, m_MaxHealth, m_pCamera, m_Red, m_Blue, m_Green, m_Alpha); 
		}
	}
}

bool GameObject::GetIsAlive()
{
    //RETURN m_IsLife
    return m_IsLife;
}

void GameObject::DealDamage(int Dam)
{
    //MINUS m_Health to the parameters
	m_Health -= Dam;
}

void GameObject::SetCamera(Camera * aCamera)
{
    //SET m_pCamera to the argument
	m_pCamera = aCamera;
}

float GameObject::GetRadius()
{
    //RETURN m_Radius
	return m_Radius;
}

void GameObject::SpawnIsALive()
{
    //SET m_IsLife to true;
    m_IsLife = true;
}

void GameObject::DestroyIsAlive()
{
    //SET m_IsLife to false;
    m_IsLife = false;
}

vec2 GameObject::GetPosition()
{
    //RETURN m_Position
    return m_Position;
}
