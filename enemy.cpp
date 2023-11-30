#include "enemy.hpp"
#include "player.hpp"
#include "bullet.hpp"
#include "objects.hpp"
#include "point.hpp"
#include <cmath>
#include <utility>


Enemy::Enemy(){
    x = 0;
    y = 0;
}


Enemy::Enemy(double x, double y, int state, double acceleration){
    this->x = x;
    this->y = y;
    this->state = state; //state 0 -> normal, state 1 -> homing
    this-> acceleration = acceleration;
    alive = true;
}




Enemy::~Enemy(){}

double Enemy::getX(){
    return x;
}

double Enemy::getY(){
    return y;
}

int Enemy::getScore(){
    return score;
}

int Enemy::getState(){
    return state;
}

bool Enemy::getAlive(){
    return alive;
}

void Enemy::setAlive(bool alive){
    this->alive = alive;
}

void Enemy::draw(sf::RenderWindow &window)
{
    spriteenemy.setPosition(x, y);
    window.draw(spriteenemy);
}

void Enemy::update(BulletManager* bulletManager, std::vector<Deathsprite*> &deathsprites, Player &player, std::vector<Point*> &pointVec, BulletManager* enemyBulletManager, int &globalScore){
    switch(state){
        case 0:
            y += velocity;
            break;
        case 1:
            x += std::cos(homing(x,y,player.getX(),player.getY())) * velocity*1.25;
            y += std::sin(homing(x,y,player.getX(),player.getY())) * velocity*1.25;
            break;
        case 2:
            
            if (velocity > 0 && shot == false){
                velocity += acceleration;
                y += velocity;
                
            }

            else {
                if (shot == false){
                shooting = true;
            }

             else if (x < WIDTH/2 && shot == true){
                velocity -= acceleration;
                x -= velocity;  
            }

            else if (x >= WIDTH/2 && shot == true){
                velocity -= acceleration;
                x += velocity;
                }
            }
            break;

    }

    //check collision
    for (int i = 0; i < bulletManager->bulletVector.size(); ++i) {
        if (bulletManager->active[i]) {
            if (collision(spriteenemy.getGlobalBounds(), bulletManager->bulletVector[i]->spritebullet.getGlobalBounds())) {
                health--;
                bulletManager->active[i] = false;
                bulletManager->bulletVector[i]->setX(WIDTH+100);
                bulletManager->bulletVector[i]->setY(HEIGHT+100);
                if (health <= 0) {
                    deathsprites.push_back(new Deathsprite(this->x, this->y));
                    deathsprites.push_back(new Deathsprite(this->x+10, this->y+10));
                    alive = false;
                    
                    //create points
                    int pointcount = rand()%3 + 1;
                    for (int i = 0; i < pointcount; ++i) {
                        pointVec.push_back(new Point(this->x + rand()%9 + 1, this->y + rand()%9 + 1));
                    }
                                        
                    globalScore += score;
                }
            }
        }
    }

    
    if (collision(spriteenemy.getGlobalBounds(), player.getGlobalBounds())){
        alive = false;
        deathsprites.push_back(new Deathsprite(this->x, this->y));
        deathsprites.push_back(new Deathsprite(this->x + rand() % 10, this->y + rand() % 10));
        if (!player.getInvincibility()){
            player.hpDecrease();
            player.invincible();
            std::cout << "Enemy collision" << std::endl;
        }
    }

    if (y > HEIGHT+50){
        alive = false;
    }
}




Red_Enemy::Red_Enemy(double x, double y, int state): Enemy(x, y, state){
    health = 3;
    score = 100;
    enemyTexture.loadFromFile("images/redfairy.png");
    spriteenemy.setTexture(enemyTexture);
    velocity = 2;
    //std::cout << "red enemy created" << std::endl;
}

Red_Enemy::~Red_Enemy(){}

void Red_Enemy::update(BulletManager* bulletManager, std::vector<Deathsprite*> &deathsprites, Player &player, std::vector<Point*> &pointVec, BulletManager* enemyBulletManager, int &globalScore){
    Enemy::update(bulletManager, deathsprites, player, pointVec, enemyBulletManager, globalScore);
    if (health <= 0){
        for (int i = 0; i < 10; ++i) {
            enemyBulletManager->GetNextAvailableBullet()->setBullet(x, y, 5, 0, degToRad(i*36), bulletType::fairyBullet, 1);
        }
        
    }
}

