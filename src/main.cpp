#include <SFML/Graphics.hpp>
#include "Game.h"
#include <iostream>

using namespace sf;
int main()
{
    srand(time(NULL));

    int windowWidth = 1280;
    int windowHeigth = 720;
    RenderWindow window(VideoMode(windowWidth, windowHeigth), "Float");
    //window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    Game game(&window);

    game.initialize();

    while (window.isOpen())
    {
        game.update();
        game.draw();
    }

    return 0;
}

int randint(int low, int high, int seed)
{
    srand(seed);
    int value = rand() % (high + 1 - low) + low;
    return value;
}

int randint(int low, int high)
{
    int value = rand() % (high + 1 - low) + low;
    //srand(value * rand());

    return value;
}

void celShade(Sprite sprite, RenderWindow* window, Color shadeColor)
{
    sprite.setColor(shadeColor);
    Vector2f spritePos = sprite.getPosition();
    sprite.setPosition(Vector2f(spritePos.x - 1, spritePos.y));
    window->draw(sprite);
    sprite.setPosition(Vector2f(spritePos.x + 1, spritePos.y));
    window->draw(sprite);
    sprite.setPosition(Vector2f(spritePos.x, spritePos.y - 1));
    window->draw(sprite);
    sprite.setPosition(Vector2f(spritePos.x, spritePos.y + 1));
    window->draw(sprite);
}
