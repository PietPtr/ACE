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
    x = x < 0 ? 0 : x;
    y = y < 0 ? 0 : y;
    x = x > WORLDSIZE ? WORLDSIZE : x;
    y = y > WORLDSIZE ? WORLDSIZE : y;

    tiles[y * WORLDSIZE + x] = (int)tile;
}

TileName World::getTile(int x, int y)
{
    return (TileName)tiles[y * WORLDSIZE + x];
}

void World::generateWorld()
{
    generateGrass();
    generateSea();
    generateLakes();
    generateRivers();
    generateRock();
    generateTrees();
}

void World::generateGrass()
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
}

void World::generateSea()
{
    //Generate the sea
    std::cout << "Generating sea and beaches.\n";

    //generation modifiers
    float seaBaseAmp = 3 + randint(-100, 100, seed) / 100.0;                        //Higher -> higher waves
    float seaMaxAmp = 300 + randint(-100, 100, seed);                               //Higher -> higher waves after randomization
    float seaBaseFreq = 10.0 + randint(-500, 500, seed) / 100.0;                    //Higher -> less waves
    float seaArchMod = 1000 + randint(0, 500, seed);                                //Higher -> wider sea
    float seaHeight = 72 + randint(0, 32, seed);                                    //Higher -> coastline more north

    float beachBaseFreq = 10.0 + randint(0, 1000, seed) / 100.0;
    float beachBaseAmp = 4 + randint(-100, 300, seed) / 100.0;
    float minBeachHeight = 3 + randint(-1, 1, seed);
    float beachOffset = 128 + randint(-128, 128);

    //std::cout << "baseAmp:" << baseAmp << ", maxAmp:" << maxAmp << ", baseFreq:" << baseFreq << ", archMod:" << archMod << "\n";

    for (int x = 0; x < WORLDSIZE; x++)
    {
        int X = x - 256;
        int Y = std::sin(X / seaBaseFreq) * seaBaseAmp * randint(100, seaMaxAmp, X / (2 * PI * seaBaseFreq) + seed) / 100.0;;
        Y = Y - (1.0/seaArchMod)*X*X + seaHeight;
        Y = Y < 0 ? 0 : Y;

        for (int y = WORLDSIZE - 1; y > WORLDSIZE - Y; y--)
        {
            setTile(x, y, SEA_WATER);
        }

        int beachHeight = 3 + sin((X + beachOffset) / beachBaseFreq) * beachBaseAmp;
        beachHeight = beachHeight < minBeachHeight ? minBeachHeight : beachHeight;

        for (int y = WORLDSIZE - Y; y > WORLDSIZE - Y - beachHeight; y--)
        {
            setTile(x, y, BEACH);
        }
    }
}

void World::generateLakes()
{
    //Generate lakes
    std::cout <<  "Generating lakes.\n";
    std::vector<Vector2f> lakeCenters;
    int numberOfLakes = 5;
    int lakeSpawnChance = 60;
    float lowestLakeCenter = WORLDSIZE / 2.0;
    float maxLakeRadius = 16;
    float minLakeRadius = 8;
    float secLakeDist = 8;
    float maxDistFromSectorCenter = 48;

    //for lakes, the world is devided into sectors where lakes can spawn
    //every sector has a 5 in 12 chance to spawn a lake near the center
    //sectors are 128x128
    for (int sectorX = 0; sectorX < WORLDSIZE; sectorX += 128)
    {
        for (int sectorY = 0; sectorY < WORLDSIZE - lowestLakeCenter; sectorY += 128)
        {
            if (randint(1, 100) <= lakeSpawnChance)
            {
                int lakex = sectorX + 64 + randint(-maxDistFromSectorCenter, maxDistFromSectorCenter);
                int lakey = sectorY + 64 + randint(-maxDistFromSectorCenter, maxDistFromSectorCenter);

                int secLakex = lakex + randint(-secLakeDist, secLakeDist);
                int secLakey = lakey + randint(-secLakeDist, secLakeDist);

                lakeCenters.push_back(Vector2f(lakex, lakey));
                lakeCenters.push_back(Vector2f(secLakex, secLakey));
                if(randint(1, 2) == 1)
                {
                    lakeCenters.push_back(Vector2f(secLakex + randint(-secLakeDist, secLakeDist),
                                                   secLakey + randint(-secLakeDist, secLakeDist)));
                }
            }
        }
    }

    for (int i = 0; i < lakeCenters.size(); i++)
    {
        int lakex = lakeCenters.at(i).x;
        int lakey = lakeCenters.at(i).y;

        int seed = randint(0,65536);
        int radius = randint(minLakeRadius, maxLakeRadius, seed);

        for (int y = lakey - radius; y < lakey + radius; y++)
        {
            for (int x = lakex - radius; x < lakex + radius; x++)
            {
                int dx = std::fabs(x - lakeCenters.at(i).x);
                int dy = std::fabs(y - lakeCenters.at(i).y);

                float dist = std::sqrt(dx*dx + dy*dy);

                if (dist < radius)
                    setTile(x, y, LAKE_WATER);
            }
        }
    }
}

void World::generateRivers()
{
    std::cout << "Generating river.\n";

    float riverx = WORLDSIZE / 2.0 + randint(-WORLDSIZE / 8, WORLDSIZE / 8, seed);
    float riverFreq = 12.0 + randint(-400, 1400) / 100.0;
    float riverAmp = 2 + randint(-100, 100) / 100.0;
    float riverWidth = 2 + randint(0, 1);
    float riverFreqMod = 64 + randint(-160, 160) / 10.0;
    float riverAmpMod = 10 + randint(-50, 50) / 10.0;
    float flowDirection = randint(-1, 1) < 0 ? -1 : 1;

    for (int y = 0; y < WORLDSIZE; y++)
    {
        for (int w = 0; w < (riverWidth + y / 30.0); w++)
        {
            int Y = y + 1;
            int x = riverx + flowDirection * (sin(Y / (riverFreq + (Y / riverFreqMod))) * (riverAmp + (Y / riverAmpMod)) + w);

            if (getTile(x, y) != SEA_WATER)
                setTile(x, y, RIVER_WATER);
        }
    }
}

void World::generateRock()
{

}

void World::generateTrees()
{

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
            //std::cout << tileDataptr->at(getTile(x, y)).getName() << "\n";
            Color pixelColor = tileDataptr->at(getTile(x, y)).getColor();
            worldMap.setPixel(x, y, pixelColor);
        }
    }

    return worldMap;
}
