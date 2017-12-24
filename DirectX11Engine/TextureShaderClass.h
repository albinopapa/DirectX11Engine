////////////////////////////////////////////////////////////////////////////////
// Filename: textureshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include "ShaderClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: TextureShaderClass
// Description: Used to invoke the HLSL shaders for drawing the 3D models that are on the GPU.
////////////////////////////////////////////////////////////////////////////////
class TextureShaderClass:public ShaderClass
{
public:
	TextureShaderClass();
	TextureShaderClass(const TextureShaderClass&) = delete;

	void Render(
		ID3D11DeviceContext* deviceContext, 
		int indexCount, 
		XMMATRIX worldMatrix, 
		XMMATRIX viewMatrix, 
		XMMATRIX projectionMatrix, 
		ID3D11ShaderResourceView* texture);

private:
	/** actually loads the shader files and makes it usable to DirectX and the GPU. */
	void InitializeShader( ID3D11Device* device, char* vsFileName, char* psFileName ) override;

	void SetShaderParameters(
		ID3D11DeviceContext*,
		XMMATRIX worldMatrix,
		XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture );
};