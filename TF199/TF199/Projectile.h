#pragma once
#include "raylib.h"

class Projectile
{
public:

	//Attributes
	Vector2 pos;
	Vector2 velocity;
	int ownerId;
	int radius;
	bool isActive;
	Color color;

	// Methods
	Projectile(int owner, Vector2 startPos, Vector2 direction);
	void Update(float dt);
	void Draw();
	int GetRadius();
	int GetOwnerId();
	Vector2 GetPosition();
};

