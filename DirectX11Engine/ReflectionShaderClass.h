#pragma once

#include "ShaderClass.h"

class ReflectionShaderClass : public ShaderClass
{
private:
	struct ReflectionBufferType
	{
		XMMATRIX reflectionMatrix;
	};

public:
	ReflectionShaderClass();
	ReflectionShaderClass( const ReflectionShaderClass& ) = delete;

	void Render( 
		ID3D11DeviceContext* deviceContext, 
		int indexCount, 
		XMMATRIX worldMatrix,
		XMMATRIX viewMatrix, 
		XMMATRIX projectionMatrix, 
		ID3D11ShaderResourceView* texture,
		ID3D11ShaderResourceView* reflectionTexture, 
		XMMATRIX reflectionMatrix );

private:
	void InitializeShader( 
		ID3D11Device* device, 
		char* vsFilename, 
		char* psFilename );

	void SetShaderParameters( 
		ID3D11DeviceContext* deviceContext, 
		XMMATRIX worldMatrix, 
		XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, 
		ID3D11ShaderResourceView* texture, 
		ID3D11ShaderResourceView* reflectionTexture, 
		XMMATRIX reflectionMatrix );

private:
	ComPtr<ID3D11Buffer> _reflectionBuffer;
};

