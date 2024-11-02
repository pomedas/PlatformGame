#pragma once
#include <list>
#include <queue>
#include "Vector2D.h"
#include "SDL2/SDL.h"
#include "Map.h"

class Pathfinding
{

public:

	Pathfinding();

	~Pathfinding();

    // L11: BFS Pathfinding methods
    void ResetPath(Vector2D pos);
    void DrawPath();
    bool IsWalkable(int x, int y);
    void PropagateBFS();

public:

    // L11: BFS Pathfinding variables
    Map* map;
    MapLayer* layerNav;
    std::queue<Vector2D> frontier;
    std::list<Vector2D> visited;
    SDL_Texture* pathTex = nullptr;
    Vector2D destination;

    //Gid of the tiles that block the path - Important adjust this value to your map
    int blockedGid = 49; 
};

