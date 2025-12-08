#include "PlayerCharacter.h"
#include "raylib.h"

PlayerCharacter::PlayerCharacter() {}

PlayerCharacter::PlayerCharacter(int id, Vector2 startPos) : id(id), pos(startPos), beginningPos(startPos)
{
	this->color = (id == 1) ? BLUE : RED;

	this->radius = 20;
	this->speed = 7;
	this->maxHp = 3;
	this->hp = maxHp;

	this->shootingDelay = 30;
	this->shootingCooldown = 0;

	this->dashDistance = 100;
	this->dashCooldown = 5;
	this->dashTimer = 0;
}

void PlayerCharacter::Draw()
{
	// Draw Player
	DrawCircleV(pos, radius, color);

	Vector2 eye = { pos.x - radius / 4.0f, pos.y - radius / 4.0f };
	DrawCircleV(eye, 4, WHITE);

	// Draw Projectiles
	for (auto& projectile : projectiles)
	{
		projectile.Draw();
	}
}

void PlayerCharacter::Reset(Vector2 startPos)
{
	this->pos = startPos;
	this->hp = maxHp;
	this->shootingCooldown = 0;
	this->dashTimer = 0;
}

void PlayerCharacter::SetMaxHp(int max)
{
	this->maxHp = max;
}

void PlayerCharacter::SetKeys(int up, int down, int left, int right, int shoot, int dash)
{
	this->keyUp = up;
	this->keyDown = down;
	this->keyLeft = left;
	this->keyRight = right;
	this->keyShoot = shoot;
	this->keyDash = dash;
}

bool PlayerCharacter::ShouldFire()
{
	if (shootingCooldown > 0)
	{
		return false;
	}
	if (IsKeyDown(keyShoot))
	{
		shootingCooldown = shootingDelay;
		return true;
	}
	return false;
}

void PlayerCharacter::Update(float dt, std::vector<Projectile>& enemyProjectiles)
{
	// Movement
	Vector2 movement = {};

	if (IsKeyDown(keyUp))
	{
		movement.y -= 1;
		pos.y -= speed;
	}
	if (IsKeyDown(keyDown))
	{
		movement.y += 1;
		pos.y += speed;
	}
	if (IsKeyDown(keyLeft))
	{
		movement.x -= 1;
		pos.x -= speed;
	}
	if (IsKeyDown(keyRight))
	{
		movement.x += 1;
		pos.x += speed;
	}

	if (movement.x != 0 || movement.y != 0)
	{
		float len = sqrt(movement.x * movement.x + movement.y * movement.y);
		bulletDirection = { movement.x / len, movement.y / len };
	}

	// Shooting Cooldown
	if (shootingCooldown > 0)
	{
		shootingCooldown--;
	}

	// Shooting
	if (shootingCooldown <= 0 && IsKeyDown(keyShoot))
	{
		projectiles.emplace_back(id, pos, bulletDirection);
		shootingCooldown = shootingDelay;
	}

	// Update Projectile Location
	for (auto& projectile : projectiles)
	{
		projectile.Update(dt);
	}
	// Remove Inactive Player Projectile
	projectiles.erase(
		std::remove_if(projectiles.begin(), projectiles.end(),
			[](const Projectile& p) { return !p.isActive; }),
		projectiles.end());

	// Check Enemy Projectile Collision
	for (auto& projectile : enemyProjectiles)
	{
		if (CheckCollisionCircles(pos, radius, projectile.GetPosition(), projectile.GetRadius()))
		{
			hp--;
			projectile.isActive = false;
		}
	}
	// Remove Inactive Enemy Projectile
	enemyProjectiles.erase(std::remove_if(enemyProjectiles.begin(), enemyProjectiles.end(),
		[](const Projectile& p) { return !p.isActive; }),
		enemyProjectiles.end());

	if (hp <= 0)
	{
		Reset(beginningPos);
	}

	// Dashing
	if (dashTimer > 0)
	{
		dashTimer--;
	}
	else if (IsKeyDown(keyDash))
	{
		if (IsKeyDown(keyUp))
		{
			pos.y -= dashDistance;
		}
		else if (IsKeyDown(keyDown))
		{
			pos.y += dashDistance;
		}
		else if (IsKeyDown(keyLeft))
		{
			pos.x -= dashDistance;
		}
		else if (IsKeyDown(keyRight))
		{
			pos.x += dashDistance;
		}
		dashTimer = dashCooldown * 60;
	}
}