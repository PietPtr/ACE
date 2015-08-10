#ifndef CHUNK_H
#define CHUNK_H
#include "Tile.h"
#include "enums.h"

using namespace sf;

class Chunk
{
    public:
        Chunk();
        void setTile(int x, int y, TileName tile);
        TileName getTile(int x, int y);
    protected:
    private:
        int tiles[16*16]; //the value is the index of tileData
};

#endif // CHUNK_H
