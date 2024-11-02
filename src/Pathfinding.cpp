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
    delete layerNav;
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

    // Draw visited
    for (const auto& pathTile : visited) {
    	Vector2D pathTileWorld = Engine::GetInstance().map.get()->MapToWorld(pathTile.getX(), pathTile.getY());
        SDL_Rect rect = { 32,0,32,32 };
        Engine::GetInstance().render.get()->DrawTexture(pathTex, pathTileWorld.getX(), pathTileWorld.getY(),&rect);
    }

    // Draw frontier
    
    // Create a copy of the queue to iterate over
    std::queue<Vector2D> frontierCopy = frontier;

    // Iterate over the elements of the frontier copy
    while (!frontierCopy.empty()) {

        //Get the first element of the queue
        Vector2D frontierTile = frontierCopy.front();
        //Get the position of the frontier tile in the world
        Vector2D pos = Engine::GetInstance().map.get()->MapToWorld(frontierTile.getX(), frontierTile.getY());
        //Draw the frontier tile
        SDL_Rect rect = { 0,0,32,32 };
        Engine::GetInstance().render.get()->DrawTexture(pathTex, pos.getX(), pos.getY(), &rect);
        //Remove the front element from the queue
        frontierCopy.pop();
    }

}

bool Pathfinding::IsWalkable(int x, int y) {

    bool isWalkable = false;

    // L11: TODO 3: return true only if x and y are within map limits
    // and the tile is walkable (tile id 0 in the navigation layer)

    //Set isWalkable to true if the position is inside map limits and is a position that is not blocked in the navigation layer
    //Get the navigation layer

    if (layerNav != nullptr) {
        if (x >= 0 && y >= 0 && x < map->GetWidth() && y < map->GetHeight()) {
            int gid = layerNav->Get(x, y);
            if (gid != blockedGid) isWalkable = true;
        }
    }

    return isWalkable;
}

void Pathfinding::PropagateBFS() {

    // L11 TODO 4: Check if we have reach a destination
    bool foundDestination = false;
    if (frontier.size() > 0) {
        Vector2D frontierTile = frontier.front();
        Vector2D playerPos = Engine::GetInstance().scene.get()->GetPlayerPosition();
        Vector2D playerPosTile = Engine::GetInstance().map.get()->WorldToMap((int)playerPos.getX(), (int)playerPos.getY());

        if (frontierTile == playerPosTile) {
            foundDestination = true;
        }
    }

    // L11: TODO 1: If frontier queue contains elements pop the first element and find the neighbors
    if (frontier.size() > 0 && !foundDestination) {

        //Get the value of the firt element in the queue
        Vector2D frontierTile = frontier.front();
        //remove the first element from the queue
        frontier.pop();
        
        std::list<Vector2D> neighbors;
        if (IsWalkable(frontierTile.getX() + 1, frontierTile.getY())) {
            neighbors.push_back(Vector2D(frontierTile.getX() + 1, frontierTile.getY()));
        }
        if (IsWalkable(frontierTile.getX(), frontierTile.getY() + 1)) {
            neighbors.push_back(Vector2D(frontierTile.getX(), frontierTile.getY() + 1));
        }
        if (IsWalkable(frontierTile.getX() - 1, frontierTile.getY())) {
            neighbors.push_back(Vector2D(frontierTile.getX() - 1, frontierTile.getY()));
        }
        if (IsWalkable(frontierTile.getX(), frontierTile.getY() - 1)) {
            neighbors.push_back(Vector2D(frontierTile.getX(), frontierTile.getY() - 1));
        }

        // L11: TODO 2: For each neighbor, if not visited, add it to the frontier queue and visited list
        for(const auto& neighbor : neighbors) {
			if (std::find(visited.begin(), visited.end(), neighbor) == visited.end()) {
				frontier.push(neighbor);
				visited.push_back(neighbor);
			}
		}

    }
}