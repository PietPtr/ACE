#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Chunk.h"
#include "World.h"
#include <dirent.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <cmath>

using namespace sf;
void celShade(Sprite sprite, RenderWindow* window, Color shadeColor);
int randint(int low, int high, int seed);
int randint(int low, int high);

Game::Game(RenderWindow* _window)
{
    window = _window;
}

void Game::initialize()
{
    loadAudio(audioFileNames);
    loadTextures();
    loadTileData();
    world.generateWorld(); //loadworld
}

void Game::update()
{
    Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window->close();
        if (event.type == Event::KeyPressed)
        {
            if (event.key.code == Keyboard::Escape)
            {
                window->close();
            }
            if (event.key.code == Keyboard::P)
            {
                sfx.at(1)->play();
                world.getWorldMap().saveToFile("screenshots/map" + std::to_string(totalTime.asMicroseconds()) + ".png");
            }
            if (event.key.code == Keyboard::F)
            {
                showFPS = !showFPS;
                window->setFramerateLimit(1);
            }
        }
        if (event.type == Event::Resized)
        {
            //sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            windowWidth = event.size.width;
            windowHeight = event.size.height;
            //window->setView(View(visibleArea));
        }
    }

    dt = clock.restart();
    totalTime += dt;

    if (showFPS)
    {
        std::cout << 1 / dt.asSeconds() << "\n";
    }

    float SPEED = 16;
    if (Keyboard::isKeyPressed(Keyboard::W))
        viewPos.y -= SPEED;
    if (Keyboard::isKeyPressed(Keyboard::A))
        viewPos.x -= SPEED;
    if (Keyboard::isKeyPressed(Keyboard::S))
        viewPos.y += SPEED;
    if (Keyboard::isKeyPressed(Keyboard::D))
        viewPos.x += SPEED;

    frame++;
}

void Game::draw()
{
    window->clear();
    //window->clear(Color(55, 55, 255));
    //view = new View(FloatRect(0, 0, windowWidth, windowHeight))
    view.setSize(Vector2f(windowWidth, windowHeight));;
    view.setCenter(viewPos);
    //view.zoom(0.5);
    if (Keyboard::isKeyPressed(Keyboard::Period))
    {
        view.zoom(2);
    }

    window->setView(view);

    /*Sprite test;
    test.setTexture(*txt.at(0));
    //window->draw(test);

    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 24; x++)
        {
            test.setPosition(Vector2f(x * 64, y * 64));
            window->draw(test);
        }
    }

    Sprite test2;
    test2.setTexture(*txt.at(1));
    //window->draw(test);

    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 24; x++)
        {
            if (randint(0, 5, x * y) == 0)
                test2.setPosition(Vector2f(x * 64, y * 64));
            //celShade(test2, window, Color(0, 0, 0));
            window->draw(test2);
        }
    }*/

    /*Sprite pix;
    pix.setTexture(*txt.at(0));
    int zoom = 64;
    int worldSeed = 4872634;
    pix.setScale(1.0/zoom, 1.0/zoom);
    //window->draw(pix);
    int bound = 1024;
    for(int x = -bound; x < bound; x++)
    {
        int X = -x;
        //std::cout << X << "\n";
        //int Y = -(1.0/128)*X*X;
        int Y = std::sin(X / 14.0) * 3 * (randint(100, 300, x / (87.96) + worldSeed) / 100.0);
        //std::cout << (randint(100, 200, x / 63) / 100.0) << "\n";
        Y = Y + (1.0/1024)*X*X;
        pix.setPosition(Vector2f(x, Y));
        window->draw(pix);
    }*/

    window->display();
}

bool Game::isWindowOpen()
{
    return window->isOpen();
}

void Game::loadAudio(std::vector<std::string> fileNames)
{
    for (int i = 0; i < fileNames.size(); i++)
    {
        sfx.push_back(new Audio());
        sfx.back()->init(fileNames[i]);
    }
}

void Game::loadTextures()
{
    /*for (int i = 0; i < fileNames.size(); i++)
    {
        txt.push_back(new Texture());
        if(!txt.back()->loadFromFile(fileNames[i]))
            window->close();
    }*/

    DIR *dir;
    struct dirent *ent;
    dir = opendir("textures/");

    std::vector<std::string> textureNames;

    if (dir != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (std::string(ent->d_name) != "." && std::string(ent->d_name) != "..")
            {
                textureNames.push_back(std::string(ent->d_name));
            }
        }
        closedir(dir);
    }
    else
    {
        window->close();
    }
    for (int i = 0; i < textureNames.size(); i++)
    {
        txt.push_back(new Texture());
        if (!txt.back()->loadFromFile("textures/" + textureNames[i]))
            window->close();
    }
}

void Game::loadTileData()
{
    /*char temp [ PATH_MAX ];
    std::string path;

    if (getcwd(temp, PATH_MAX) != 0)
        path = std::string(temp);
    */
    DIR *dir;
    struct dirent *ent;
    dir = opendir("tile-data/");

    std::vector<std::string> textFileNames;

    if (dir != NULL)
    {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL)
        {
            if (std::string(ent->d_name) != "." && std::string(ent->d_name) != "..")
            {
                //std::cout << ent->d_name << "\n";
                textFileNames.push_back(std::string(ent->d_name));
            }

        }
        closedir (dir);
    }
    else
    {
        /* could not open directory */
        window->close();
    }

    for (int i = 0; i < textFileNames.size(); i++)
    {
        //std::ifstream file("/tile-data/grass");// + textFileNames.at(i));
        std::string filename = "tile-data/" + textFileNames.at(i);
        std::ifstream file(filename);

        std::vector<std::string> rawData;

        while(!file.eof())
        {
            std::string line;
            std::getline(file, line);
            if (line.length() >= 1)
            {
                rawData.push_back(line);
            }
        }
        file.close();

        std::string name = rawData.at(0);
        int textureIndex =       std::stoi(rawData.at(1), nullptr);
        int offsetX =            std::stoi(rawData.at(2), nullptr);
        int offsetY =            std::stoi(rawData.at(3), nullptr);
        int tileSizeX =          std::stoi(rawData.at(4), nullptr);
        int tileSizeY =          std::stoi(rawData.at(5), nullptr);
        int unwalkableOffsetX =  std::stoi(rawData.at(6), nullptr);
        int unwalkableOffsetY =  std::stoi(rawData.at(7), nullptr);
        int unwalkableSizeX =    std::stoi(rawData.at(8), nullptr);
        int unwalkableSizeY =    std::stoi(rawData.at(9), nullptr);
        int r =                  std::stoi(rawData.at(10), nullptr);
        int g =                  std::stoi(rawData.at(11), nullptr);
        int b =                  std::stoi(rawData.at(12), nullptr);

        tileData.push_back(Tile(name, textureIndex, Vector2f(offsetX, offsetY),
                                Vector2f(tileSizeX, tileSizeY),
                                Vector2f(unwalkableOffsetX, unwalkableOffsetY),
                                Vector2f(unwalkableSizeX, unwalkableSizeY),
                                Color(r, g, b)));
    }
}


