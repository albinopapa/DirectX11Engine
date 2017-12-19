#include "Actor.h"



Actor::Actor()
{
	_MovementComp.reset(new MovementComponent);
	//_Model.reset(new Model());
}

Actor::~Actor()
{
}

bool Actor::Initialize(bool visible)
{
	bVisible = visible;

	_MovementComp->Initialize();
	//_Model->Initialize();

	return true;
}