Black_Enemy::Black_Enemy(double x, double y, int state): Enemy(x, y, state){
    health = 5;
    score = 200;
    enemyTexture.loadFromFile("images/blackfairy.png");
    spriteenemy.setTexture(enemyTexture);
    velocity = 1.5;
    bulletCooldown = rand()%240 + 1;
    //std::cout << "black enemy created" << std::endl;
}

Black_Enemy::~Black_Enemy(){}

void Black_Enemy::update(BulletManager* bulletManager, std::vector<Deathsprite*> &deathsprites, Player &player, std::vector<Point*> &pointVec, BulletManager* enemyBulletManager, int &globalScore){
    Enemy::update(bulletManager, deathsprites, player, pointVec, enemyBulletManager, globalScore);
    if (bulletCooldown <= 0 && y >= 0)
    {
        bulletCooldown = 240;
        for (int i = 0; i < 3; i++){
            enemyBulletManager->GetNextAvailableBullet()->setBullet(x, y, 4, 0, degToRad(80 + i * 10), bulletType::fairyBullet, 1);
        } //16 is half of the width of the enemy, while 11 was half the width of the bullet  //27 is the height of the enemy
    }
    else
    {
        bulletCooldown--;
    }
}

Purple_Enemy::Purple_Enemy(double x, double y, int state): Enemy(x, y, state){
    health = 10;
    score = 500;
    enemyTexture.loadFromFile("images/purplefairy.png");
    spriteenemy.setTexture(enemyTexture);
    velocity = 0.9;
    //std::cout << "purple enemy created" << std::endl;
}

Purple_Enemy::~Purple_Enemy(){}

void Purple_Enemy::update(BulletManager* bulletManager, std::vector<Deathsprite*> &deathsprites, Player &player, std::vector<Point*> &pointVec, BulletManager* enemyBulletManager, int &globalScore){
    Enemy::update(bulletManager, deathsprites, player, pointVec, enemyBulletManager, globalScore);
    if (bulletCooldown <= 0 && y >= 0)
    {
        bulletCooldown = 90;
        enemyBulletManager->GetNextAvailableBullet()->setBullet(x, y, 3, -0.001, homing(x, y, player.getX(), player.getY()), bulletType::fairyBullet, 2);
        enemyBulletManager->GetNextAvailableBullet()->setBullet(x, y, 5, -0.003, homing(x, y, player.getX(), player.getY()), bulletType::fairyBullet, 2);
        enemyBulletManager->GetNextAvailableBullet()->setBullet(x, y, 8, -0.005, homing(x, y, player.getX(), player.getY()), bulletType::fairyBullet, 2);
        
    }
    else
    {
        bulletCooldown--;
    }
}

Blue_Enemy::Blue_Enemy(double x, double y, int state): Enemy(x, y, state){
    health = 4;
    score = 300;
    enemyTexture.loadFromFile("images/bluefairy.png");
    spriteenemy.setTexture(enemyTexture);
    velocity = 2.5;
    //std::cout << "blue enemy created" << std::endl;
}

Blue_Enemy::~Blue_Enemy(){}

Miniboss::Miniboss(double x, double y, int state, double acceleration): Enemy(x, y, state, acceleration){
    health = 1000;
    score = 0;
    shot = false;
    enemyTexture.loadFromFile("images/pattern1fairy.png");
    spriteenemy.setTexture(enemyTexture);
    velocity = 5;
    bulletCooldown = 0;
    shooting = false;
   
}

Miniboss::~Miniboss(){}

