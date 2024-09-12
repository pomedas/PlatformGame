#pragma once

#include "Module.h"
#include <list>

// Create a struct needed to hold the information to Map node
struct TileSet
{
    int firstgid;
    std::string name;
    int tilewidth;
    int tileheight;
    int spacing;
    int margin;
    int tilecount;
    int columns;

    SDL_Texture* texture;

    // Implement the method that receives the gid and returns a Rect

    SDL_Rect GetRect(unsigned int gid) {
        SDL_Rect rect = { 0 };

        int relativeIndex = gid - firstgid;
        rect.w = tilewidth;
        rect.h = tileheight;
        rect.x = margin + (tilewidth + spacing) * (relativeIndex % columns);
        rect.y = margin + (tileheight + spacing) * (relativeIndex / columns);

        return rect;
    }
};

struct MapLayer
{
    // Add the info to the MapLayer Struct
    int id;
    std::string name;
    int width;
    int height;
    unsigned int* tiles;

    // Short function to get the gid value of x,y
    unsigned int Get(int x, int y) const
    {
        return tiles[(y * width) + x];
    }
};

struct MapData
{
    int width;
    int height;
    int tilewidth;
    int tileheight;
    std::list<TileSet*> tilesets;

    //Add a list/array of layers to the map
    std::list<MapLayer*> layers;
};

// Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!

class Map : public Module
{
public:

    Map();

    // Destructor
    virtual ~Map();

    // Called before render is available
    bool Awake(pugi::xml_node config);

    // Called before the first frame
    bool Start();

    // Called each loop iteration
    bool Update(float dt);

    // Called before quitting
    bool CleanUp();

    // Load new map
    bool Load(std::string mapFileName);

    //Create a method that translates x,y coordinates from map positions to world positions
    Vector2D MapToWorld(int x, int y) const;

public: 
    std::string mapName;
    std::string mapPath;

private:
    // L05: DONE 1: Declare a variable data of the struct MapData
    MapData mapData;
    bool mapLoaded;
};