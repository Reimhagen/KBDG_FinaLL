#ifndef BULLET_H
#define BULLET_H

#include "Sprite.h"
#include "BoundingBox.h"

namespace Engine {
	class Bullet
	{
	public:
		Bullet(Sprite* sprite);
		~Bullet();
		float xVelocity = 0;
		float yVelocity = 0;
		Sprite* sprite;
		void SetPosition(float x, float y);
		vec2 GetPosition();
		void Update(float deltaTime);
		void Draw();
		BoundingBox boundingBox;
	};
}
#endif

