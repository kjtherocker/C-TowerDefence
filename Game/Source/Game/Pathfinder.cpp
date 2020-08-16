#include "GamePCH.h"

Pathfinder::Pathfinder(Tilemap* pTilemap, int width, int height)
{
    m_pTilemap = pTilemap;

    m_NumNodes = width * height;

    m_Nodes = new PathNode[m_NumNodes];
    m_OpenNodes = new int[m_NumNodes];

    m_MapWidth = width;
    m_MapHeight = height;

    Reset();
}
    
Pathfinder::~Pathfinder()
{
    delete[] m_Nodes;
    delete[] m_OpenNodes;
}

void Pathfinder::Reset()
{
    m_NumOpen = 0;

    for( int i=0; i<m_NumNodes; i++ )
    {
        m_Nodes[i].parentNodeIndex = -1;
        m_Nodes[i].status = PathNode::Unchecked;

        m_Nodes[i].f = 0;
        m_Nodes[i].g = FLT_MAX; // set g to be highest cost possible, so any comparison will be better.
        m_Nodes[i].h = -1; // -1 indicates the heuristic hasn't been calculated yet.
    }
}
        
bool Pathfinder::FindPath(int sx, int sy, int ex, int ey)
{
    // reset the pathfinder
    Reset();

    // get the starting tile index and the destination tile index
    int currentNodeIndex = CalculateNodeIndex( sx, sy );
    int endNodeIndex = CalculateNodeIndex( ex, ey );

    // set starting node cost to zero, then add it to the open list to start the process.
    m_Nodes[currentNodeIndex].g = 0;
    AddToOpen( currentNodeIndex );

    while( true )
    {
        // find the lowest F and remove it from the open list.
        currentNodeIndex = FindLowestFInOpen();
        RemoveFromOpen( currentNodeIndex );

        // if we found the end node, we're done.
        if( currentNodeIndex == endNodeIndex )
            return true;

        // mark it as closed
        m_Nodes[currentNodeIndex].status = PathNode::Closed;

        // add neighbours to open list
        AddNeighboursToOpenList( currentNodeIndex, endNodeIndex );

        // if we're out of nodes to check, then we're done without finding the end node.
        if( m_NumOpen == 0 )
            return false;
    }

    // this shouldn't happen, we should either find the destination or run out of nodes in the open list.
    assert( false );
    return false;
}

int Pathfinder::GetPath(int* path, int maxdistance, int ex, int ey)
{
    int nodeIndex = CalculateNodeIndex( ex, ey );

    int length = 0;
    while( true )
    {
        if( path != nullptr && length >= maxdistance )
            return -1; // path didn't fit in size.

        if( path != nullptr ) // if no path array is passed in, just get the length
            path[length] = nodeIndex;
        length++;

        nodeIndex = m_Nodes[nodeIndex].parentNodeIndex;

        if( nodeIndex == -1 )
            return length;
    }

    // there should be no way to get here.
    assert( false );
    return -1;
}

void Pathfinder::AddToOpen(int nodeindex)
{
    assert( m_Nodes[nodeindex].status != PathNode::Closed );

    // if the node isn't already open, then add it to the list.
    if( m_Nodes[nodeindex].status != PathNode::Open )
    {
        m_OpenNodes[m_NumOpen] = nodeindex;
        m_NumOpen++;
        m_Nodes[nodeindex].status = PathNode::Open;
    }
}

void Pathfinder::RemoveFromOpen(int nodeindex)
{
    // remove the node from the open list, since we don't care about order, replace the node we're removing with the last node in list
    for( int i=0; i<m_NumOpen; i++ )
    {
        if( m_OpenNodes[i] == nodeindex )
        {
            m_NumOpen--;
            m_OpenNodes[i] = m_OpenNodes[m_NumOpen];
            return;
        }
    }
}

