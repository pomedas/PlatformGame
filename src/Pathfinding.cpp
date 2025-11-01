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
		// we use the second tile in the tileset to draw the visited. That's why we use rect x=33
        SDL_Rect rect = { 33,1,map->GetTileWidth(),map->GetTileHeight()};
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
		//Draw the frontier tile. We use the first tile in the tileset to draw the frontier. That's why we use rect x=1
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
    if(layerNav != nullptr) {

		//Check map limits
        if (x >= 0 && x < map->GetMapSizeInTiles().getX() &&
            y >= 0 && y < map->GetMapSizeInTiles().getY()) {
			//Get the gid of the tile
            int gid = layerNav->Get(x, y);

			//Check if the gid is different from the blocked gid
            if (gid != blockedGid) {
                isWalkable = true;
            }
        }
	}

    return isWalkable;
}

void Pathfinding::PropagateBFS() {

    // L11 TODO 4: Check if we have reach a destination
    bool foundDestination = false;
    if (!frontier.empty()) {
        Vector2D frontierTile = frontier.front();
		Vector2D playerPos = Engine::GetInstance().scene->GetPlayerPosition();
        Vector2D playerPosTile = Engine::GetInstance().map->WorldToMap((int)playerPos.getX(), (int)playerPos.getY());

        if (frontierTile == playerPosTile) {
            foundDestination = true;
        }
    }

    // L11: TODO 1: If frontier queue contains elements pop the first element and find the neighbors
    if (!frontier.empty() && !foundDestination) {
        //Get the value of the firt element in the queue
        Vector2D frontierTile = frontier.front();
        //remove the first element from the queue
        frontier.pop();
		//Get the neighbors (4 directions)
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
        for (const auto& neighbor : neighbors) {

			//Check if the neighbor has been visited
			bool isVisited = false;
            for(const auto& visitedTile : visited) {
                if(visitedTile == neighbor) {
                    isVisited = true;
                    break;
				}
			}
			//If not visited add it to the frontier and visited list
            if (!isVisited) {
                frontier.push(neighbor);
                visited.push_back(neighbor);
            }
        }

    }
}