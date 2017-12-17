#pragma once

#include <memory>
#include <vector>

#include "Actor.h"
#include "Position.h" 
#include "LightClass.h"

using namespace std;

class Scene
{
public:
	Scene();
	~Scene();
	bool Initialize();
	void Tick(float deltaTime, class Input* pInput);
	void ProcessInput(float deltaTime, Input * pInput);
	void UpdateActors(float deltaTime);
	
	//unique_ptr<MovementComponent> _Camera;
	unique_ptr<Actor> _Camera;
	vector<unique_ptr<Actor>> _Actors;
	vector<unique_ptr<LightClass>> _Lights;
};
