#include "PlayerCharacter.h"
#include "raylib.h"

static inline float Clamp(float value, float min, float max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}


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
	this->isDead = false;
}

void PlayerCharacter::SetMaxHp(int max)
{
    this->maxHp = max;
    this->hp = maxHp;
    this->isDead = false;
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

void PlayerCharacter::Update(float dt, std::vector<Projectile>& enemyProjectiles)
{
	if (isDead)
	{
		return;
	}

	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;

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

			if (hp <= 0)
			{
				isDead = true;
				break;
			}
			else
			{
    			beginningPos = {
        		(float)GetRandomValue(150, 1130),
        		(float)GetRandomValue(150, 570)
    		};
    			pos = beginningPos;
			}

		}
	}
	// Remove Inactive Enemy Projectile
	enemyProjectiles.erase(std::remove_if(enemyProjectiles.begin(), enemyProjectiles.end(),
		[](const Projectile& p) { return !p.isActive; }),
		enemyProjectiles.end());

	// Player Death
	

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

	// Keep Player Within Screen Bounds
	pos.x = Clamp(pos.x, radius, 1280 - radius);
	pos.y = Clamp(pos.y, radius, 720 - radius);
}