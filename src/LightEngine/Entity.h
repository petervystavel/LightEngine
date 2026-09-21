#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>

namespace sf 
{
	class Shape;
    class Color;
}

class Scene;

class Entity
{
    struct Target 
    {
		sf::Vector2f position;
        float distance;
		bool isSet;
    };

public:
	enum MotionType 
	{
		Static,
		Kinematic,
		Dynamic
	};

	enum CollisionType 
	{
		Ignore,
		Overlap,
		Block
	};

protected:
    sf::CircleShape mShape;
    sf::Vector2f mDirection;
	Target mTarget;
    float mSpeed = 0.f;
    bool mToDestroy = false;
    int mTag = -1;

	MotionType mMotionType;
	CollisionType mCollisionType;

public:
	bool GoToward(sf::Vector2f position, float speed = -1.f);
    bool GoTo(sf::Vector2f position, float speed = -1.f);
    void SetPosition(sf::Vector2f position, float ratioX = 0.5f, float ratioY = 0.5f);
	void SetDirection(sf::Vector2f direction, float speed = -1.f);
	void SetSpeed(float speed) { mSpeed = speed; }
	void SetTag(int tag) { mTag = tag; }
	float GetRadius() const { return mShape.getRadius(); }
	void SetMotionType(MotionType type) { mMotionType = type; }
	void SetCollisionType(CollisionType type) { mCollisionType = type; }

    sf::Vector2f GetPosition(float ratioX = 0.5f, float ratioY = 0.5f) const;
	sf::Shape* GetShape() { return &mShape; }

	bool IsTag(int tag) const { return mTag == tag; }
    bool IsColliding(Entity* other) const;
	bool IsInside(float x, float y) const;

    void Destroy();
	bool ToDestroy() const { return mToDestroy; }
	
	template<typename T>
	T* GetScene() const;

    Scene* GetScene() const;
	float GetDeltaTime() const;

    template<typename T>
    T* CreateEntity(float radius, const sf::Color& color);

protected:
    Entity() = default;
    ~Entity() = default;

    virtual void OnUpdate() {};
    virtual void OnCollision(Entity* collidedWith) {};
	virtual void OnInitialize() {};
	virtual void OnDestroy() {};
	
private:
    void Update();
	void Initialize(float radius, const sf::Color& color);
	void Repulse(Entity* other);
	void CollisionReaction(Entity* other);

    friend class GameManager;
    friend Scene;
};

#include "Entity.inl"