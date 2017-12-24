////////////////////////////////////////////////////////////////////////////////
// Filename: shadermanagerclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"

#include "TextureShaderClass.h"
#include "LightShaderClass.h"
#include "FontShaderClass.h"
#include "ReflectionShaderClass.h"
#include "WaterShaderClass.h"
#include "RefractionShaderClass.h"

#include "GfxUtil.h"

#include "LightClass.h"


using namespace GfxUtil;

////////////////////////////////////////////////////////////////////////////////
// Class name: ShaderManagerClass
////////////////////////////////////////////////////////////////////////////////
class ShaderManagerClass
{
public:
	ShaderManagerClass();
	ShaderManagerClass(const ShaderManagerClass&) = delete;

	void Initialize(ID3D11Device*);

	void Render(
		ID3D11DeviceContext* device, 
		int indexCount, 
		XMMATRIX worldMatrix, 
		XMMATRIX viewMatrix, 
		XMMATRIX projectionMatrix, 
		Material* material, 
		LightClass* light, 
		const std::vector<std::unique_ptr<LightClass>>& lights,
		SceneEffects effects,
		XMFLOAT3 cameraPos = XMFLOAT3(0,0,0), 
		XMMATRIX reflectionMatrix = XMMatrixIdentity(), 
		ID3D11ShaderResourceView * reflectionTexture = nullptr, 
		ID3D11ShaderResourceView * refractionTexture = nullptr, 
		ID3D11ShaderResourceView * normalTexture = nullptr);

	void RenderFontShader(
		ID3D11DeviceContext* deviceContext,
		int indexCount,
		XMMATRIX worldMatrix,
		XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture,
		XMFLOAT4 pixelColor );

	FontShaderClass* GetFontShader();

private:
	// Make a private class object for each shader type the application will be using.
	unique_ptr<TextureShaderClass> _TextureShader;
	unique_ptr<LightShaderClass> _LightShader;
	unique_ptr<FontShaderClass> _FontShader;
	unique_ptr<ReflectionShaderClass> _ReflectionShader;
	unique_ptr<WaterShaderClass> _WaterShader;
	unique_ptr<RefractionShaderClass> _RefractionShader;
};