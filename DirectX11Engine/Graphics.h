#pragma once

////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

//////////////
// INCLUDES //
//////////////
//#include <windows.h>
#include "d3dclass.h"
#include "camera.h"
#include "textclass.h"
#include "model.h"
#include "LightClass.h"
#include "rendertextureclass.h"
#include "debugwindowclass.h"
#include "modellistclass.h"
#include "frustumclass.h"
#include "shadermanagerclass.h"
//#include "textureshaderclass.h"
//#include "LightShaderClass.h"

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
	bool ComposeFrame(/*float rotation, int mouseX, int mouseY, int fps, int cpu,*/ float frameTime, int fps, float posX, float posY, float posZ,
		float rotX, float rotY, float rotZ);
	bool DrawFrame(float frameTime);

	bool RenderToReflection(float time);

	bool RenderRefractionToTexture();

	void RenderText(/*const DirectX::XMMATRIX &worldMatrix, const DirectX::XMMATRIX &baseViewMatrix, const DirectX::XMMATRIX &orthoMatrix*/);

	inline Camera* GetCamera() { return _Camera; }
	bool UpdateRenderCounts(ID3D11DeviceContext*, int, int, int);

private:
	bool RenderReflectionToTexture();
	//bool RenderToTexture(float frameTime);
	bool RenderScene(float fogStart, float fogEnd, float frameTime);

	bool UpdateFpsString(ID3D11DeviceContext*, int);
	bool UpdatePositionStrings(ID3D11DeviceContext*, float, float, float, float, float, float);

private:
	D3DClass* _D3D;
	Camera* _Camera;
	TextClass* _Text; //@CUSTOM - now have multiple text classes holding different info
	FontClass* _Font1;

	TextClass *_FpsString, *_VideoStrings, *_PositionStrings;
	int _previousFps;
	int _previousPosition[6];
	TextClass* _RenderCountStrings;
	float textureTranslation = 0.f;

	Model* _Model;
	Model* _ModelSingle;
	LightClass* _Light;
	ModelListClass* _ModelList;
	FrustumClass* _Frustum;
	RenderTextureClass* _RenderTexture;
	DebugWindowClass* _DebugWindow;
	
	ShaderManagerClass* _ShaderManager;

	/////////// REFLECTION //////////////////
//	RenderTextureClass* _ReflectionTexture;
	Model* _FloorModel;

	float _modelRotation = 0.0f;

	/////////// WATER ////////////////
	Model*_GroundModel, *_WallModel, *_BathModel, *_WaterModel;
	//LightClass* _Light;

	RenderTextureClass *_RefractionTexture, *_ReflectionTexture;

	float _waterHeight, _waterTranslation;
};