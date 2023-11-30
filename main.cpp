#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <cmath>
#include <chrono>
#include <fstream>


#include "player.hpp"
#include "player.cpp"
#include "bullet.hpp"
#include "bullet.cpp"
#include "enemy.hpp"
#include "enemy.cpp"
#include "objects.hpp"
#include "objects.cpp"
#include "point.hpp"
#include "point.cpp"


int main()
{
    #pragma region 
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Shmup game!");
    window.setFramerateLimit(60);
    

    sf::Texture background1Texture;
    sf::Texture background2Texture;
    background1Texture.loadFromFile("images/starfield1.png");
    background2Texture.loadFromFile("images/starfield2.png");

    sf::Sprite background1Sprite(background1Texture);
    sf::Sprite background2Sprite(background2Texture);

    background1Sprite.setPosition(0,0);
    background2Sprite.setPosition(0,-HEIGHT);
    sf::Texture playerTexture;
    sf::Sprite spriteplayer;
    sf::Texture EnemyBulletTexture;
    EnemyBulletTexture.loadFromFile("images/enemybullet.png");

    sf::Texture gameoverTexture;
    gameoverTexture.loadFromFile("images/gg.png");
    sf::Sprite ggSprite(gameoverTexture);
    ggSprite.setPosition(-150, 100);


    sf::Texture startTexture;
    startTexture.loadFromFile("images/Start.png");
    sf::Sprite stSprite(startTexture);
    stSprite.setPosition(25, 300);


    sf::Event event;

    Player player;

    std::vector<BulletManager*> bulletManagerManager;
    bulletManagerManager.push_back(new BulletManager(EnemyBulletTexture, bulletType::fairyBullet, 180));
    EnemyBulletTexture.loadFromFile("images/enemyBullet2.png");
    bulletManagerManager.push_back(new BulletManager(EnemyBulletTexture, bulletType::bossBullet, 600));
    // bulletManagerManager.push_back(new BulletManager(EnemyBulletTexture, bulletType::bossBullet, 150));
    // bulletManagerManager.push_back(new BulletManager(EnemyBulletTexture, bulletType::bossBullet, 150));
    // bulletManagerManager.push_back(new BulletManager(EnemyBulletTexture, bulletType::bossBullet, 150));

    std::vector<Boss*> bosses;
    std::vector<Enemy*> enemies;
    std::vector<Deathsprite*> deathsprites;
    std::vector<Point*> pointVec;
    
    int score = 0;
    int level = 1;
    int enemy_count = 0;
    int enemy_per_level = 0;
    int spawn_timer = 0;
    int patternspawn_timer= -1;
    int patternenemy_count = 0;
    int patternenemy_kill = 0;
    bool spawnleft = true;
    int activeBullets = 0;
    bool gamestart = false;
    int highScore = 0;
    std::ifstream file("highscore.txt");
    if (file.is_open())
    {
        file >> highScore;
        file.close();
    }

    srand(time(NULL));

    sf::Music BGM;
    if (!BGM.openFromFile("music/BGM.wav")) {
          std::cout<< "Error"<< std::endl;
    }
    BGM.setVolume(50);//50);
    BGM.setLoop(true);
    BGM.pause();
        

    sf::Music Death;
    if (!Death.openFromFile("music/Death.wav")) {
          std::cout<< "Error"<< std::endl;
    }
    Death.setVolume(50);//50);
    Death.setLoop(true);


    sf::Font font;
    if (!font.loadFromFile("font/Classic.ttf")) {
        std::cout<< "Error"<< std::endl;
    }
    

    for (int i = 0; i < player.getBombCount(); i++){
        player.bombs.push_back(new Bomb(15 + i * 45, HEIGHT - 45));
    }

    player.setBombCount(60);

    sf::Text text;
    text.setFont(font);
    text.setString("Score: "+ std::to_string(score));
    text.setCharacterSize(40);
    text.setFillColor(sf::Color::Yellow);
    text.setPosition(20, 20);

    sf::Text high;
    high.setFont(font);
    high.setCharacterSize(30);
    high.setFillColor(sf::Color::Yellow);
    high.setPosition(300, 550);

    #pragma endregion

    while (1 == window.isOpen())
    {
       
        sf::Event event;
        

        if (event.key.code == sf::Keyboard::Enter){
            sf::Color StartScreen = stSprite.getColor();
            StartScreen.a = 0; 
            stSprite.setColor(StartScreen);
            gamestart = true;
            BGM.play();
        }

        while (window.pollEvent(event)){if (event.type == sf::Event::Closed){window.close();}}

        if(gamestart){
            background2Sprite.move(0, 1);
            background1Sprite.move(0, 1);
            if (background1Sprite.getPosition().y >= HEIGHT)
                background1Sprite.setPosition(0,-HEIGHT);
            if (background2Sprite.getPosition().y >= HEIGHT)
                background2Sprite.setPosition(0,-HEIGHT);


            //levels
            spawn_timer++;
            if (enemy_count == 0 && level < 3){

                level++;
                enemy_per_level += 2;

                for (int i = 0; i < enemy_per_level; i++){
                    int random_spawn = rand() % 2;
                
                    if (random_spawn == 0){
                        enemies.push_back(new Red_Enemy(rand() % -600 - 20, rand() % -20 - 300, 0));
                        enemy_count++;
                    }
                    else if (random_spawn == 1){
                        enemies.push_back(new Black_Enemy(rand() % -600 -20, rand() % -20 - 200, 0));
                        enemy_count++;
                    }
        
                }
            }

            else if (enemy_count == 0 && level == 3 ) {
                level++;
                spawn_timer = 0;
            }

            else if (level == 4){
                
                
                if (patternenemy_count != 12 && spawn_timer == 20){
                    patternspawn_timer++;
                    enemies.push_back(new Blue_Enemy(patternspawn_timer*70, -20,1));
                    patternenemy_count++;
                    spawn_timer = 0;            
                }

                if (patternenemy_kill == 12){
                    patternenemy_count = 0;
                    patternspawn_timer = -1;
                    patternenemy_kill = 0;
                    spawn_timer = 0;
                    
                    level++;
                }
            }
            else if (level == 5){

                if (patternenemy_count != 12 && spawn_timer == 20){
                    patternspawn_timer++;
                    enemies.push_back(new Blue_Enemy(WIDTH - patternspawn_timer*70, -20,1));
                    patternenemy_count++;
                    spawn_timer = 0;
                }

                  
                if (enemy_count == 0){

                    if (patternenemy_kill == 12){

                        patternenemy_count = 0;
                        patternspawn_timer = -1;
                        patternenemy_kill = 0;
                        
                        spawn_timer = 0;
                        level++;
                    }

                    
                    else{
                        for (int i = 0; i < 3; i++){     
                            enemies.push_back(new Purple_Enemy(WIDTH*1/4 + WIDTH*i/4 - 25, -100, 0));
                            enemy_count++;
                        }
                    }
                } 
            }
            
            if (level == 6){

                if (patternenemy_count != 8 && spawn_timer == 45){
                    patternspawn_timer++;
                    if (spawnleft){
                            enemies.push_back(new Miniboss(WIDTH/4, -20 , 2, -0.05));
                            spawnleft = false;
                            }
                    else if (!spawnleft){
                            enemies.push_back(new Miniboss(WIDTH/4*3, -20 , 2, -0.05));
                            spawnleft = true;
                            }
                    patternenemy_count++;
                    spawn_timer = 0;
                }

                if (patternenemy_kill >= 1 && enemies.size() == 0 && activeBullets == 0){
                    patternenemy_count = 0;
                    patternspawn_timer = -1;
                    patternenemy_kill = 0;
                    spawn_timer = 0;
                    
                    level++;
                    bosses.push_back(new Flan());
                }
            }

            if (level == 7){
                //300, 200, 300
                if (bosses[0]->getHealth() <= 500 && bosses[0]->getState() == 1){
                    bosses[0]->setState(2);
                    bosses[0]->setTimeCounter(-200);
                    //make a function for this
                    for (int i = 0; i < bulletManagerManager[0]->bulletVector.size(); ++i) {
                        bulletManagerManager[0]->active[i] = false;
                        if (rand() % 3 >= 1 && bulletManagerManager[0]->bulletVector[i]->getBulletType() == bulletType::bossBullet){
                            pointVec.push_back(new Point(bulletManagerManager[0]->bulletVector[i]->getX(), bulletManagerManager[0]->bulletVector[i]->getY()));
                        }
                        bulletManagerManager[0]->bulletVector[i]->setX(WIDTH+100);
                        bulletManagerManager[0]->bulletVector[i]->setY(HEIGHT+100);
                    }

                    for (int i = 0; i < pointVec.size(); ++i) {
                        pointVec[i]->toggleBombed();
                    }
                }
                if (bosses[0]->getHealth() <= 300 && bosses[0]->getState() == 2){
                    bosses[0]->setState(3);
                    bosses[0]->setTimeCounter(-199);
                    bosses[0]->resetCooldown();
                    bosses[0]->setDestination(270, 100);
                    for (int i = 1; i < 4; ++i){
                        bosses.push_back(new Flan());
                        bosses[i]->setState(3 + i);
                        bosses[i]->setTimeCounter(-240);
                        bosses[i]->setDestination(270, 100);
                    }
                    
                    for (int j = 0; j < bulletManagerManager[1]->bulletVector.size(); ++j) {
                        bulletManagerManager[1]->active[j] = false;
                        bulletManagerManager[1]->bulletVector[j]->setX(WIDTH+100);
                        bulletManagerManager[1]->bulletVector[j]->setY(HEIGHT+100);
                    }
                }
                if (player.getDamageOnBoss() >= 300 && bosses[0]->getState() == 3){
                    for (int i = 1; i < 4; ++i){
                        bosses[i]->setTimeCounter(0);
                        bosses[i]->setState(10);
                        bosses[i]->setDestination(270, 100);
                    }
                    bosses[0]->setTimeCounter(0);
                    bosses[0]->setState(9);
                }
                if (bosses[0]->getState() == 9 && bosses[0]->getTimeCounter() >= 420){
                    player.setLife(0);
                }

            }
        }
        
        window.clear();
        
        window.draw(background1Sprite);
        window.draw(background2Sprite);
        window.draw(stSprite);
        if(gamestart){
            window.draw(text);
        }
        player.draw(window);

        player.update(score);

        
        //enemy management
        if (gamestart){
            for (int i = 0; i < enemies.size(); ++i) {
                enemies[i]->draw(window);
                enemies[i]->update(player.bulletManager, deathsprites, player, pointVec, bulletManagerManager[0], score);
                if (!enemies[i]->getAlive()){
                    text.setString("Score: "+ std::to_string(score));
                    switch(enemies[i]->getState()){
                        case 0:
                            enemy_count--;
                            break;
                            
                        case 1:
                            patternenemy_kill++;
                            break;
                        case 2:
                            patternenemy_kill++;
                            break;
                    }
                    enemies.erase(enemies.begin() + i);
                }
            }

            for(int i = 0; i < deathsprites.size(); i++){
                deathsprites[i]->lifetime--;
                if (deathsprites[i]->lifetime == 0){
                    deathsprites.erase(deathsprites.begin() + i);
                }
                deathsprites[i]->draw(window);
            }

            for (int i = 0; i < bosses.size(); ++i) {
                if (bosses[i]->getState() < 4){
                    bosses[i]->draw(window);
                }
                else if (bosses[i]->getTimeCounter() >= 0){
                    bosses[i]->draw(window);
                }
                bosses[i]->update(player.bulletManager, player, bulletManagerManager, activeBullets);
                //print destination
                // std::cout << "boss " << i << " state: " << bosses[i]->getState() << std::endl;
                //std::cout << i << " destination : " << bosses[i]->destination.first << ", " << bosses[i]->destination.second << std::endl;
                // std::cout << "boss " << i << " position: " << bosses[i]->getX() << ", " << bosses[i]->getY() << std::endl;
                std::cout << "damage on boss : " << player.getDamageOnBoss() << std::endl;
            }

            for (int i = 0; i < player.bombs.size(); i++){
                player.bombs[i]->draw(window);
            }

            if (player.getBombCount() < 60){
                player.incrementBombCount();
            }

            if (player.bombs.size() > 0 && checkBombKey() && player.getBombCount() == 60){
                player.setBombCount(0);
                player.bombs.erase(player.bombs.begin() + player.bombs.size() - 1);
                for (int i = 0; i < enemies.size(); ++i) {
                    deathsprites.push_back(new Deathsprite(enemies[i]->getX(), enemies[i]->getY()));
                    deathsprites.push_back(new Deathsprite(enemies[i]->getX() + rand() % 10, enemies[i]->getY() + rand() % 10));
                    pointVec.push_back(new Point(enemies[i]->getX() + rand() % 9 + 1, enemies[i]->getY() + rand() % 9 + 1));
                    enemies[i]->setAlive(false);
                }
                for (int i = 0; i < bulletManagerManager[0]->bulletVector.size(); ++i) {
                    bulletManagerManager[0]->active[i] = false;
                    if (rand() % 3 >= 1 && bulletManagerManager[0]->bulletVector[i]->getBulletType() == bulletType::bossBullet){
                        pointVec.push_back(new Point(bulletManagerManager[0]->bulletVector[i]->getX(), bulletManagerManager[0]->bulletVector[i]->getY()));
                    }
                    bulletManagerManager[0]->bulletVector[i]->setX(WIDTH+100);
                    bulletManagerManager[0]->bulletVector[i]->setY(HEIGHT+100);
                }
                if (level >= 7){
                    for (int i = 0; i < bulletManagerManager[1]->bulletVector.size(); ++i) {
                        bulletManagerManager[1]->active[i] = false;
                        if (rand() % 3 >= 1 && bulletManagerManager[1]->bulletVector[i]->getBulletType() == bulletType::bossBullet){
                            pointVec.push_back(new Point(bulletManagerManager[1]->bulletVector[i]->getX(), bulletManagerManager[1]->bulletVector[i]->getY()));
                        }
                        bulletManagerManager[1]->bulletVector[i]->setX(WIDTH+100);
                        bulletManagerManager[1]->bulletVector[i]->setY(HEIGHT+100);
                    }
                }
                for (int i = 0; i < pointVec.size(); ++i) {
                    pointVec[i]->toggleBombed();
                }
            }

            //enemy bullets and points management
            activeBullets = 0;
            for (int i = 0; i < bulletManagerManager[0]->bulletVector.size(); ++i) {
                if (bulletManagerManager[0]->active[i]){
                    bulletManagerManager[0]->bulletVector[i]->draw(window);
                    bulletManagerManager[0]->bulletVector[i]->update();
                }
                
                if (bulletManagerManager[0]->bulletVector[i]->spritebullet.getGlobalBounds().intersects(player.getGlobalBounds()) && bulletManagerManager[0]->active[i]){
                    bulletManagerManager[0]->active[i] = false;
                    bulletManagerManager[0]->bulletVector[i]->setX(WIDTH+100);
                    bulletManagerManager[0]->bulletVector[i]->setY(HEIGHT+100);
                    if (!player.getInvincibility()){
                        player.hpDecrease();
                        player.invincible();
                        std::cout << "Bullet collision" << std::endl;
                    }
                }
                if (bulletManagerManager[0]->bulletVector[i]->getY() > HEIGHT + 40 || bulletManagerManager[0]->bulletVector[i]->getY() < -40 || bulletManagerManager[0]->bulletVector[i]->getX() > WIDTH + 40 || bulletManagerManager[0]->bulletVector[i]->getX() < -40){
                    bulletManagerManager[0]->active[i] = false;
                    // bulletManagerManager[0]->bulletVector[i]->setX(WIDTH+100);
                    // bulletManagerManager[0]->bulletVector[i]->setY(HEIGHT+100);
                }
                if (bulletManagerManager[0]->active[i]){
                    activeBullets++;
                }
            }
            
            if (level >= 7){
                for (int i = 0; i < bulletManagerManager[1]->bulletVector.size(); ++i) {
                    if (bulletManagerManager[1]->active[i]){
                        bulletManagerManager[1]->bulletVector[i]->draw(window);
                        bulletManagerManager[1]->bulletVector[i]->update();
                    }
                    
                    if (bulletManagerManager[1]->bulletVector[i]->spritebullet.getGlobalBounds().intersects(player.getGlobalBounds()) && bulletManagerManager[1]->active[i]){
                        bulletManagerManager[1]->active[i] = false;
                        bulletManagerManager[1]->bulletVector[i]->setX(WIDTH+100);
                        bulletManagerManager[1]->bulletVector[i]->setY(HEIGHT+100);
                        if (!player.getInvincibility()){
                            player.hpDecrease();
                            player.invincible();
                            std::cout << "Bullet collision" << std::endl;
                        }
                    }
                    if (bulletManagerManager[1]->bulletVector[i]->getY() > HEIGHT + 40 || bulletManagerManager[1]->bulletVector[i]->getY() < -40 || bulletManagerManager[1]->bulletVector[i]->getX() > WIDTH + 40 || bulletManagerManager[1]->bulletVector[i]->getX() < -40){
                        bulletManagerManager[1]->active[i] = false;
                    }
                    if (bulletManagerManager[1]->active[i]){
                        activeBullets++;
                    }
                }
            }


            for (int i = 0; i < pointVec.size(); ++i) {
                pointVec[i]->draw(window);
                pointVec[i]->update(player);
                if (pointVec[i]->getGlobalBounds().intersects(player.getGlobalBounds())){
                    pointVec.erase(pointVec.begin() + i);
                    score += pointVec[i]->getScoreValue();
                    text.setString("Score: "+ std::to_string(score));
                }
                
                //if go out of screen erase
                if (pointVec[i]->getY() > HEIGHT){
                    pointVec.erase(pointVec.begin() + i);
                }
            }


            //check death
            if (player.getLives() <= 0){
                window.clear();
                Death.play();
                window.draw(ggSprite);
                text.setCharacterSize(30);
                text.setPosition(300, 500);
                window.draw(text);
                if (score > highScore){
                    std::ofstream outputFile("highscore.txt");
                    if (outputFile.is_open())
                    {
                        outputFile << score;
                        outputFile.close();
                        high.setString("HighScore: "+ std::to_string(score));
                        window.draw(high);
                    }
                }
                else{

                    std::ifstream file("highscore.txt");
                    if (file.is_open())
                    {
                        file >> highScore;
                        file.close();
                        high.setString("HighScore: "+ std::to_string(highScore));
                        window.draw(high);

                    }
                    
                }

            }
        }

        window.display();

    }

    return 0;
}

