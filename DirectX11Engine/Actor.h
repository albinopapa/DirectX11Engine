#pragma once

#include "GfxUtil.h"
#include "Position.h" 
#include "Model.h"

using namespace GfxUtil;

class Actor
{
public:
	Actor();
	~Actor();

	bool InitializeMovement(bool visible);

	inline MovementComponent* GetMovementComponent()
	{
		return _MovementComp.get();
	}

	inline void SetModel(Model* model){_Model = model;}

	inline Model* GetModel() const { return _Model; }

	bool bCustomAppearance = false;
	bool bVisible = true;

private:
	unique_ptr<MovementComponent> _MovementComp;
	Model* _Model;
};

