#ifndef LESSONAI02_NPCWAVE_H
#define LESSONAI02_NPCWAVE_H

#include "Game.h"
#include "Setting.h"
#include "Texture.h"
#include "Sprite.h"
#include "Enemy.h"
#include "Hero.h"
#include "MainMenuScreen.h"
#include "Bullet.h"
#include <random> 
#include <vector>

namespace Engine {
	class LessonAI02_NPCWave :public Engine::Game
	{
	public:
		LessonAI02_NPCWave(Setting* setting);
		~LessonAI02_NPCWave();
		virtual void Init();
		virtual void Update();
		virtual void Render();
		

		void StartGame();
		void ShowMainMenu();
		std::vector<Enemy*> enemies;
		float timeInterval = 0;
		//std::vector<Bullet*> inUseBullets;
		//std::vector<Bullet*> readyBullets;

	private:

		vector<Bullet*> inUseBullets;
		vector<Bullet*> readyBullets;
		
		void SpawnBullet();

		
		Texture* texture;
		Texture* bulletTexture;
		Texture* textureEnemy;
		Texture* bgTexture;
		Sprite* sprite;
		float spawnTimer;
		
		

		Hero* hero;
		MainMenuScreen* mainMenu;
		bool inGame;
		
		Setting* setting;
	};
}
#endif