void Miniboss::update(BulletManager* bulletManager, std::vector<Deathsprite*> &deathsprites, Player &player, std::vector<Point*> &pointVec, BulletManager* enemyBulletManager, int &globalScore){
    Enemy::update(bulletManager, deathsprites, player, pointVec, enemyBulletManager, globalScore);
    if (shooting == true){    

        for (int i = 0; i < 2; i++){
             enemyBulletManager->GetNextAvailableBullet()->setBullet(x, y, i+3, -0.005, homing(x, y, player.getX(), player.getY()), 0.9, bulletType::fairyBullet, 1);
            for (int j = 1; j < 6; j++){
                enemyBulletManager->GetNextAvailableBullet()->setBullet(x, y, i+3, -0.005, homing(x, y, player.getX()+j*75, player.getY()), 0.9, bulletType::fairyBullet, 1);
                enemyBulletManager->GetNextAvailableBullet()->setBullet(x, y, i+3, -0.005, homing(x, y, player.getX()-j*75, player.getY()), 0.9, bulletType::fairyBullet, 1);
            }
        }

        
        shot = true;
        shooting = false;
        }

    if (x > WIDTH+50 || x < -50){
        alive = false;
    }
        
}

Boss::Boss(){
    this->state = 0; 
}

Boss::~Boss(){}

double Boss::getX(){
    return x;
}

double Boss::getY(){
    return y;
}

int Boss::getHealth(){
    return health;
}

int Boss::getState(){
    return state;
}

int Boss::getTimeCounter(){
    return timeCounter;
}

void Boss::setState(int state){
    this->state = state;
}

void Boss::setTimeCounter(int timeCounter){
    this->timeCounter = timeCounter;
}

void Boss::resetCooldown(){
    for (int i = 0; i < sizeof(bulletCooldown) / sizeof(bulletCooldown[0]); ++i){
        bulletCooldown[i] = 0;
    }
}

void Boss::setDestination(double x, double y) {
    destination = std::make_pair(x, y);
}

void Boss::randDestination(double range) {
    sf::Vector2u textureSize = enemyTexture.getSize();
    int width = textureSize.x;
    int height = textureSize.y;

    double randX = rand() % ((int)(range * 2 + 1)) + (x - range);
    double randY = rand() % ((int)(range * 2 + 1)) + (y - range);
    
    if (randX > 0 && randX < WIDTH - ((double) width / 2) && randY > 0 && randY < 200 - ((double) height / 2)) {
        destination = std::make_pair(randX, randY);
    } else {
        destination = std::make_pair(x, y);
    }
}

    //                 1
    //           1 1 1 1 1 1 1
    // center = 4
    // range = 3
    // rand() % 7 = 0, 1, 2, 3, 4, 5, 6
    // center - range = 1
    //           1
    //           1 1 1 1 1 1 1

void Boss::goToDestination(){
    x += std::cos(homing(destination.first, destination.second, x, y)) * velocity;
    y += std::sin(homing(destination.first, destination.second, x, y)) * velocity;
}

void Boss::update(BulletManager* bulletManager, Player &player, std::vector<BulletManager*> &enemyBulletManager, int activeBullets){
    //check collision
    for (int i = 0; i < bulletManager->bulletVector.size(); ++i) {
        if (bulletManager->active[i]) {
            if (collision(spriteenemy.getGlobalBounds(), bulletManager->bulletVector[i]->spritebullet.getGlobalBounds())) {
                health--;
                if (state >= 3){
                    player.damageDealt();
                }
                bulletManager->active[i] = false;
                bulletManager->bulletVector[i]->setX(WIDTH+100);
                bulletManager->bulletVector[i]->setY(HEIGHT+100);
            }
        }
    }
    if (collision(spriteenemy.getGlobalBounds(), player.getGlobalBounds())){
        if (!player.getInvincibility()){
            player.hpDecrease();
            player.invincible();
            std::cout << "Boss collision" << std::endl;
        }
    }
}

void Boss::draw(sf::RenderWindow &window)
{
    spriteenemy.setPosition(x, y);
    window.draw(spriteenemy);
}

Flan::Flan(): Boss(){
    x = WIDTH/2 - 30;
    y = -380;
    health = 800;
    score = 8000;
    enemyTexture.loadFromFile("images/flan.png");
    spriteenemy.setTexture(enemyTexture);
    velocity = 2;
    angle = 90;
    int ranD = rand() % 4;
    if (ranD == 0){
        direction = std::make_pair(-1, -1);
    }
    else if (ranD == 1){
        direction = std::make_pair(-1, 1);
    }
    else if (ranD == 2){
        direction = std::make_pair(1, -1);
    }
    else{
        direction = std::make_pair(1, 1);
    }
}

