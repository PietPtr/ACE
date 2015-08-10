#include "Tile.h"

using namespace sf;

Tile::Tile(std::string _name, int _textureIndex, Vector2f _offset, Vector2f _tileSize, Vector2f _unwalkableOffset, Vector2f _unwalkableSize, Color _color)
{
    name = _name;
    textureIndex = _textureIndex;
    offset = _offset;
    tileSize = _tileSize;
    unwalkableOffset = _unwalkableOffset;
    unwalkableSize = _unwalkableSize;
    color = _color;
}
