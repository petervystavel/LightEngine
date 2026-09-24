#pragma once

#include "Scene.h"

class SampleScene : public Scene
{
	Entity* pEntity1;
	Entity* pEntity2;

	Entity* pEntitySelected;

	const float mStrengthMax = 1000;
	const float mLineLengthMax = 200;
	sf::Vector2f mVelocity;

private:
	void TrySetSelectedEntity(Entity* pEntity, int x, int y);
	void TryDrawLine();

public:
	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;
};