Flan::~Flan(){}

void Flan::update(BulletManager* bulletManager, Player &player, std::vector<BulletManager*> &enemyBulletManagerManager, int activeBullets){
    Boss::update(bulletManager, player, enemyBulletManagerManager, activeBullets);
    switch (state)
    {
    case 0:
        //boss appears and fly downwards
        if (timeCounter < 240){
            timeCounter++;
            y += 2;
        }
        else{
            state = 1;
        }
        break;

    case 1:
        if (checkBombKey() && player.getBombCount() == 60 && player.bombs.size() > 0){
            timeCounter = 0;
            totalTime += timeCounter;
        }
        if (timeCounter < 240){
            timeCounter++;
        }
        else if (timeCounter < 1167){
            timeCounter++;
            if (activeBullets < 21){
                for (int i = 0; i < 5; ++i){
                    // 60 * i means x increments by 60 in row 1 and the random just makes the bullets less organized // y same as the boss's and the rand also randomized it (just forf decoration) // velocity starts negative so when it increases, it the bullets will change from going upward to downwards // the angle the bullet is heading
                    //left
                    enemyBulletManagerManager[0]->GetNextAvailableBullet()->setBullet(60 * (i + 1)      + (rand() % 15 - 7),     y +                    rand() % 9 - 4, -2, 0.05,   degToRad(90), sf::Color(221,231,112,200), bulletType::bossBullet, 1);
                    enemyBulletManagerManager[0]->GetNextAvailableBullet()->setBullet(60 * (i + 1) - 7  + (rand() % 15 - 7),     y + (8.5 * (5 - i))  + rand() % 9 - 4, -2.5, 0.06, degToRad(90), sf::Color(212,187,81,255),  bulletType::bossBullet, 1);
                    enemyBulletManagerManager[0]->GetNextAvailableBullet()->setBullet(60 * (i + 1) - 14 + (rand() % 15 - 7),     y + (17 * (5 - i))   + rand() % 9 - 4, -3, 0.075,  degToRad(90), sf::Color(219,86,102,200),  bulletType::bossBullet, 1);
                    enemyBulletManagerManager[0]->GetNextAvailableBullet()->setBullet(60 * (i + 1) - 21 + (rand() % 15 - 7),     y + (25.5 * (5 - i)) + rand() % 9 - 4, -3.25, 0.1, degToRad(90), sf::Color(135,40,106,200),  bulletType::bossBullet, 1);
                    enemyBulletManagerManager[0]->GetNextAvailableBullet()->setBullet(60 * (i + 1) - 28 + (rand() % 15 - 7),     y + (34 * (5 - i))   + rand() % 9 - 4, -3.5, 0.12, degToRad(90), sf::Color(33,144,189,200),  bulletType::bossBullet, 1);
                    //right
                    enemyBulletManagerManager[0]->GetNextAvailableBullet()->setBullet(60 * (i + 1) + x      + (rand() % 15 - 7), y +                    rand() % 9 - 4, -2, 0.05,   degToRad(90), sf::Color(190,200,160,200), bulletType::bossBullet, 1);
                    enemyBulletManagerManager[0]->GetNextAvailableBullet()->setBullet(60 * (i + 1) + x - 7  + (rand() % 15 - 7), y + (8.5 * i)        + rand() % 9 - 4, -2.5, 0.06, degToRad(90), sf::Color(55,224,223,200),  bulletType::bossBullet, 1);
                    enemyBulletManagerManager[0]->GetNextAvailableBullet()->setBullet(60 * (i + 1) + x - 14 + (rand() % 15 - 7), y + (17 * i)         + rand() % 9 - 4, -3, 0.075,  degToRad(90), sf::Color(143,43,141,200),  bulletType::bossBullet, 1);
                    enemyBulletManagerManager[0]->GetNextAvailableBullet()->setBullet(60 * (i + 1) + x - 21 + (rand() % 15 - 7), y + (25.5 * i)       + rand() % 9 - 4, -3.25, 0.1, degToRad(90), sf::Color(170,195,120,200), bulletType::bossBullet, 1);
                    enemyBulletManagerManager[0]->GetNextAvailableBullet()->setBullet(60 * (i + 1) + x - 28 + (rand() % 15 - 7), y + (34 * i)         + rand() % 9 - 4, -3.5, 0.12, degToRad(90), sf::Color(8,179,201,200),   bulletType::bossBullet, 1);
                }
            }
        }
        else{
            timeCounter = 0;
            totalTime += timeCounter;
        }
        break;
    case 2:
        timeCounter++;
        if (bulletCooldown[1] > 80){
            for (int i = 1; i < 36; ++i){
                enemyBulletManagerManager[0]->GetNextAvailableBullet()->setBullet(x + 30, y, 4, -0.002, degToRad(angle + 36 + (7.2 * i)), sf::Color(100,60,223,120), bulletType::bossBullet, 2);
            }
            bulletCooldown[1] = 0;
        }
        if (bulletCooldown[2] > 100){
            for (int i = 1; i < 36; ++i){
                enemyBulletManagerManager[0]->GetNextAvailableBullet()->setBullet(x + 30, y, 4, -0.002, degToRad(angle + 36 + (7.2 * i)), sf::Color(100,60,223,120), bulletType::bossBullet, 2);
            }
            bulletCooldown[2] = bulletCooldown[1];
        }
        if (timeCounter >= 0 && patternCounter < 3){

            bulletCooldown[0]++;
            bulletCooldown[1]++;
            bulletCooldown[2]++;

            if (bulletCooldown[0] < 81){
                enemyBulletManagerManager[1]->GetNextAvailableBullet()->setBullet(x + 30, y, 1, -0.0007, degToRad(angle + (3.6 * bulletCooldown[0])), bulletType::bossBullet, 0);
            }
            else if (bulletCooldown[0] >= 100){
                bulletCooldown[0] = 0;
                angle = angle - 36;
                patternCounter++;
            }
            
        }
        else if (patternCounter == 3){
            patternCounter = 0;
        }
        else{
            y += 2;
        }
        break;
    case 3:
        if (timeCounter < 0){
            goToDestination();
        }
        break;
    case 4:
        if (timeCounter < 0){
            goToDestination();
        }
        else if (timeCounter < 240){
            x -= 0.8;
            y += 0.12;
        }
        break;
    case 5:
        if (timeCounter < 0){
            goToDestination();
        }
        else if (timeCounter < 240){
            x += 0.6;
            y -= 0.02;
        }
        break;
    case 6:
        if (timeCounter < 0){
            goToDestination();
        }
        else if (timeCounter < 240){
            x += 1;
            y += 0.1;
        }
        break;
    case 9:
        timeCounter++;
        if (timeCounter >= 90){
            y -= velocity;
        }
        break;
    case 10:
        timeCounter++;
        if (timeCounter >= 90){
            goToDestination();
        }
        break;
    }

    if (state >= 3 && state < 8){
        timeCounter++;
        if (timeCounter >= 240){
            if (x <= 10){
                x += 15;
                direction.first = 1;
            }
            else if (x >= WIDTH - 62){
                x -= 32;
                direction.first = -1;
            }
            if (y <= 0){
                y += 15;
                direction.second = 1;
            }
            else if (y >= 200){
                y -= 15;
                direction.second = -1;
            }
                
            bulletCooldown[1]++;
            if (bulletCooldown[1] > 180){
                destination.first = rand() % 3 + 1;
                destination.second = rand() % 2 + 1;
                
                x += direction.first * destination.first;
                y += direction.second * destination.second;
            }

            bulletCooldown[0]++;
            if (bulletCooldown[0] > 80){
                for (int i = 1; i < 51; ++i){
                    enemyBulletManagerManager[1]->GetNextAvailableBullet()->setBullet(x + 30, y, 3, -0.001, degToRad((7.2 * i)), bulletType::bossBullet, 0);
                }
                bulletCooldown[0] = 0;
            }
        }
    }
}