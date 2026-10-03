#pragma once

#include "Scene.h"
#include "GameManager.h"
#include "Entity.h"

//template<typename T>
//T* Scene::CreateEntity(float radius, const sf::Color& color)
//{
//	static_assert(std::is_base_of<Entity, T>::value, "T must be derived from Entity");
//
//	T* newEntity = new T();
//
//	Entity* entity = newEntity;
//	entity->Initialize(radius, color);
//	
//	mpGameManager->mEntitiesToAdd.push_back(newEntity);
//
//	return newEntity;
//}
