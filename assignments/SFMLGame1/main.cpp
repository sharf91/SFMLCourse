#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <memory>
#include "BasicShape.h"

// Each shape has: name, position, speed, color, ...
// The shaps must bounce off the walls of the window that contain them
// Shapes will not start outside window
// Shape name must be placed in the center of the shape
// RECT: Name X Y VX VY R G B W H
// CIRC: Name X Y VX VY R G B R


// TODO: 1. Move creation logic of shapes into BasicShape?
//       2. Update poistion of text
//       3. Update position of shape          
//       4. think about bounding box and how to handle collisions

int main()
{
    std::ifstream fin("config.txt");
    if (!fin)
    {
        std::cerr << "Failed to open config file." << std::endl;
        exit(-1);
    }

    std::string command;
    fin >> command;
    if (command != "Window")
    {
        std::cerr << "First command is now Window." << std::endl;
        exit(-1);
    }

    int windowWidth, windowHeight;
    fin >> windowWidth >> windowHeight;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Bumper Dumper");

    fin >> command;
    std::string fontFile;
    int fontSize, fontR, fontG, fontB;
    if (command != "Font")
    {
        fontFile = "fonts/arial.ttf";
    }
    else
    {
        fin >> fontFile >> fontSize >> fontR >> fontG >> fontB;
    }

    sf::Font myFont;
    if (!myFont.loadFromFile(fontFile)) {
        std::cerr << "Could not load font!" << std::endl;
        exit(-1);
    }
    
    std::vector<std::shared_ptr<BasicShape>> shapes;

    while (fin >> command)
    {
        if (command != "Circle" && command != "Rectangle")
        {
            std::cerr << "Unrecognized shape in config file: " << command << std::endl;
            exit(-1);
        }
        
        std::string name;
        float posX, posY, velX, velY;
        int r, g, b;
        fin >> name >> posX >> posY >> velX >> velY >> r >> g >> b;

        std::shared_ptr<sf::Shape> shape;

        if (command == "Circle")
        {
            float radius;
            fin >> radius;
            shape = std::make_shared<sf::CircleShape>(radius, 40);
        } 
        else if (command == "Rectangle")
        {
            float width, height;
            fin >> width >> height;
            shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(width, height));
        }
        
        shape->setFillColor(sf::Color(r, g, b));

        std::shared_ptr<sf::Text> text = std::make_shared<sf::Text>(name, myFont, fontSize);
        text->setFillColor(sf::Color(fontR, fontG, fontB));

        std::shared_ptr<BasicShape> basicShape = std::make_shared<BasicShape>(shape, text, posX, posY, velX, velY);
        shapes.push_back(basicShape);
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            //if (event.type == sf::Event::KeyPressed) {
            //    if (event.key.code == sf::Keyboard::X)
            //    {
            //        circleMoveSpeed *= -1.0f;
            //    }
            //}
        }

        window.clear();

        for (auto &shape : shapes)
        {
            window.draw(*shape->getShape());
            window.draw(*shape->getName());
        }
        
        window.display();

        for (auto& shape : shapes)
        {
            shape->update();
            shape->handleBounds(0, 0, (float) windowWidth, (float) windowHeight);
        }  
    }

    return 0;
}