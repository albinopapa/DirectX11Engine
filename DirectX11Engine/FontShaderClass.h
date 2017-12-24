////////////////////////////////////////////////////////////////////////////////
// Filename: fontshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include "ShaderClass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: FontShaderClass
////////////////////////////////////////////////////////////////////////////////
class FontShaderClass: public ShaderClass
{
private:
	struct PixelBufferType
	{
		XMFLOAT4 pixelColor;
	};

public:
	FontShaderClass();
	FontShaderClass( const FontShaderClass& ) = delete;

	void Render(
		ID3D11DeviceContext* deviceContext,
		int indexCount,
		XMMATRIX worldMatrix,
		XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture,
		XMFLOAT4 pixelColor );

private:
	void InitializeShader( ID3D11Device*, char*, char* ) override;
	void SetShaderParameters(
		ID3D11DeviceContext*,
		XMMATRIX,
		XMMATRIX,
		XMMATRIX,
		ID3D11ShaderResourceView*,
		XMFLOAT4 );

private:
	ComPtr<ID3D11Buffer> _pixelBuffer;
};