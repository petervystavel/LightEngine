#pragma once

#include "Scene.h"

class SampleScene : public Scene
{
	Entity* pEntity1;
	Entity* pEntity2;

	Entity* pEntitySelected;

private:
	void TrySetSelectedEntity(Entity* pEntity, int x, int y);

public:
	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;
};


