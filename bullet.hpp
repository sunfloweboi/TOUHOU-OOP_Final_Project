#ifndef BULLET_HPP
#define BULLET_HPP


#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>


enum class bulletType{
    playerBullet,
    fairyBullet,
    bossBullet
};


class Bullet{
    protected:
        double x;
        double y;
        double velocity;
        double acceleration;
        double angle; // in rad
        int state;
        bulletType iAMa;

    public :
        sf::Texture bulletTexture;
        sf::Sprite spritebullet;
        Bullet(double x, double y, double velocity, double angle, sf::Texture bulletTexture);
        ~Bullet();
        double getX();
        double getY();
        bulletType getBulletType();
        void setX(double x);
        void setY(double y);
        void draw(sf::RenderWindow &window);
        virtual void update();
        bool IsOffScreen();
        void setBullet(double x, double y, double velocity, double acceleration, double angle, bulletType iAMa, int state);
        void setBullet(double x, double y, double velocity, double acceleration, double angle, double size, bulletType iAMa, int state);
        void setBullet(double x, double y, double velocity, double acceleration, double angle, sf::Color color, bulletType iAMa, int state);
};

class PlayerBullet : public Bullet{
    public:
        PlayerBullet();
        PlayerBullet(double x, double y, double velocity, double angle, sf::Texture bulletTexture);
        ~PlayerBullet();
        void update() override;
};


class EnemyBullet : public Bullet{
    private:
        bulletType iAMa; //iAMa either be enum fairyBullet or bossBullet
    public:
        EnemyBullet(double x, double y, double velocity, double acceleration, double angle, sf::Texture bulletTexture, bulletType iAMa);
        ~EnemyBullet();
        void update()override;
};

class BulletManager {
    public:
        std::vector<Bullet*> bulletVector;
        std::vector<bool> active;
        sf::Texture bulletTexture;
        BulletManager(sf::Texture bulletTexture);
        BulletManager(sf::Texture bulletTexture, bulletType iAMa, int MAX_ENEMY_BULLETS);
        ~BulletManager();
        BulletManager& operator=(const BulletManager& other);
        Bullet* GetNextAvailableBullet();
        void UpdateBullets();
        void DrawBullets(sf::RenderWindow &window);
};


#endif