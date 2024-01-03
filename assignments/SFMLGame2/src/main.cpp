//#include <SFML/Graphics.hpp>

#include <iostream>

#include "Game.h"

/**
* Winbdow W H FPS FullScreen
* Player ShapeRad CollRad Speed(pixels) Color(R, G, B) BorderColor(R, G, B) BorderSize NumVerts
* Enemy  ShapeRad CollRad SpeedMin SpeedMax BorderColor(R, G, B) BorderSize VertMin VertMax Lifespan(seconds) SpawnInterval(seconds)
* Bullet ShapeRad CollRad Speed Color(R, G, B) BorderColor(R, G, B) BorderSize NumVerts Lifespan
*/
int main()
{
    Game g("config.txt");
    g.run();

    return 0;
}