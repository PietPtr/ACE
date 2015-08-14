#include <SFML/Graphics.hpp>
#include "Game.h"
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
    world.initialize(randint(0, 65536), &tileData, &txt, window);
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
                world.getWorldMap(viewPos).saveToFile("screenshots/map" + std::to_string(world.getSeed()) + ".png");
                world.setSeed(randint(0, 65536));
                world.generateWorld();
            }
            if (event.key.code == Keyboard::F)
            {
                showFPS = !showFPS;
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

    if (showFPS && frame % 30 == 0)
    {
        std::cout << 1 / dt.asSeconds() << "\n";
    }

    float SPEED = 4096;
    if (Keyboard::isKeyPressed(Keyboard::W))
        playerPos.y -= SPEED * dt.asSeconds();
    if (Keyboard::isKeyPressed(Keyboard::A))
        playerPos.x -= SPEED * dt.asSeconds();
    if (Keyboard::isKeyPressed(Keyboard::S))
        playerPos.y += SPEED * dt.asSeconds();
    if (Keyboard::isKeyPressed(Keyboard::D))
        playerPos.x += SPEED * dt.asSeconds();

    std::cout << playerPos.x << " " << playerPos.y << "\n";

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



    Vector2f viewDistance;
    viewDistance.x = (windowWidth / TILESIZE / 2) + 4;
    viewDistance.y = (windowHeight / TILESIZE / 2) + 4;

    if (Keyboard::isKeyPressed(Keyboard::Period))
    {
        int zoom = 16;
        view.zoom(zoom);
        viewDistance.x *= zoom;
        viewDistance.y *= zoom;
    }


    window->setView(view);

    world.draw(playerPos, viewDistance);

    if (Keyboard::isKeyPressed(Keyboard::M))
    {
        Texture mapTexture;
        mapTexture.loadFromImage(world.getWorldMap((Vector2f)playerPos));
        Sprite worldmap;
        worldmap.setPosition(Vector2f(-WORLDSIZE / 2, -WORLDSIZE / 2));
        worldmap.setTexture(mapTexture);
        window->draw(worldmap);
    }

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

    std::sort(textureNames.begin(), textureNames.end());

    for (int i = 0; i < textureNames.size(); i++)
    {
        Texture loadedTexture;
        if (!loadedTexture.loadFromFile("textures/" + textureNames[i]))
            window->close();

        txt.push_back(loadedTexture);
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

    std::sort(textFileNames.begin(), textFileNames.end());

    for (int i = 0; i < textFileNames.size(); i++)
    {
        std::cout << textFileNames.at(i) << "\n";
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


