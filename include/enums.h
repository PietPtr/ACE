#ifndef ENUMS_H_INCLUDED
#define ENUMS_H_INCLUDED

enum TileName
{
    GRASS,          //0
    TREE,           //1
    SEA_WATER,      //2
    LAKE_WATER,     //3
    RIVER_WATER,    //4
    BEACH,          //5
    EDGE,           //6
    ROCK            //7
};

enum Overlay
{
    BOTTOMLEFT = 8,
    BOTTOMRIGHT,
    TOPLEFT,
    TOPRIGHT,
    DOWN,
    LEFT,
    RIGHT,
    UP,
    DOWN_LEFT,
    DOWN_RIGHT,
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT_RIGHT,
    UP_DOWN_LEFT,
    UP_DOWN_RIGHT,
    UP_RIGHT_LEFT,
    NONE,
};

#endif // ENUMS_H_INCLUDED
