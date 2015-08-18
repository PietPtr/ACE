#ifndef WORLD_H
#define WORLD_H
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "enums.h"

using namespace sf;

const int WORLDSIZE = 512; //defualt: 512, max: 1435
const int TILESIZE  = 32;

class World
{
    public:
        World();
        void initialize(int seed, std::vector<Tile>* tileDataptr, std::vector<Texture>* txtptr, RenderWindow* window);

        Image getWorldMap(Vector2f playerPosition);
        TileName getTile(int x, int y);            // These set and get tiles by global
        void setTile(int x, int y, TileName tile); // coordinates (0, 0) to (511, 511)
        bool isGrassLike(int x, int y);            // returns true for certain tiles that look like grass (GRASS, TREE, etc)
        void setSeed(int _seed) { seed = _seed; }
        int getSeed() { return seed; }

        void loadWorld(std::string filename);
        void saveWorld(std::string filename);

        //Main generation methods
        void generateWorld();
        void generateGrass();
        void generateSea();
        void generateLakes();
        void generateRivers();
        void generateRock();
        void generateTrees();
        void generationCleanup();
        //Auxiliary generation methods
        void generateCircle(Vector2f position, int radius, TileName tile);
        void smooth(int x, int y, TileName innerTile, TileName outerTile);

        void draw(Vector2<double> position, Vector2f viewDistance, Time totalTime);
    protected:
    private:
        int seed = 0;
        int tiles[WORLDSIZE * WORLDSIZE];

        Time previousDrawTime;

        std::vector<Tile>* tileDataptr = nullptr;
        std::vector<Texture>* txtptr = nullptr;
        RenderWindow* window;
};

#endif // WORLD_H
