#include "player.hpp"
#include "enemy.hpp"
#include "bullet.hpp"
#include "objects.hpp"

#include <cmath>


Player::Player()
{
    bombcount = 4;
    lives = 10;
    player_velocity = 5;
    playerTexture.loadFromFile("images/player.png");
    spriteplayer.setTexture(playerTexture);
    hitbox.loadFromFile("images/redsquare.png");
    hitboxsprite.setTexture(hitbox);
    hitboxsprite.setScale(0.006, 0.006);
    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("images/bullet.png");
    x = 400;
    y = HEIGHT-100;
    bulletManager = new BulletManager(bulletTexture);
}

double Player::getX()
{
    return x;
}

double Player::getY()
{
    return y;
}

int Player::getLives()
{
    return lives;
}

bool Player::getInvincibility()
{
    return invincibility;
}

int Player::getInvincibilityCooldown()
{
    return invincibilityCooldown;
}

sf::FloatRect Player::getGlobalBounds()
{
    return hitboxsprite.getGlobalBounds();
}

int Player::getBombCount()
{
    return bombcount;
}

int Player::getDamageOnBoss()
{
    return damageOnBoss;
}

void Player::damageDealt()
{
    damageOnBoss++;
}

void Player::setLife(int life)
{
    this->lives = life;
}

void Player::setBombCount(int bombcount)
{
    this->bombcount = bombcount;
}

void Player::incrementBombCount()
{
    bombcount++;
}

void Player::hpDecrease()
{
    lives--;
    std::cout << "HP: " << lives << std::endl;
}

void Player::shoot(int state)
{
    if (bulletCooldown <= 0)
    {
        bulletCooldown = 5;
        switch (state)
        {
        case 1:
        //normal shoot
            bulletManager->GetNextAvailableBullet()->setBullet(x + 6, y, 25, 0, 0, bulletType::playerBullet, 1);
            break;
        case 2:
        //double shoot
            bulletManager->GetNextAvailableBullet()->setBullet(x - 4, y, 25, 0, 0, bulletType::playerBullet, 1);
            bulletManager->GetNextAvailableBullet()->setBullet(x + 16, y, 25, 0, 0, bulletType::playerBullet, 1);
            break;
        case 3:
        //maybe double shoot and add 2 more beside them and set velovity 0 and acceleration increase over time
            break;
        }
    }
    else
    {
        bulletCooldown--;
    }
}

void Player::draw(sf::RenderWindow &window)
{

    spriteplayer.setPosition(x, y);
    hitboxsprite.setPosition(x+9, y+16);

    window.draw(spriteplayer);
    window.draw(hitboxsprite);

    bulletManager->DrawBullets(window);
}

void Player::update(int score)
{
    if ((1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || 1 == sf::Keyboard::isKeyPressed(sf::Keyboard::A)) && x > 0)
    {
        x -= player_velocity;
    }
    if ((1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || 1 == sf::Keyboard::isKeyPressed(sf::Keyboard::D)) && x < WIDTH-30)
    {
        x += player_velocity;
    }
    if ((1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || 1 == sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && y > 0)
    {
        y -= player_velocity;
    }
    if ((1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || 1 == sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && y < HEIGHT-50)
    {
        y += player_velocity;
    }

    if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || 1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){
        if (score >= 10000)
            shoot(2);
        else
            shoot(1);
    }
    
    if (invincibility){

        if (invincibilityCooldown <= 0){
            sf::Color spriteColor = spriteplayer.getColor();
            spriteColor.a = 255; // Set alpha to 192 (75% opacity)
            spriteplayer.setColor(spriteColor);
            invincibility = false;
        }
        else{
            invincibilityCooldown--;
        }
    }

    bulletManager->UpdateBullets();
}

void Player::invincible(){
    invincibility = true;
    invincibilityCooldown = 120;
    sf::Color spriteColor = spriteplayer.getColor();
    spriteColor.a = 128; // Set alpha to 192 (75% opacity)
    spriteplayer.setColor(spriteColor);
}