#include "World.h"
#include "enums.h"
#include "SFML/Graphics.hpp"
#include "time.h"
#include <fstream>

using namespace sf;

int randint(int low, int high, int seed);
int randint(int low, int high);

const double PI = 3.141592654;

World::World()
 {

 }

void World::initialize(int _seed, std::vector<Tile>* _tileDataptr, std::vector<Texture>* _txtptr, RenderWindow* _window)
{
    seed = _seed;
    tileDataptr = _tileDataptr;
    txtptr = _txtptr;
    window = _window;
}

void World::setTile(int x, int y, TileName tile)
{
    x = x < 0 ? 0 : x;
    y = y < 0 ? 0 : y;
    x = x > WORLDSIZE-1 ? WORLDSIZE-1 : x;
    y = y > WORLDSIZE-1 ? WORLDSIZE-1 : y;

    //std::cout << "(" << x << "," << y << ") " << tile << ", " << y * WORLDSIZE + x << ", " << (WORLDSIZE * WORLDSIZE) - (y * WORLDSIZE + x) << "\n";

    tiles[y * WORLDSIZE + x] = (int)tile;
}

TileName World::getTile(int x, int y)
{
    if (x < 0 || x >= WORLDSIZE || y < 0 || y >= WORLDSIZE)
    {
        return EDGE;
    }
    else
    {
        return (TileName)tiles[y * WORLDSIZE + x];
    }
}

void World::generateWorld()
{
    std::cout << "Generating world with seed " << seed << ".\n";
    generateGrass();
    generateTrees();
    generateRock();
    generateSea();
    generateLakes();
    generateRivers();
    generationCleanup();
}

void World::generateTrees()
{
    /*for (int y = 0; y < WORLDSIZE; y++)
    {
        for (int x = 0; x < WORLDSIZE; x++)
        {
            if (randint(0, 100) <= 1)
            {
                setTile(x, y, TREE);
            }
        }
    }*/
    float numberOfForests = 50;
    float maxTreeDistance = 16;

    for (int f = 0; f < numberOfForests; f++)
    {
        int x = randint(0, WORLDSIZE, f * f * seed);
        int y = randint(0, WORLDSIZE, (f+1) * seed);

        setTile(x, y, TREE);

        int forestSize = maxTreeDistance + randint(-4, 4, seed+f);

        for (int t = 0; t < forestSize * 2; t++)
        {
            int X = x + randint(-forestSize, forestSize, seed / (t+f+1024) - t);
            int Y = y + randint(-forestSize, forestSize, seed * (t+f) + f);

            setTile(X, Y, TREE);
        }
    }

}

void World::generateGrass()
{
    //Set every tile to grass
    for (int y = 0; y < WORLDSIZE; y++)
    {
        for (int x = 0; x < WORLDSIZE; x++)
        {
            setTile(x, y, GRASS);
        }
    }
}

void World::generateRock()
{
    std::vector<Vector2f> rockCenters;
    int numberOfRock = 15;
    float maxRockRadius = 8;
    float minRockRadius = 4;
    float secRocksDist = 3;
    float maxSecondaryRock = 16;

    for (int i = 0; i < numberOfRock; i++)
    {
        Vector2f previousRockPos(randint(0, WORLDSIZE, i * i * seed), randint(0, WORLDSIZE, (i + 1) * seed));

        for (int r = 0; r < maxSecondaryRock - randint(0, 3, seed); r++)
        {
            previousRockPos.x += randint(-secRocksDist, secRocksDist, i + seed * i);
            previousRockPos.y += randint(-secRocksDist, secRocksDist, i - seed * (r + 1));

            rockCenters.push_back(previousRockPos);
        }
        //std::cout << "X: " << previousRockPos.x << ", Y: " << previousRockPos.y << "\n";
    }

    for (int i = 0; i < rockCenters.size(); i++)
    {
        int radius = randint(minRockRadius, maxRockRadius, seed * i);



        generateCircle(rockCenters.at(i), radius, ROCK);
    }

}

void World::generateSea()
{
    //Generate the sea

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
            if (randint(1, 100) <= lakeSpawnChance || (sectorX == 0 && sectorY == 0))
            {
                int lakex = sectorX + 64 + randint(-maxDistFromSectorCenter, maxDistFromSectorCenter);
                int lakey = sectorY + 64 + randint(-maxDistFromSectorCenter, maxDistFromSectorCenter);

                int secLakex = lakex + randint(-secLakeDist, secLakeDist, seed + lakex + lakey);
                int secLakey = lakey + randint(-secLakeDist, secLakeDist, seed + lakex + lakey);

                lakeCenters.push_back(Vector2f(lakex, lakey));
                lakeCenters.push_back(Vector2f(secLakex, secLakey));
                if(randint(1, 2) == 1)
                {
                    lakeCenters.push_back(Vector2f(secLakex + randint(-secLakeDist, secLakeDist, seed + lakex + lakey),
                                                   secLakey + randint(-secLakeDist, secLakeDist, seed + lakex + lakey)));
                }
            }
        }
    }

    for (int i = 0; i < lakeCenters.size(); i++)
    {
        int radius = randint(minLakeRadius, maxLakeRadius, seed * i);

        generateCircle(lakeCenters.at(i), radius, LAKE_WATER);
    }
}

