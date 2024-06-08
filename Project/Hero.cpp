#include "Hero.h"

#include "LessonAI02_NPCWave.h"
#include "SATCollision.h"
#include <iostream>
#include <cmath>

Engine::Hero::Hero(Texture* texture, Shader* shader, Quad* quad) :BaseCharacter(texture, shader, quad)
{
	sprite->SetNumXFrames(8);
	sprite->SetNumYFrames(9);
	sprite->AddAnimation("up", 24, 31);
	sprite->AddAnimation("upright", 24, 31);
	sprite->AddAnimation("right", 24, 31);
	sprite->AddAnimation("downright", 24, 31);
	sprite->AddAnimation("down", 24, 31);
	sprite->AddAnimation("downleft", 24, 31);
	sprite->AddAnimation("left", 24, 31);
	sprite->AddAnimation("upleft", 24, 31);
	
	sprite->PlayAnim("down");
	sprite->SetScale(3);
	sprite->SetAnimationDuration(90);

	inputManager = NULL;
	

}

Engine::Hero::~Hero()
{
	
}

void Engine::Hero::Update(float deltaTime)
{
	if (Engine::CharacterState::ALIVE == state) {
		Move(deltaTime);
	}
	
}


void Engine::Hero::Move(float deltaTime)
{
	// Get Current Player's Position
	float speed = 0.07f;
	// s = v * t;
	// Control player's movement using keyboard or gamepad
	velocity = vec2(0, 0);
	string anim = "";
	if (inputManager->IsKeyPressed("W")) {
		velocity += vec2(0.0f, speed);
		anim += "up";
		isUp = true;
		isDown = false;
	}
	else {
		isUp = false;
	}

	if (inputManager->IsKeyPressed("S")) {
		velocity += vec2(0.0f, -speed);
		anim += "down";
		isDown = true;
		isUp = false;
	}
	else {
		isUp = true;
	}

	if (inputManager->IsKeyPressed("D")) {
		velocity += vec2(speed, 0.0f);
		anim += "right";
		isRight = true;
		isLeft = false;
	}
	else {
		isRight = false;
	}

	if (inputManager->IsKeyPressed("A")) {
		velocity += vec2(-speed, 0.0f);
		anim += "left";
		isLeft = true;
		isRight = false;

	}
	else {
		isLeft = false;
	}

	
	
	// Update new player's position
	vec2 newPosition = GetPosition() + velocity * deltaTime;
	sprite->SetPosition(newPosition.x, newPosition.y);
	// Update player's animation
	sprite->PlayAnim(anim);
	sprite->Update(deltaTime);

}

bool Engine::Hero::IsUp() const {
	return isUp;
}

bool Engine::Hero::IsDown() const {
	return isDown;
}

bool Engine::Hero::IsLeft() const {
	return isLeft;
}

bool Engine::Hero::IsRight() const {
	return isRight;
}

void Engine::Hero::Init(Input* inputManager)
{
	this->inputManager = inputManager;

	inputManager->AddInputMapping("A", SDLK_a);
	inputManager->AddInputMapping("A", SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	inputManager->AddInputMapping("A", SDL_CONTROLLER_AXIS_LEFTX);

	inputManager->AddInputMapping("D", SDLK_d);
	inputManager->AddInputMapping("D", SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	inputManager->AddInputMapping("D", SDL_CONTROLLER_AXIS_RIGHTX);

	inputManager->AddInputMapping("W", SDLK_w);
	inputManager->AddInputMapping("W", SDL_CONTROLLER_BUTTON_DPAD_UP);
	inputManager->AddInputMapping("W", SDL_CONTROLLER_AXIS_RIGHTY);

	inputManager->AddInputMapping("S", SDLK_s);
	inputManager->AddInputMapping("S", SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	inputManager->AddInputMapping("S", SDL_CONTROLLER_AXIS_LEFTY);


}
