#ifndef ENEMY_HPP
#define ENEMY_HPP


#include "objects.hpp"
#include "player.hpp"
#include "bullet.hpp"
#include "point.hpp"
#include <utility>



class Enemy{
    protected:
        double health;
        int score;
        int state;
        double x;
        double y;
        double velocity;
        double acceleration;
        bool alive;
        bool shooting;
        bool shot;
        double bulletCooldown = 0;

        sf::Texture enemyTexture;
        sf::Sprite spriteenemy;

    public:
        Enemy();
        
        Enemy(double x, double y, int state, double acceleration = 0);

        ~Enemy();
        // getters
        double getX();
        double getY();
        int getScore();
        int getState();
        bool getAlive();
        // setters
        void setAlive(bool alive);

        void draw(sf::RenderWindow &window);
        virtual void update(BulletManager* bulletManager, std::vector<Deathsprite*> &deathsprites, Player &player, std::vector<Point*> &pointVec, BulletManager* enemyBulletManager, int &globalScore);

        
};

class Red_Enemy: public Enemy{
    public:
        Red_Enemy(double x, double y, int state);
        ~Red_Enemy();
        void update(BulletManager* bulletManager, std::vector<Deathsprite*> &deathsprites, Player &player, std::vector<Point*> &pointVec, BulletManager* enemyBulletManager, int &globalScore) override;
};

class Black_Enemy: public Enemy{
    public:
        Black_Enemy(double x, double y, int state);
        ~Black_Enemy();
        void update(BulletManager* bulletManager, std::vector<Deathsprite*> &deathsprites, Player &player, std::vector<Point*> &pointVec, BulletManager* enemyBulletManager, int &globalScore) override;
};

class Purple_Enemy: public Enemy{
    public:
        Purple_Enemy(double x, double y, int state);
        ~Purple_Enemy();
        void update(BulletManager* bulletManager, std::vector<Deathsprite*> &deathsprites, Player &player, std::vector<Point*> &pointVec, BulletManager* enemyBulletManager, int &globalScore) override;
};

class Blue_Enemy: public Enemy{
    public:
        Blue_Enemy(double x, double y, int state);
        ~Blue_Enemy();
};

class Miniboss : public Enemy{
    public:
        Miniboss(double x, double y, int state, double acceleration = 0);
        ~Miniboss();
        void update(BulletManager* bulletManager, std::vector<Deathsprite*> &deathsprites, Player &player, std::vector<Point*> &pointVec, BulletManager* enemyBulletManager, int &globalScore) override;

};

class Boss{
    protected:
        int health;
        int score;
        int state;
        double x;
        double y;
        double velocity;
        double angle;
        int bulletCooldown[5] = {0};
        int totalTime = 0;
        int timeCounter = 0;
        int activeBullets;
        int patternCounter = 0;

        sf::Texture enemyTexture;
        sf::Sprite spriteenemy;
    public:
        std::pair<double, double> destination;
        std::pair<double, double> direction;
        Boss();
        ~Boss();
        double getX();
        double getY();
        int getHealth();
        int getState();
        int getTimeCounter();
        void setState(int state);
        void setTimeCounter(int timeCounter);
        void resetCooldown();
        void setDestination(double x, double y);
        void randDestination(double range);
        void goToDestination();
        void draw(sf::RenderWindow &window);
        virtual void update(BulletManager* bulletManager, Player &player, std::vector<BulletManager*> &enemyBulletManager, int activeBullets);
};

class Flan : public Boss{
    private:
        int angle;
    public:
        Flan();
        ~Flan();
        void update(BulletManager* bulletManager, Player &player, std::vector<BulletManager*> &enemyBulletManager, int activeBullets)override;
};




#endif