#include <SFML/Graphics.hpp>
#include <iostream>
#include "Audio.h"
#include "Tile.h"
#include "World.h"

using namespace sf;

int randint(int low, int high, int seed);
int randint(int low, int high);

class Game
{
    public:
        Game(RenderWindow* window);
        void initialize();
        void update();
        void draw();
        bool isWindowOpen();

        void loadAudio(std::vector<std::string> fileNames);
        void loadTextures();
        void loadTileData();
    protected:
    private:
        RenderWindow* window;

        Time dt;
        Time totalTime;
        Clock clock;
        int frame = 0;
        bool showFPS = false;

        int windowWidth = 1280;
        int windowHeight = 720;

        View view;

        std::vector<std::string> audioFileNames { "first.wav", "second.wav" };

        std::vector<Tile> tileData;
        std::vector<Audio*> sfx;
        std::vector<Texture*> txt;

        Vector2f viewPos { 0, 0 };

        World world { randint(0, 65536), &tileData, &txt, window };
};
