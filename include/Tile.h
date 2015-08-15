#include <iostream>
#include <SFML/Graphics.hpp>
#ifndef TILE_H
#define TILE_H

using namespace sf;

class Tile
{
    public:
        Tile(std::string name, int textureIndex, Vector2f offset, Vector2f tileSize, Vector2f unwalkableOffset,
             Vector2f unwalkableSize, Color color, int numberOfFrames, int frameTime);
        std::string getName() { return name; }
        Color getColor() { return color; }
        int getTextureIndex() { return textureIndex; }
        Vector2f getOffset() { return offset; }
        Vector2f getTileSize() { return tileSize; }
        Vector2f getUnwalkableOffset() { return unwalkableOffset; }
        Vector2f getUnwalkableSize() { return unwalkableSize; }
        int getNumberOfFrames() { return numberOfFrames; }
        int getFrameTime() { return frameTime; }
    protected:
    private:
        std::string name;
        int textureIndex = 0; //index of game->txt to use
        Vector2f offset;
        Vector2f tileSize;
        Vector2f unwalkableOffset;
        Vector2f unwalkableSize;
        Color color;
        int numberOfFrames = 0;
        int frameTime = 0;
};

#endif // TILE_H