void World::generateRivers()
{

    float riverx = WORLDSIZE / 2.0 + randint(-WORLDSIZE / 8, WORLDSIZE / 8, seed);  //x position of the river
    float riverFreq = 12.0 + randint(-400, 1400, seed) / 100.0;                           //base frequency of the sinus wave
    float riverAmp = 2 + randint(-100, 100, seed) / 100.0;                                //base amplitude of the sinus wave
    float riverWidth = 2 + randint(0, 1, seed);                                           //base width of the river
    float riverFreqMod = 64 + randint(-160, 160, seed) / 10.0;                            //rate at which the frequency changes (y / value)
    float riverAmpMod = 12 + randint(0, 50, seed) / 10.0;                                 //rate at which the amplitude changes (y / value)
    float riverWidthMod = 30.0 + randint(-50, 50, seed) / 10.0;                           //rate at which the river widens (y / value)
    float flowDirection = randint(-1, 1, seed) < 0 ? -1 : 1;                              //whether the river first goes left or right
    float riverSideWidth = 16;
    float riverStreamOffsetMod = (10.0 + randint(-300, 700, seed) / 100.0) * flowDirection;

    for (int y = 0; y < WORLDSIZE; y++)
    {
        for (int w = -riverSideWidth; w < (riverWidth + y / riverWidthMod) + riverSideWidth; w++)
        {
            int Y = y + 1;
            int x = riverx + flowDirection * (sin(Y / (riverFreq + (Y / riverFreqMod))) * (riverAmp + (Y / riverAmpMod)) + w) + (y / riverStreamOffsetMod);

            if (w < (riverWidth + y / riverWidthMod) && w >= 0)
            {
                if (getTile(x, y) != SEA_WATER)
                {
                    setTile(x, y, RIVER_WATER);
                }
            }
            else
            {
                if (getTile(x, y) == LAKE_WATER)
                    setTile(x, y, GRASS);
            }
        }
    }
}

void World::generateCircle(Vector2f position, int radius, TileName tile)
{
    for (int y = position.y - radius; y < position.y + radius; y++)
    {
        for (int x = position.x - radius; x < position.x + radius; x++)
        {
            int dx = std::fabs(x - position.x);
            int dy = std::fabs(y - position.y);

            float dist = std::sqrt(dx*dx + dy*dy);

            if (dist < radius)
                setTile(x, y, tile);
        }
    }
}

void World::generationCleanup()
{
    //remove some rough edges from lakes
    for (int i = 0; i < 3; i++)
    {
        for (int y = 0; y < WORLDSIZE; y++)
        {
            for (int x = 0; x < WORLDSIZE; x++)
            {
                if (getTile(x, y) == LAKE_WATER)
                {
                    int surroundingNonLakeTiles = 0;
                    if (getTile(x + 1, y) != LAKE_WATER)
                        surroundingNonLakeTiles++;
                    if (getTile(x - 1, y) != LAKE_WATER)
                        surroundingNonLakeTiles++;
                    if (getTile(x, y + 1) != LAKE_WATER)
                        surroundingNonLakeTiles++;
                    if (getTile(x, y - 1) != LAKE_WATER)
                        surroundingNonLakeTiles++;

                    if (surroundingNonLakeTiles > 2)
                        setTile(x, y, GRASS);
                }
            }
        }
    }
}

void World::draw(Vector2f position, Vector2f viewDistance)
{
    int X = ((position.x - ((int)position.x % 64)) / 64) - (viewDistance.x);
    int Y = ((position.y - ((int)position.y % 64)) / 64) - (viewDistance.y);

    for (int y = Y; y <  Y + viewDistance.y * 2; y++)
    {
        for(int x = X; x < X + viewDistance.x * 2; x++)
        {
            TileName tile = getTile(x, y);

            Sprite tileSprite;

            tileSprite.setTexture(txtptr->at(tileDataptr->at(tile).getTextureIndex()));
            tileSprite.setPosition(x * 64 + tileDataptr->at(tile).getOffset().x, y * 64 + tileDataptr->at(tile).getOffset().y);
            window->draw(tileSprite);
        }
    }
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
