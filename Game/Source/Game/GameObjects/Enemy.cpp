#include "GamePCH.h"
#include "../../Libraries/Framework/Source/Common/spritesheet.h"

Enemy::Enemy(Mesh* pMesh, Mesh* healthmesh, ShaderProgram* pShaderhealth, ShaderProgram* pShader, GLuint texture, vec2 position, vec2 size)
	: GameObject( pMesh, pShader,texture)
{
    //INTIALIZE ALL
	m_pController = 0;
	m_pTilemap = 0;
	m_pCamera = 0;

	m_pHealthMesh = healthmesh;
	m_pHealthShader = pShaderhealth;

	m_Red = 1.0f;
	m_Alpha = 1.0f;


	m_Position = position;
	m_Target = m_Position;

	m_PlayerSize = size;
   
    CurrentPath[PATHFINDINGPATH] = {};

    m_Found = false;
	m_Radius = ENEMYRADIUS;

	m_MaxHealth = MAXHEALTH;
	m_Health = m_MaxHealth;
	
	m_AccumlatedTime = 0;
	m_CurrentFrameNumber = 0;
}

Enemy::~Enemy()
{
    //Delete ALL
}


void Enemy::SetTilemap(Tilemap* pTilemap)
{
    //SET m_Tilemap to the arguement
	m_pTilemap = pTilemap;
}

float Enemy::GetHealth()
{
    //RETURN m_Health;
	return m_Health;
}

void Enemy::Spawn()
{
    //SET m_Health to 100
    m_Health = 100;
    //SET the m_Position Vector2 to (1.0f,1.0f)
    m_Position = Vector2(1.0f, 1.0f);
}

bool Enemy::GetIsAlive()
{
    //RETURN m_IsLife
    return m_IsLife;
}

bool Enemy::GetFound()
{
    //RETURN m_Found
    return m_Found;
}

void Enemy::SetPathfinder(int a_Columns, int a_Rows,Vector2 endp, Pathfinder* a_Pathfinder)
{
    //SET m_Truepos to the players currentposition
    ivec2 m_truepos = ivec2((int)m_Position.x, (int)m_Position.y);
    //SET the StartPoint to truepos
    ivec2 startpoint = m_truepos;
    //IF the End Position is equal to Vector2(0.0f,0.0f)
	if (endp == Vector2(0.0f, 0.0f))
	{
        //SET ivec2 endpoint to ivec2(The currentcolumns - 2, the current Rows - 2)	
	
		ivec2 endpoint = ivec2(a_Columns - 2, a_Rows - 2);
        //SET  m_Found to the pathfindersFind path
		m_Found = a_Pathfinder->FindPath(startpoint.x, startpoint.y, endpoint.x, endpoint.y);
        //SET the path  to a large number
		int Path[PATHFINDINGPATH];
        //Set the lenght of the pathfinder 
		int len = a_Pathfinder->GetPath(Path, PATHFINDINGPATH, endpoint.x, endpoint.y);

        //Send in SetPath
		SetPath(Path, len);
	}
	else
    {
        //SET ivec2 endpoint to ivec2(The currentcolumns - 2, the current Rows - 2)	
		ivec2 endpoint = ivec2((int)endp.x ,(int)endp.y);
        //SET  m_Found to the pathfindersFind path
		m_Found = a_Pathfinder->FindPath(startpoint.x, startpoint.y, endpoint.x, endpoint.y);
        //SET the path  to a large number
		int Path[PATHFINDINGPATH];
        //Set the lenght of the pathfinder 
		int len = a_Pathfinder->GetPath(Path, PATHFINDINGPATH, endpoint.x, endpoint.y);
        //Send in SetPath
		SetPath(Path, len);
	}


}

void Enemy::SetSpriteSheet(SpriteSheet * Sprite)
{
    //Set the spritesheet the arguement Sprite
	m_pSpriteSheet = Sprite;
}

void Enemy::SetPath(int * a_Path, int length)
{
    //For how many there are in the length 
    for (int i = 0; i < length; i++)
    {
        //Check the current path 
        CurrentPath[i] = a_Path[length - i - 1];
    }
    for (int i = length; i < PATHFINDINGPATH; i++)
    {
        //Check if its an invalid index
        CurrentPath[i] = -1;
    }
}

bool Enemy::WouldPlayerFitOnMap(vec2 worldpos)
{
	vec2 adjustedworldpos;
	ivec2 tilepos;

	// Test bottom left corner.
	adjustedworldpos = worldpos;
	adjustedworldpos.x -= m_PlayerSize.x / 2;
	tilepos = m_pTilemap->GetTilePosForWorldPos(adjustedworldpos);
	if (m_pTilemap->IsTileWalkableForTilePos(tilepos.x, tilepos.y) == false)
		return false;

	// Test bottom right corner.
	adjustedworldpos = worldpos;
	adjustedworldpos.x += m_PlayerSize.x / 2;
	tilepos = m_pTilemap->GetTilePosForWorldPos(adjustedworldpos);
	if (m_pTilemap->IsTileWalkableForTilePos(tilepos.x, tilepos.y) == false)
		return false;

	// Test top left corner.
	adjustedworldpos = worldpos;
	adjustedworldpos.x -= m_PlayerSize.x / 2;
	adjustedworldpos.y += m_PlayerSize.y;
	tilepos = m_pTilemap->GetTilePosForWorldPos(adjustedworldpos);
	if (m_pTilemap->IsTileWalkableForTilePos(tilepos.x, tilepos.y) == false)
		return false;

	// Test top right corner.
	adjustedworldpos = worldpos;
	adjustedworldpos.x += m_PlayerSize.x / 2;
	adjustedworldpos.y += m_PlayerSize.y;
	tilepos = m_pTilemap->GetTilePosForWorldPos(adjustedworldpos);
	if (m_pTilemap->IsTileWalkableForTilePos(tilepos.x, tilepos.y) == false)
		return false;

	return true;
}

