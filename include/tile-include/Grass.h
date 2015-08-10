#ifndef GRASS_H
#define GRASS_H
#include <SFML/Graphics.hpp>
#include "Tile.h"

using namespace sf;

class Grass: public Tile
{
    public:
        Grass();
    protected:
    private:
        int textureIndex = 1;
        Vector2f offset { 0, 0 };
        Vector2f tileSize { 64, 64 };
        Vector2f unwalkableOffset { 0, 0 };
        Vector2f unwalkableSize { 0, 0 };
};

#endif // GRASS_H
