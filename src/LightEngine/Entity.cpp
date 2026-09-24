#include "Entity.h"

#include "GameManager.h"
#include "Utils.h"
#include "Debug.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <iostream>

void Entity::Initialize(float radius, const sf::Color& color)
{
	mVelocity = { 0, 0 };

	mShape.setOrigin(0.f, 0.f);
	mShape.setRadius(radius);
	mShape.setFillColor(color);

	OnInitialize();
}

bool Entity::IsColliding(Entity* other) const
{
	sf::Vector2f distance = GetPosition(0.5f, 0.5f) - other->GetPosition(0.5f, 0.5f);

	float sqrLength = (distance.x * distance.x) + (distance.y * distance.y);

	float radius1 = mShape.getRadius();
	float radius2 = other->mShape.getRadius();

	float sqrRadius = (radius1 + radius2) * (radius1 + radius2);

	return sqrLength < sqrRadius;
}

bool Entity::IsInside(float x, float y) const
{
	sf::Vector2f position = GetPosition(0.5f, 0.5f);

	float dx = x - position.x;
	float dy = y - position.y;

	float radius = mShape.getRadius();

	return (dx * dx + dy * dy) < (radius * radius);
}

void Entity::Destroy()
{
	mToDestroy = true;

	OnDestroy();
}

void Entity::AddForce(sf::Vector2f direction, float strength)
{
	mForces.push_back({ direction, strength });
}

void Entity::AddImpulse(sf::Vector2f direction, float impulse)
{
	Utils::Normalize(direction);

	mVelocity += direction * impulse;
}

void Entity::AddImpulse(sf::Vector2f velocity)
{
	mVelocity += velocity;
}

void Entity::TryBounce(sf::Vector2f normal, float restitution)
{
	float dotProduct = Utils::Dot(mVelocity, normal);
	if (dotProduct <= 0)
		return;

	Bounce(normal, restitution);
}

void Entity::Bounce(sf::Vector2f normal, float restitution)
{
	float dotProduct = Utils::Dot(mVelocity, normal);
	sf::Vector2f v1 = normal * dotProduct;
	sf::Vector2f v2 = mVelocity - v1;
	v1 *= -1.f;

	mVelocity = (v1 + v2) * restitution;
}

void Entity::SetPosition(sf::Vector2f newPosition, float ratioX, float ratioY)
{
	float size = mShape.getRadius() * 2;

	newPosition.x -= size * ratioX;
	newPosition.y -= size * ratioY;

	mShape.setPosition(newPosition.x, newPosition.y);
}

sf::Vector2f Entity::GetPosition(float ratioX, float ratioY) const
{
	float size = mShape.getRadius() * 2;
	sf::Vector2f position = mShape.getPosition();

	position.x += size * ratioX;
	position.y += size * ratioY;

	return position;
}

float Entity::GetX(float ratioX) const
{
	float size = mShape.getRadius() * 2;
	sf::Vector2f position = mShape.getPosition();

	position.x += size * ratioX;

	return position.x;
}

float Entity::GetY(float ratioY) const
{
	float size = mShape.getRadius() * 2;
	sf::Vector2f position = mShape.getPosition();

	position.y += size * ratioY;

	return position.y;
}

void Entity::FixedUpdate()
{
	float fixedDt = GetFixedDeltaTime();

	for (int i = 0; i < mForces.size(); ++i)
	{
		Force& force = mForces.front();

		sf::Vector2f direction = force.direction;
		Utils::Normalize(direction);
		float strength = force.force;

		mVelocity += direction * strength * fixedDt;
	}

	sf::Vector2f translation = mVelocity * fixedDt;

	mShape.move(translation);

	float top = GetY(0.f);
	std::cout << top << std::endl;
	if (top < 0.f)
		TryBounce({ 0, -1 }, 0.5f);

	float bottom = GetY(1.f);
	if (bottom > GetWindowHeight())
		TryBounce({ 0, 1 }, 0.5f);

	float left = GetX(0.f);
	if (left < 0)
		TryBounce({ -1, 0 }, 0.5f);

	float right = GetX(1.f);
	if (right > GetWindowWidth())
		TryBounce({ 1, 0 }, 0.5f);
}

void Entity::Update()
{
	OnUpdate();
}

Scene* Entity::GetScene() const
{
	return GameManager::Get()->GetScene();
}

float Entity::GetDeltaTime() const
{
	return GameManager::Get()->GetDeltaTime();
}

float Entity::GetFixedDeltaTime() const
{
	return GameManager::Get()->GetFixedDeltaTime();
}

int Entity::GetWindowWidth() const
{
	return GetScene()->GetWindowWidth();
}

int Entity::GetWindowHeight() const
{
	return GetScene()->GetWindowHeight();
}