#include "Entity.h"

#include "GameManager.h"
#include "Utils.h"
#include "Debug.h"

#include <cmath>

//#include <SFML/Graphics/Color.hpp>
//#include <SFML/Graphics/CircleShape.hpp>

void Entity::Initialize(float radius, const Color& color)
{
	mDirection = Vector2f(0.0f, 0.0f);

	mRadius = radius;
	mColor = color;
	mTarget.isSet = false;

	OnInitialize();
}

void Entity::Repulse(Entity* other) 
{
	Vector2f distance = GetPosition(0.5f, 0.5f) - other->GetPosition(0.5f, 0.5f);
	
	float sqrLength = (distance.x * distance.x) + (distance.y * distance.y);
	float length = std::sqrt(sqrLength);

	float radius1 = mRadius;
	float radius2 = other->mRadius;

	float overlap = (length - (radius1 + radius2)) * 0.5f;

	Vector2f normal = distance / length;

	Vector2f translation = overlap * normal;

	Vector2f position1 = GetPosition(0.5f, 0.5f) - translation;
	Vector2f position2 = other->GetPosition(0.5f, 0.5f) + translation;

	SetPosition(position1.x, position1.y, 0.5f, 0.5f);
	other->SetPosition(position2.x, position2.y, 0.5f, 0.5f);
}

bool Entity::IsColliding(Entity* other) const
{
	Vector2f distance = GetPosition(0.5f, 0.5f) - other->GetPosition(0.5f, 0.5f);

	float sqrLength = (distance.x * distance.x) + (distance.y * distance.y);

	float radius1 = mRadius;
	float radius2 = other->mRadius;

	float sqrRadius = (radius1 + radius2) * (radius1 + radius2);

	return sqrLength < sqrRadius;

	return false;
}

bool Entity::IsPointInside(float x, float y) const
{
	Vector2f position = GetPosition(0.5f, 0.5f);

	float dx = x - position.x;
	float dy = y - position.y;

	return (dx * dx + dy * dy) < (mRadius * mRadius);

	return false;
}

void Entity::Destroy()
{
	mToDestroy = true;

	OnDestroy();
}

void Entity::SetPosition(float x, float y, float ratioX, float ratioY)
{
	float size = mRadius * 2;

	x -= size * ratioX;
	y -= size * ratioY;

	mPosition = { x, y };

	//#TODO Optimise
	if (mTarget.isSet) 
	{
		Vector2f position = GetPosition(0.5f, 0.5f);
		mTarget.distance = Utils::GetDistance(position.x, position.y, mTarget.position.x, mTarget.position.y);
		GoToDirection(mTarget.position.x, mTarget.position.y);
		mTarget.isSet = true;
	}
}

Vector2f Entity::GetPosition(float ratioX, float ratioY) const
{
	float size = mRadius * 2;
	Vector2f position = mPosition;

	position.x += size * ratioX;
	position.y += size * ratioY;

	return position;
}

bool Entity::GoToDirection(int x, int y, float speed)
{
	Vector2f position = GetPosition(0.5f, 0.5f);
	Vector2f direction = { x - position.x, y - position.y };
	
	bool success = direction.Normalize();
	if (success == false)
		return false;

	SetDirection(direction.x, direction.y, speed);

	return true;
}

bool Entity::GoToPosition(int x, int y, float speed)
{
	if (GoToDirection(x, y, speed) == false)
		return false;

	Vector2f position = GetPosition(0.5f, 0.5f);

	mTarget.position = { x, y };
	mTarget.distance = Utils::GetDistance(position.x, position.y, x, y);
	mTarget.isSet = true;

	return true;
}

void Entity::SetDirection(float x, float y, float speed)
{
	if (speed > 0)
		mSpeed = speed;

	mDirection = { x, y };
	mTarget.isSet = false;
}

void Entity::Update()
{
	float dt = GetDeltaTime();
	float distance = dt * mSpeed;
	Vector2f translation = mDirection * distance;

	mPosition += translation;

	if (mTarget.isSet) 
	{
		float x1 = GetPosition(0.5f, 0.5f).x;
		float y1 = GetPosition(0.5f, 0.5f).y;

		float x2 = x1 + mDirection.x * mTarget.distance;
		float y2 = y1 + mDirection.y * mTarget.distance;

		//Debug::DrawLine(x1, y1, x2, y2, sf::Color::Cyan);

		//Debug::DrawCircle(mTarget.position.x, mTarget.position.y, 5.f, sf::Color::Magenta);

		mTarget.distance -= distance;

		if (mTarget.distance <= 0.f)
		{
			SetPosition(mTarget.position.x, mTarget.position.y, 0.5f, 0.5f);
			mDirection = Vector2f(0.f, 0.f);
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