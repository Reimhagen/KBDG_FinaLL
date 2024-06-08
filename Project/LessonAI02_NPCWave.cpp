#include "LessonAI02_NPCWave.h"
//#include "SATCollision.h"
#include "Texture.h"
#include "MainMenuScreen.h"
#include "Enemy.h"
#include "Hero.h"
#include "Bullet.h"
#include "Sprite.h"
#include <algorithm>
#include <random>


Engine::LessonAI02_NPCWave::LessonAI02_NPCWave(Setting* setting) :Engine::Game(setting), setting(setting), inGame(false)
{
    
	texture = NULL;
	spawnTimer = 0.0f;
	hero = NULL;
	setting->windowTitle = "Enemy Wave Tembak";

    bgTexture = new Texture("bgg12.png");
}

Engine::LessonAI02_NPCWave::~LessonAI02_NPCWave()
{
	delete hero;
	delete mainMenu;
    delete texture;
    delete bulletTexture;
    delete textureEnemy;
    delete bgTexture;

    
    for (Enemy* enemy : enemies) {
        delete enemy;
    }
    for (Bullet* bullet : readyBullets) {
        delete bullet;
    }
    for (Bullet* bullet : inUseBullets) {
        delete bullet;
    }

}

void Engine::LessonAI02_NPCWave::Init()
{
	texture = new Texture("mainchara.png");
	SetBackgroundColor(0, 0, 0);
	mainMenu = new MainMenuScreen();
	mainMenu->SetGame(this);
	mainMenu->Init();
    
    
    
    bulletTexture = new Texture("bullet.png");
    int bulletNum = 50;
    for (int i = 0; i < bulletNum; i++) {
        Sprite* bs = (new Sprite(bulletTexture, defaultSpriteShader, defaultQuad));
        bs->SetNumXFrames(1);
        bs->SetNumYFrames(1);
        bs->SetScale(2);
        readyBullets.push_back(new Bullet(bs));
    }

    textureEnemy = new Texture("turtles.png");

   

    inputManager->AddInputMapping("Shoot", SDLK_SPACE);

	
}

void Engine::LessonAI02_NPCWave::Update() {
    if (!inGame) {
        mainMenu->Update();
    }
    else {

        timeInterval += GetGameTime();

        if (inputManager->IsKeyPressed("Shoot")) {
            
            SpawnBullet();

        }

        if (inputManager->IsKeyPressed("quit")) {
            state = Engine::State::EXIT;
        }

        // Spawn Enemies
        spawnTimer += GetGameTime();
        if (spawnTimer >= 1000) {

            // Create a new enemy
            Enemy* enemy = new Enemy(textureEnemy, defaultSpriteShader, defaultQuad);

            // Set random enemy's position
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> disX(0, setting->screenWidth);
            std::uniform_int_distribution<> disY(0, setting->screenHeight);
            std::uniform_int_distribution<> disBorder(0, 3);

            int border = disBorder(gen);
            float x = (float)disX(gen);
            float y = (float)disY(gen);

            // 0 = spawn from bottom, 1 = left, 2 = top, 3 = right
            if (0 == border) {
                y = -enemy->GetSprite()->GetScaleHeight();
            }
            else if (1 == border) {
                x = -enemy->GetSprite()->GetScaleWidth();
            }
            else if (2 == border) {
                y = (float)setting->screenHeight;
            }
            else {
                x = (float)setting->screenWidth;
            }
            enemy->SetPosition(x, y);
            enemy->SetMaxSpeed(0.06f);
            enemy->SetTarget(hero);
            enemies.push_back(enemy);
            spawnTimer = 0;
        }

        for (Enemy* e : enemies) {
            e->Update(GetGameTime());

            for (auto bIter = inUseBullets.begin(); bIter != inUseBullets.end(); ) { // Use iterator for inUseBullets
                Bullet* b = *bIter;
                b->Update(GetGameTime());

                for (auto eIter = enemies.begin(); eIter != enemies.end(); ) { // Use iterator for enemies
                    Enemy* e = *eIter;

                    if (b->boundingBox.CollideWith(&e->boundingBox)) {
                        // Remove bullet
                        b->SetPosition(0.0f, 0.0f);
                        b->xVelocity = 0.0f;
                        b->yVelocity = 0.0f;
                        readyBullets.push_back(b);

                        bIter = inUseBullets.erase(bIter); // Erase and update iterator

                        // Remove enemy
                        eIter = enemies.erase(eIter);  // Erase and update iterator
                        delete e;

                        break; // Break out of inner loop since bullet is gone
                    }
                    else {
                        ++eIter; // Move to the next enemy if no collision
                    }
                }
                if (bIter != inUseBullets.end()) { // Check if bullet still exists
                    ++bIter; // Move to the next bullet if not erased
                }
            }
        }

        hero->Update(GetGameTime());

        for (Bullet* b : inUseBullets) {
            // If bullet off screen then remove a bullet from in-use container, and insert into ready-to-use container
            if (b->GetPosition().x < -b->sprite->GetScaleWidth() || b->GetPosition().x > setting->screenWidth) {
                b->SetPosition(0.0f, 0.0f);
                b->xVelocity = 0.0f;
                b->yVelocity = 0.0f;
                readyBullets.push_back(b);
                inUseBullets.erase(std::remove(inUseBullets.begin(), inUseBullets.end(), b), inUseBullets.end());
            }

            b->Update(GetGameTime());
        }


    }

}

void Engine::LessonAI02_NPCWave::Render() {
    if (!inGame) {
        mainMenu->Draw();
    }
    else {

        

        for (Enemy* enemy : enemies) {
            enemy->Draw();
        }
        hero->Draw();

        for (Bullet* b : inUseBullets) {
            b->Draw();
        }

        

    }
}

void Engine::LessonAI02_NPCWave::ShowMainMenu() {
    inGame = false;
    mainMenu->Init();
}

void Engine::LessonAI02_NPCWave::SpawnBullet()
{
    if (timeInterval >= 150) {

        if (hero == nullptr) {
            std::cout << "Error: hero is null!" << std::endl;
            return;
        }

        if (readyBullets.empty()) {
            std::cout << "No ready bullets available!" << std::endl;
            return;
        }

        Bullet* b = readyBullets.back();
        readyBullets.pop_back();
        b->SetPosition(hero->GetPosition().x, hero->GetPosition().y);
        
        
        b->xVelocity = 0.0f;
        b->yVelocity = 0.0f;
        
        if (hero->IsUp()) {
            b->yVelocity = 0.6f;
            
        }

        else if (hero->IsDown()) {
            b->yVelocity = -0.6f;
        }

        if (hero->IsRight()) {
            b->xVelocity = 0.6f;
            b->yVelocity = 0.0f;
            
        }

        else if (hero->IsLeft()) {
            b->xVelocity = -0.6f;
            b->yVelocity = 0.0f;
        }

        inUseBullets.push_back(b);
        timeInterval = 0;

        std::cout << "Bullet spawned: xVelocity = " << b->xVelocity << ", yVelocity = " << b->yVelocity << std::endl;

    }


}

void Engine::LessonAI02_NPCWave::StartGame() {
    inGame = true;

    sprite = new Sprite(texture, defaultSpriteShader, defaultQuad);
    hero = new Hero(texture, defaultSpriteShader, defaultQuad);
    hero->Init(inputManager);
    hero->SetPosition(setting->screenWidth / 2.0f - hero->GetSprite()->GetScaleWidth(), setting->screenHeight / 2.0f);
    //inputManager->AddInputMapping("Shoot", SDLK_SPACE);

    
}



