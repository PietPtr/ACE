#include "World.h"
#include "enums.h"
#include "SFML/Graphics.hpp"
#include "time.h"
#include <fstream>

using namespace sf;

int randint(int low, int high, int seed);
int randint(int low, int high);

const double PI = 3.141592654;

World::World(int _seed, std::vector<Tile>* _tileDataptr)
{
    seed = _seed;
    tileDataptr = _tileDataptr;
}

void World::setTile(int x, int y, TileName tile)
{
    tiles[y * WORLDSIZE + x] = (int)tile;
}

TileName World::getTile(int x, int y)
{
    return (TileName)tiles[y * WORLDSIZE + x];
}

void World::generateWorld()
{
    //Set every tile to grass
    std::cout << "Generating grass.\n";
    for (int y = 0; y < WORLDSIZE; y++)
    {
        for (int x = 0; x < WORLDSIZE; x++)
        {
            setTile(x, y, GRASS);
        }
    }

    //Generate the sea
    std::cout << "Generating sea.\n";

    //generation modifiers
    float baseAmp = 3 + randint(-100, 100, seed) / 100.0;                        //Higher -> higher waves
    float maxAmp = 300 + randint(-100, 100, seed);                               //Higher -> higher waves after randomization
    float baseFreq = 10.0 + randint(-500, 500, seed) / 100.0;                    //Higher -> less waves
    float archMod = 1000 + randint(0, 500, seed);                             //Higher -> wider sea
    float height = 72 + randint(0, 32, seed);                                  //Higher -> coastline more north

    //std::cout << "baseAmp:" << baseAmp << ", maxAmp:" << maxAmp << ", baseFreq:" << baseFreq << ", archMod:" << archMod << "\n";

    for (int x = 0; x < WORLDSIZE; x++)
    {
        int X = x - 256;
        int Y = std::sin(X / baseFreq) * baseAmp * randint(100, maxAmp, X / (2 * PI * baseFreq) + seed) / 100.0;;
        Y = Y - (1.0/archMod)*X*X + height;
        Y = Y < 0 ? 0 : Y;

        for (int y = WORLDSIZE - 1; y > WORLDSIZE - Y; y--)
        {
            setTile(x, y, SEA_WATER);
        }
    }

    //Generate beaches
    std::cout << "Generating beaches.\n";

    //Generate lakes

    //Generate rivers (connecting some lakes to the sea)

    //Generate rocky areas and ores

    //Generate trees

    //setTile(15, 213, SEA_WATER);

}

void World::printWorld()
{
    int skip = 1;
    for (int y = 0; y < WORLDSIZE; y += skip)
    {
        for (int x = 0; x < WORLDSIZE; x += skip)
        {
            switch(getTile(x, y))
            {
            case(0):
                std::cout << "G";
                break;
            case(1):
                std::cout << "T";
                break;
            case(2):
                std::cout << "~";
                break;
            default:
                std::cout << " ";
                break;
            }
        }
        std::cout << "\n";
    }

}

Image World::getWorldMap()
{
    Image worldMap;
    worldMap.create(WORLDSIZE, WORLDSIZE, Color(0, 255, 0));

    for (int y = 0; y < WORLDSIZE; y++)
    {
        for (int x = 0; x < WORLDSIZE; x++)
        {
            Color pixelColor = tileDataptr->at(getTile(x, y)).getColor();
            worldMap.setPixel(x, y, pixelColor);
        }
    }

    return worldMap;
}
