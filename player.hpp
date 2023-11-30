#ifndef PLAYER_HPP
#define PLAYER_HPP


#include "bullet.hpp"
#include "objects.hpp"


#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>


class Player{
    private:
        int lives;
        double player_velocity;
        double x;
        double y;
        int bombcount;
        int bulletCooldown = 0;
        bool invincibility = false;
        int invincibilityCooldown = 0;
        int damageOnBoss = 0;

        sf::Texture playerTexture;
        sf::Texture hitbox;

        sf::Sprite spriteplayer;
        sf::Sprite hitboxsprite;

    public:
        std::vector<Bomb*> bombs;
        BulletManager* bulletManager;

        Player();
        //getters
        double getX();
        double getY();
        int getLives();
        bool getInvincibility();
        int getInvincibilityCooldown();
        sf::FloatRect getGlobalBounds();
        int getBombCount();
        int getDamageOnBoss();
        void damageDealt();
        void setLife(int life);
        void setBombCount(int bombcount);
        void incrementBombCount();

        //setters
        void hpDecrease();
        void shoot(int state);
        void draw(sf::RenderWindow &window);
        void update(int score);
        void invincible();


};


#endif