//This file stores small class objects and also functions that are not in-class


#ifndef OBJECTS_HPP
#define OBJECTS_HPP


#include <iostream>
#include <string>
#include <vector>
#include<array>
#include <SFML/Graphics.hpp>

const int WIDTH = 600;
const int HEIGHT = 900;
const double pI = 3.14159265358979323846;

class Deathsprite{
    public:
        double x;
        double y;
        int lifetime;  

        sf::Texture deathspriteTexture;
        sf::Sprite deathsprite;

        Deathsprite(double x, double y);
        ~Deathsprite();
        void draw(sf::RenderWindow &window);
        
       
};

class Bomb{
    public:
        double x;
        double y; 

        sf::Texture bombTexture;
        sf::Sprite bomb;

        Bomb(double x, double y);
        ~Bomb();
        void draw(sf::RenderWindow &window);
};

bool checkBombKey();

bool collision(sf::FloatRect sprite1, sf::FloatRect sprite2);

double degToRad(double deg);

double radToDeg(double rad);

double homing(double x1, double y1, double x2, double y2);


#endif