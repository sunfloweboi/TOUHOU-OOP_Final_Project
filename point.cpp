#include "point.hpp"
#include "objects.hpp"
#include "player.hpp"
#include <cmath>


Point::Point(double x, double y){
    pointTexture.loadFromFile("images/point.png");
    spritepoint.setTexture(pointTexture);
    spritepoint.setScale(0.5,0.5);
    spritepoint.setPosition(x, y);
    this->x = x;
    this->y = y;
    this->score_value = (rand()%20 + 10) * 10; // ranges bewteen 100 and 300
    
}

Point::~Point(){}

double Point::getX(){
    return x;
}

double Point::getY(){
    return y;
}

sf::FloatRect Point::getGlobalBounds(){
    return spritepoint.getGlobalBounds();
}

int Point::getScoreValue(){
    return score_value;
}

void Point::toggleBombed(){
    if (!bombed){
        bombed = true;
    }
}

void Point::draw(sf::RenderWindow &window){
    window.draw(spritepoint);
}

void Point::update(Player &player){
    if (!bombed){
        y += 7;
    }
    else{
        x += std::cos(homing(x, y, player.getX(), player.getY())) * 5;
        y += std::sin(homing(x, y, player.getX(), player.getY())) * 5;
    }
    spritepoint.setPosition(x, y);
}