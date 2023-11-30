#include "bullet.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "objects.hpp"

#include <cmath>


const int MAX_PLAYER_BULLETS = 50;


Bullet::Bullet(double x, double y, double velocity, double angle, sf::Texture bulletTexture){
    this->x = x;
    this->y = y;
    this->velocity = velocity;
    this->angle = angle;
    this->bulletTexture = bulletTexture;
    spritebullet.setTexture(this->bulletTexture);
    sf::Color spriteColor = spritebullet.getColor();
    spriteColor.a = 192; // Set alpha to 192 (75% opacity)
    spritebullet.setColor(spriteColor);
}

Bullet::~Bullet(){}

double Bullet::getX(){
    return x;
}

double Bullet::getY(){
    return y;
}

bulletType Bullet::getBulletType(){
    return iAMa;
}

void Bullet::setX(double x){
    this->x = x;
}

void Bullet::setY(double y){
    this->y = y;
}

void Bullet::draw(sf::RenderWindow &window){
    spritebullet.setPosition(round(x), round(y));
    window.draw(spritebullet);
}


void Bullet::update(){
    x += std::cos(angle) * velocity;
    y += std::sin(angle) * velocity;
}

bool Bullet::IsOffScreen() {
    if (x < -100 || x > WIDTH+100 || y < -100 || y > HEIGHT+100) {
        return true;
    }
    return false;
}

void Bullet::setBullet(double x, double y, double velocity, double acceleration, double angle, bulletType iAMa, int state){
    this->x = x;
    this->y = y;
    this->velocity = velocity;
    this->acceleration = acceleration;
    this->angle = angle;
    this->iAMa = iAMa;
    this->state = state;
}

void Bullet::setBullet(double x, double y, double velocity, double acceleration, double angle, double size, bulletType iAMa, int state){
    this->x = x;
    this->y = y;
    this->velocity = velocity;
    this->acceleration = acceleration;
    this->angle = angle;
    this->iAMa = iAMa;
    this->state = state;
    spritebullet.setScale(size, size);
}

void Bullet::setBullet(double x, double y, double velocity, double acceleration, double angle, sf::Color color, bulletType iAMa, int state){
    this->x = x;
    this->y = y;
    this->velocity = velocity;
    this->acceleration = acceleration;
    this->angle = angle;
    this->iAMa = iAMa;
    this->state = state;
    spritebullet.setColor(color);
}

PlayerBullet::PlayerBullet(double x, double y, double velocity, double angle, sf::Texture bulletTexture) : Bullet(x, y, velocity, angle, bulletTexture){
    acceleration = 0;
    state = 1;
}

PlayerBullet::~PlayerBullet(){}

void PlayerBullet::update(){
    y -= velocity;
}

EnemyBullet::EnemyBullet(double x, double y, double velocity, double acceleration, double angle, sf::Texture bulletTexture, bulletType iAMa) : Bullet(x, y, velocity, angle, bulletTexture){
    this->acceleration = acceleration;
    this->iAMa = iAMa;
}


EnemyBullet::~EnemyBullet(){}

void EnemyBullet::update(){
    Bullet::update();
    if (acceleration != 0){
        velocity += acceleration;
    }
    if (iAMa == bulletType::fairyBullet && state == 2 && velocity < 0.5){
        velocity = 0.5;
        acceleration = 0;
    }
    if (iAMa == bulletType::bossBullet){
        if (state == 1){
            acceleration -= acceleration*0.1;
        }
        spritebullet.setRotation(radToDeg(angle) + 90);
    }
}


//Bullet Manager

// for player bullets
BulletManager::BulletManager(sf::Texture bulletTexture) {
    bulletVector.resize(MAX_PLAYER_BULLETS); // Resize the vector
    active.resize(MAX_PLAYER_BULLETS); // Resize the vector

    for (int i = 0; i < MAX_PLAYER_BULLETS; ++i) {
        bulletVector[i] = new PlayerBullet(WIDTH+100, HEIGHT+100, 0, 0, bulletTexture);
        active[i] = false;
    }
    std::cout << "Player's BulletManager constructor called : Success : " << bulletVector.size() << std::endl;
}

// for enemy bullets
BulletManager::BulletManager(sf::Texture bulletTexture, bulletType iAMa, int MAX_ENEMY_BULLETS) {
    bulletVector.resize(MAX_ENEMY_BULLETS); // Resize the vector
    active.resize(MAX_ENEMY_BULLETS); // Resize the vector

    for (int i = 0; i < MAX_ENEMY_BULLETS; ++i) {
        bulletVector[i] = new EnemyBullet(WIDTH+100, HEIGHT+100, 0, 0, 0, bulletTexture, iAMa);
        active[i] = false;
    }
    
    std::cout << "Enemy's BulletManager constructor called : Success : " << bulletVector.size() << std::endl;
}

BulletManager::~BulletManager() {
    for (int i = 0; i < bulletVector.size(); ++i) {
        delete bulletVector[i];
    }
}

BulletManager& BulletManager::operator=(const BulletManager& other) {
    if (this == &other) {
        return *this;
    }

    // Perform the assignment of member variables
    bulletVector.clear();
    bulletVector.reserve(other.bulletVector.size());
    for (const auto* bullet : other.bulletVector) {
        bulletVector.push_back(new Bullet(*bullet));
    }

    active = other.active;
    bulletTexture = other.bulletTexture;

    return *this;
}

Bullet* BulletManager::GetNextAvailableBullet() {
    for (int i = 0; i < bulletVector.size(); ++i) {
        if (!active[i]) {
            active[i] = true;
            return bulletVector[i];
        }
    }
    bulletVector.push_back(new PlayerBullet(WIDTH+100, HEIGHT+100, 0, 0, bulletTexture));
    active.push_back(true);
    return bulletVector[bulletVector.size() - 1];
}

void BulletManager::UpdateBullets() {
    for (int i = 0; i < bulletVector.size(); ++i) {
        if (active[i]) {
            bulletVector[i]->update();
            if (bulletVector[i]->IsOffScreen()) {
                active[i] = false;
            }
        }
    }
}

void BulletManager::DrawBullets(sf::RenderWindow &window) {
    for (int i = 0; i < bulletVector.size(); ++i) {
        //if (active[i]) {
            bulletVector[i]->draw(window);
        //}
    }
}