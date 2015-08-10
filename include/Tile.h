#include <iostream>
#include <SFML/Graphics.hpp>
#ifndef TILE_H
#define TILE_H

using namespace sf;

class Tile
{
    public:
        Tile(std::string name, int textureIndex, Vector2f offset, Vector2f tileSize, Vector2f unwalkableOffset, Vector2f unwalkableSize, Color color);
        std::string getName() { return name; }
        Color getColor() { return color; }
    protected:
    private:
        std::string name;
        int textureIndex; //index of game->txt to use
        Vector2f offset;
        Vector2f tileSize;
        Vector2f unwalkableOffset;
        Vector2f unwalkableSize;
        Color color;
};

#endif // TILE_H
