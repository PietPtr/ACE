#ifndef WORLD_H
#define WORLD_H
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "enums.h"

const int WORLDSIZE = 512;

class World
{
    public:
        World(int seed, std::vector<Tile>* tileDataptr);
        void generateWorld();
        void printWorld();
        Image getWorldMap();
        TileName getTile(int x, int y);            // These set and get tiles by global
        void setTile(int x, int y, TileName tile); // coordinates (0, 0) to (511, 511)
        void setSeed(int _seed) { seed = _seed; }
        int getSeed() { return seed; }

        void generateGrass();
        void generateSea();
        void generateLakes();
        void generateRivers();
        void generateRock();
        void generateTrees();
        void generationCleanup();
    protected:
    private:
        int seed;
        int tiles[WORLDSIZE * WORLDSIZE];

        std::vector<Tile>* tileDataptr;
};

#endif // WORLD_H