void Enemy::Update(float deltatime)
{
	assert(m_pTilemap != 0);
	assert(m_pSpriteSheet != 0);
    //IF the m_Health is lesser then 0 then
    if (m_Health <= 0)
    {
        //SET m_IsLife is equal to false
        m_IsLife = false;
    }

    //IF m_IsLife is equal to true then
    if (m_IsLife == true)
    {


        //SET the speed to LOWSPEED
        float speed = LOWSPEED;

        //SET the direction to Vector2(0,0)
        vec2 dir(0, 0);

        //SET the Timer
        m_AccumlatedTime += deltatime;

        //IF the Timer is greater then 0.2f Then
        if (m_AccumlatedTime > 0.2f)
        {
            //SET Timer to 0.0f
            m_AccumlatedTime = 0.0f;
            //INCREMENT m_CurrenFrameNumber
            m_CurrentFrameNumber++;
            //If m_CurrentFrameNumber is greater then 1 then
            if (m_CurrentFrameNumber > 1)
            {
                //SET m_CurrentFrameNumber to 0
                m_CurrentFrameNumber = 0;
            }
        }


        //SET the animfilename to nothing
        std::string animfilename = "";


        //SET the actualframe to the CurrentFramenumber
        int actualframe = m_CurrentFrameNumber;

        //SET the difference to 0
        Vector2 Difference = Vector2(0.0f, 0.0f);

        //FOR how many are in the path 
        for (int i = 0; i < PATHFINDINGPATH; i++)
        {
            //if the currentpath is equal to an invalid index
            if (CurrentPath[i] == -1)
            {
                break;
            }


            //SET the difference to 0
            Difference = m_Position - m_pTilemap->GetWorldPosForTileIndex(CurrentPath[i]);
            //IF the difference is lesser then  0.05 then
            if (abs(Difference.x) < 0.05 && abs(Difference.y) < 0.05)
            {
                // IF the path is on the current path and not invalid then
                if (i + 1 < PATHFINDINGPATH && CurrentPath[i + 1] != -1)
                {
                    //SET m_Target to the next node
                    m_Target = m_pTilemap->GetWorldPosForTileIndex(CurrentPath[i + 1]);

                }
                else
                {
                    //m_Target is equal to m_Position
                    m_Target = m_Position;
                }
            }
        }

        //IF m_Target is rqual to LOWHEALTH then
		if (m_Health <= LOWHEALTH)
		{
            //SET speed to HIGHSPEED
			speed = HIGHSPEED;
		}
		
        //SET dir to m_Target - m_Position
		dir = m_Target - m_Position;
        //NORMALIZE dir
        dir.Normalize();



        ///UPDATE m_Position plus equal dir times speed times deltatime
        m_Position += dir*speed*deltatime;
        //if the direction is right 
        if (dir.x > 0.5f)
        {
            //Set Animation frame
            animfilename = "OctorocRight" + std::to_string(actualframe + 1);
        }
        //if the direction is left
        else if (dir.x < -0.5f)
        {
            //Set Animation frame
            animfilename = "OctorocLeft" + std::to_string(actualframe + 1);
        }
        //if the direction is down
        else if (dir.y < -0.5f)
        {
            //Set Animation frame
            animfilename = "OctorocDown" + std::to_string(actualframe + 1);
        }
        //if the direction is up
        else if (dir.y > 0.5f)
        {
            //Set Animation frame
            animfilename = "OctorocUp" + std::to_string(actualframe + 1);
        }
        else
        {
            //Set Animation frame
            animfilename = "OctorocDown" + std::to_string(actualframe + 1);
        }

        // Check for collisions and set dir to 0 if we can't move.
        {
            // If we can't fit horizontally, zero out dir x.
            if (WouldPlayerFitOnMap(m_Position + vec2(dir.x, 0.0f)) == false)
            {

                dir.x = 0;
            }

            // If we can't fit vertically, zero out dir y.
            if (WouldPlayerFitOnMap(m_Position + vec2(0.0f, dir.y)) == false)
            {

                dir.y = 0;
            }
        }

        //ADD png to the end of the file name 
        animfilename += ".png";

        SpriteInfo* pInfo = m_pSpriteSheet->GetSpriteInfoByFilename(animfilename);

        vec2 texsize = m_pSpriteSheet->GetTextureSize();

        m_UVScale.Set(pInfo->m_Size.x / (float)texsize.x, pInfo->m_Size.y / (float)texsize.y);

        m_UVoffset.Set(pInfo->m_Offset.x / texsize.x, pInfo->m_Offset.y / texsize.y);
    }
	
}
