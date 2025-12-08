#pragma once
#include "raylib.h"
#include <vector>
#include "Projectile.h"

class PlayerCharacter
{
public:

	// Attributes
	int id;
	int hp;
	int maxHp;
	int radius;
	int speed;
	Vector2 pos;
	Color color;
	std::vector<Projectile> projectiles;

	// Input Keys
	int keyUp, keyDown, keyLeft, keyRight, keyShoot, keyDash;

	// Shooting Controls
	int shootingDelay;
	int shootingCooldown;
	Vector2 bulletDirection = {0, -1};

	// Dashing Controls
	int dashDistance;
	int dashCooldown;
	int dashTimer;

	// Methods
	PlayerCharacter();
	PlayerCharacter(int id, Vector2 startPos);
	void Draw();
	void Update(float dt);
	void Reset(Vector2 startPos);
	void SetKeys(int up, int down, int left, int right, int shoot, int dash);
	void SetMaxHp(int max);
	bool ShouldFire();
};