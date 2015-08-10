#include "Chunk.h"
#include "enums.h"

using namespace sf;

Chunk::Chunk()
{

}

void Chunk::setTile(int x, int y, TileName tile)
{
    tiles[y * 16 + x] = tile;
}

TileName Chunk::getTile(int x, int y)
{
    return (TileName)tiles[y * 16 + x];
}
