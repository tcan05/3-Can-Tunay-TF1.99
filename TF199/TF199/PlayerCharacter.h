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
	bool isDead = false;
	Vector2 pos;
	Vector2 beginningPos;
	Color color;
	std::vector<Projectile> projectiles;

	// Input Keys
	int keyUp, keyDown, keyLeft, keyRight, keyShoot, keyDash;

	// Shooting Controls
	int shootingDelay;
	int shootingCooldown;
	Vector2 bulletDirection;

	// Dashing Controls
	int dashDistance;
	int dashCooldown;
	int dashTimer;

	// Methods
	PlayerCharacter(int id, Vector2 startPos);
	void Draw();
	void Update(float dt, std::vector<Projectile>& enemyProjectiles);
	void Reset(Vector2 startPos);
	void SetKeys(int up, int down, int left, int right, int shoot, int dash);
	void SetMaxHp(int max);
	bool ShouldFire();
};