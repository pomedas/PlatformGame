#include "Pathfinding.h"
#include "Engine.h"
#include "Textures.h"
#include "Map.h"
#include "Render.h"
#include "Scene.h"

Pathfinding::Pathfinding() {
    
     //Loads texture to draw the path
    pathTex = Engine::GetInstance().textures.get()->Load("Assets/Maps/MapMetadata.png");
    map = Engine::GetInstance().map.get();
    layerNav = map->GetNavigationLayer();
}

Pathfinding::~Pathfinding() {

}

// L11: BFS Pathfinding methods
void Pathfinding::ResetPath(Vector2D pos) {

    // Clear the queue
    while (!frontier.empty()) {
        frontier.pop();
    }
    //Clear the visited list
    visited.clear();

    frontier.push(pos);
    visited.push_back(pos);
}

void Pathfinding::DrawPath() {

    Vector2D point;
	Map* map = Engine::GetInstance().map.get();

    // Draw visited
    for (const auto& pathTile : visited) {
    	Vector2D pathTileWorld = Engine::GetInstance().map.get()->MapToWorld((int)pathTile.getX(), (int)pathTile.getY());
        SDL_Rect rect = { 1,1,map->GetTileWidth(),map->GetTileHeight()};
        Engine::GetInstance().render->DrawTexture(pathTex, (int)pathTileWorld.getX(), (int)pathTileWorld.getY(),&rect);
    }

    // Draw frontier
    
    // Create a copy of the queue to iterate over
    std::queue<Vector2D> frontierCopy = frontier;

    // Iterate over the elements of the frontier copy
    while (!frontierCopy.empty()) {

        //Get the first element of the queue
        Vector2D frontierTile = frontierCopy.front();
        //Get the position of the frontier tile in the world
        Vector2D pos = Engine::GetInstance().map.get()->MapToWorld((int)frontierTile.getX(), (int)frontierTile.getY());
        //Draw the frontier tile
        SDL_Rect rect = { 1,1,map->GetTileWidth(),map->GetTileHeight() };
        Engine::GetInstance().render->DrawTexture(pathTex, (int)pos.getX(), (int)pos.getY(), &rect);

        //Remove the front element from the queue
        frontierCopy.pop();
    }

}

bool Pathfinding::IsWalkable(int x, int y) {

    bool isWalkable = false;

    // L11: TODO 3: return true only if x and y are within map limits
    // and the tile is walkable (not blocked)

    isWalkable = true;

    return isWalkable;
}

void Pathfinding::PropagateBFS() {

    // L11 TODO 4: Check if we have reach a destination

    // L11: TODO 1: If frontier queue contains elements pop the first element and find the neighbors

        // L11: TODO 2: For each neighbor, if not visited, add it to the frontier queue and visited list
}