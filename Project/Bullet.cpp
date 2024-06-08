#include "Bullet.h"

Engine::Bullet::Bullet(Sprite* sprite)
{
	this->sprite = sprite;
}

Engine::Bullet::~Bullet()
{
}

void Engine::Bullet::SetPosition(float x, float y)
{
	sprite->SetPosition(x, y);
}

vec2 Engine::Bullet::GetPosition()
{
	return sprite->GetPosition();
}

void Engine::Bullet::Update(float deltaTime)
{
	float x = GetPosition().x;
	float y = GetPosition().y;

	x += xVelocity * deltaTime;
	y += yVelocity * deltaTime; 
	
	SetPosition(x, y);
	sprite->Update(deltaTime);

	vec2 p1 = sprite->GetPosition();
	vec2 p2 = p1 + vec2(sprite->GetScaleWidth(), 0.0f);
	vec2 p3 = p1 + vec2(sprite->GetScaleWidth(), sprite->GetScaleHeight());
	vec2 p4 = p1 + vec2(0.0f, sprite->GetScaleHeight());
	boundingBox.SetVertices(p1, p2, p3, p4);
}

void Engine::Bullet::Draw()
{
	sprite->Draw();
}
