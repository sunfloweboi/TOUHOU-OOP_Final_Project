#include "objects.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include <cmath>




Deathsprite::Deathsprite(double x, double y){
    this->x = x;
    this->y = y;
    this->lifetime = 10;
    deathspriteTexture.loadFromFile("images/death.png");
    deathsprite.setTexture(deathspriteTexture);
}

Deathsprite::~Deathsprite(){}

void Deathsprite::draw(sf::RenderWindow &window){
    deathsprite.setPosition(x, y);
    window.draw(deathsprite);
}

Bomb::Bomb(double x, double y){
    this->x = x;
    this->y = y;
    bombTexture.loadFromFile("images/bomb.png");
    bomb.setTexture(bombTexture);
}

Bomb::~Bomb(){}

void Bomb::draw(sf::RenderWindow &window){
    bomb.setPosition(x, y);
    window.draw(bomb);
}

bool checkBombKey(){
    if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        return true;
    return false;
}

bool collision(sf::FloatRect sprite1, sf::FloatRect sprite2)
{
    return sprite1.intersects(sprite2);
}

double degToRad(double deg){
    return deg * pI / 180;
}

double radToDeg(double rad){
    return rad * 180 / pI;
}

double homing(double x1, double y1, double x2, double y2){
    bool reached = false;
    double dx = x1 - x2;
    double dy = y1 - y2;

    if (dx <= 0){
        return atan(dy/dx);
    }
    else if (dx>0){
        return atan(dy/dx) + pI;
    }
}

