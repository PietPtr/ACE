#ifndef WORLD_H
#define WORLD_H
#include <SFML/Graphics.hpp>
#include "Chunk.h"
#include "enums.h"

const int WORLDSIZE = 32;

class World
{
    public:
        World(int seed, std::vector<Tile>* tileDataptr);
        Chunk* getChunk(int x, int y);
        void generateWorld();
        void printWorld();
        Image getWorldMap();
        TileName getTile(int x, int y);            // These set and get tiles by global
        void setTile(int x, int y, TileName tile); // coordinates (0, 0) to (511, 511)
    protected:
    private:
        int seed;
        Chunk chunks[WORLDSIZE * WORLDSIZE];

        std::vector<Tile>* tileDataptr;
};

#endif // WORLD_H
