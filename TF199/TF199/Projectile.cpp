#include "Projectile.h"

Projectile::Projectile() {}

Projectile::Projectile(int owner, Vector2 startPos, Vector2 direction)
{
	this->ownerId = owner;
	this->color = (ownerId == 1) ? DARKBLUE : ORANGE;

	this->pos = startPos;
	int speed = 1000;

	this->velocity = {direction.x * speed, direction.y * speed};
	this->radius = 6;
	this->isActive = true;
}

void Projectile::Update(float dt)
{
	if (!isActive)
	{
		return;
	}
	pos.x += velocity.x * dt;
	pos.y += velocity.y * dt;
}

void Projectile::Draw()
{
	if (!isActive)
	{
		return;
	}
	DrawCircleV(pos, radius, color);
}

Vector2 Projectile::GetPosition()
{
	return pos;
}

int Projectile::GetRadius()
{
	return radius;
}

int Projectile::GetOwnerId()
{
	return ownerId;
}