int Pathfinder::FindLowestFInOpen()
{
    // loop through the nodes in the open list, then find and return the node with the lowest f score
    float lowest = FLT_MAX;
    int lownode = -1;

    for( int i=0; i<m_NumOpen; i++ )
    {
        if( m_Nodes[m_OpenNodes[i]].f < lowest )
        {
            lowest = m_Nodes[m_OpenNodes[i]].f;
            lownode = m_OpenNodes[i];
        }
    }

    return lownode;
}

int Pathfinder::CalculateNodeIndex(int tx, int ty)
{
    assert( tx >= 0 && tx < m_MapWidth &&
            ty >= 0 && ty < m_MapHeight );

    // calculate the node index based on the tiles x/y
    return ty*m_MapWidth + tx;
}

int Pathfinder::CheckIfNodeIsClearAndReturnNodeIndex(int tx, int ty)
{
    // if the node is out of bounds, return -1 (an invalid tile index)
    if( tx < 0 || tx >= m_MapWidth ||
        ty < 0 || ty >= m_MapHeight )
        return -1;

    int nodeIndex = CalculateNodeIndex( tx, ty );

    // if the node is already closed, return -1 (an invalid tile index)
    if( m_Nodes[nodeIndex].status == PathNode::Closed )
        return -1;
        
    // if the node can't be walked on, return -1 (an invalid tile index)
    if( m_pTilemap->IsTileWalkableForTilePos( tx, ty ) == false )
        return -1;

    // return a valid tile index
    return nodeIndex;
}

void Pathfinder::AddNeighboursToOpenList(int nodeIndex, int endNodeIndex)
{
    // calculate the tile x/y based on the nodeIndex
    int tx = nodeIndex % m_MapWidth;
    int ty = nodeIndex / m_MapWidth;

    // create an array of the four neighbour tiles
    int nodestoadd[4];
    nodestoadd[0] = CheckIfNodeIsClearAndReturnNodeIndex( tx,   ty-1 );
    nodestoadd[1] = CheckIfNodeIsClearAndReturnNodeIndex( tx,   ty+1 );
    nodestoadd[2] = CheckIfNodeIsClearAndReturnNodeIndex( tx+1, ty   );
    nodestoadd[3] = CheckIfNodeIsClearAndReturnNodeIndex( tx-1, ty   );

    // loop through the array
    for( int i=0; i<4; i++ )
    {
        int nodetoadd;
        nodetoadd = nodestoadd[i];

        // check if the node to add has a valid node index
        if( nodetoadd != -1 )
        {
            int cost = 1; // incremental cost to reach neighbour
            int newcost = (int)m_Nodes[nodeIndex].g + cost; // total cost to reach neighbour

            // add the node to the open list
            AddToOpen( nodetoadd );

            // if the cost to get there from here is less than the previous cost to get there, then overwrite the values.
            if( newcost < m_Nodes[nodetoadd].g )
            {
                // set the parent node.
                m_Nodes[nodetoadd].parentNodeIndex = nodeIndex;

                // calculate the cost to travel to that node.
                m_Nodes[nodetoadd].g = (float)newcost;

                // if we haven't already calculated the heuristic, calculate it.
                if( m_Nodes[nodetoadd].h == -1 )
                    m_Nodes[nodetoadd].h = CalculateH( nodetoadd, endNodeIndex );

                // calculate the final value
                m_Nodes[nodetoadd].f = m_Nodes[nodetoadd].g + m_Nodes[nodetoadd].h;
            }
        }
    }
}


float Pathfinder::CalculateH(int nodeIndex1, int nodeIndex2)
{
    // calculate the h score using the manhatten distance (absolute diff in x + absolute diff in y)
    int tx1 = nodeIndex1 % m_MapWidth;
    int ty1 = nodeIndex1 / m_MapWidth;

    int tx2 = nodeIndex2 % m_MapWidth;
    int ty2 = nodeIndex2 / m_MapWidth;

    return (float)abs( tx1 - tx2 ) + abs( tx1 - tx2 );
}
