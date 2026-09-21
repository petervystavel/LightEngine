#include "Entity.h"

#include "GameManager.h"
#include "Utils.h"
#include "Debug.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/CircleShape.hpp>

void Entity::Initialize(float radius, const sf::Color& color)
{
	mDirection = sf::Vector2f(0.0f, 0.0f);

	mShape.setOrigin(0.f, 0.f);
	mShape.setRadius(radius);
	mShape.setFillColor(color);

	mTarget.isSet = false;

	OnInitialize();
}

void Entity::Repulse(Entity* other)
{
	sf::Vector2f distance = GetPosition(0.5f, 0.5f) - other->GetPosition(0.5f, 0.5f);

	float sqrLength = (distance.x * distance.x) + (distance.y * distance.y);
	float length = std::sqrt(sqrLength);

	float radius1 = mShape.getRadius();
	float radius2 = other->mShape.getRadius();

	float overlap = (length - (radius1 + radius2)) * 0.5f;

	sf::Vector2f normal = distance / length;

	sf::Vector2f translation = overlap * normal;

	sf::Vector2f position1 = GetPosition(0.5f, 0.5f) - translation;
	sf::Vector2f position2 = other->GetPosition(0.5f, 0.5f) + translation;

	SetPosition(position1, 0.5f, 0.5f);
	other->SetPosition(position2, 0.5f, 0.5f);
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

void Entity::SetPosition(sf::Vector2f newPosition, float ratioX, float ratioY)
{
	float size = mShape.getRadius() * 2;

	newPosition.x -= size * ratioX;
	newPosition.y -= size * ratioY;

	mShape.setPosition(newPosition.x, newPosition.y);

	if (mTarget.isSet)
	{
		GoTo(mTarget.position);

		//sf::Vector2f position = GetPosition(0.5f, 0.5f);
		//mTarget.distance = Utils::GetDistance(position.x, position.y, mTarget.position.x, mTarget.position.y);
		//GoToward({ mTarget.position.x, mTarget.position.y });
		//mTarget.isSet = true;
	}
}

sf::Vector2f Entity::GetPosition(float ratioX, float ratioY) const
{
	float size = mShape.getRadius() * 2;
	sf::Vector2f position = mShape.getPosition();

	position.x += size * ratioX;
	position.y += size * ratioY;

	return position;
}

bool Entity::GoToward(sf::Vector2f newPosition, float speed)
{
	sf::Vector2f position = GetPosition(0.5f, 0.5f);

	sf::Vector2f direction = newPosition - position;
	bool success = Utils::Normalize(direction);
	if (success == false)
		return false;

	SetDirection(direction, speed);

	return true;
}

bool Entity::GoTo(sf::Vector2f newPosition, float speed)
{
	if (GoToward(newPosition, speed) == false)
		return false;

	sf::Vector2f position = GetPosition(0.5f, 0.5f);

	mTarget.position = newPosition;
	mTarget.distance = Utils::GetDistance(newPosition.x, newPosition.y, position.x, position.y);
	mTarget.isSet = true;

	return true;
}

void Entity::SetDirection(sf::Vector2f direction, float speed)
{
	if (speed > 0)
		mSpeed = speed;

	bool success = Utils::Normalize(direction);
	_ASSERT(success);

	mDirection = direction;
	mTarget.isSet = false;
}

void Entity::Update()
{
	float dt = GetDeltaTime();
	float distance = dt * mSpeed;
	sf::Vector2f translation = distance * mDirection;
	mShape.move(translation);

	if (mTarget.isSet)
	{
		sf::Vector2f position = GetPosition(0.5f, 0.5f);
		sf::Vector2f target = mTarget.position;

		Debug::DrawLine(position, target, sf::Color::Cyan);
		Debug::DrawCircle(target, 5.f, sf::Color::Magenta);

		mTarget.distance -= distance;

		if (mTarget.distance <= 0.f)
		{
			SetPosition(mTarget.position, 0.5f, 0.5f);
			mDirection = { 0, 0 };
			mTarget.isSet = false;
		}
	}

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