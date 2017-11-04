#pragma once

////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////

//////////////
// INCLUDES //
//////////////
//#include <windows.h>
#include "d3dclass.h"
#include "camera.h"
#include "model.h"
#include "LightShaderClass.h"
#include "LightClass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class Graphics
{
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	// Create the D3DClass object and call the D3DClass Initialize function.
	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int fps, int cpu, float frameTime);

	inline Camera* GetCamera() { return _Camera; }

private:
	bool Render(float);

private:
	D3DClass* _D3D;
	Camera* _Camera;
	Model* _Model;
	LightShaderClass* _LightShader;
	LightClass* _Light;

	float _modelRotation = 0.0f;
};