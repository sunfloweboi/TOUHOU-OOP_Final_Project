#ifndef POINT_HPP
#define POINT_HPP

#include "objects.hpp"


//point objecct to be collected by player
class Point{
    private:
        double x;
        double y;
        int score_value;
        bool bombed = false;
        sf::Texture pointTexture;
        sf::Sprite spritepoint;

    public:
        Point(double x, double y);
        ~Point();
        double getX();
        double getY();
        sf::FloatRect getGlobalBounds();
        int getScoreValue();
        void toggleBombed();
        void draw(sf::RenderWindow &window);
        void update(Player &player);
};


#endif