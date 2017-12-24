#pragma once
#include "ShaderClass.h"
class RefractionShaderClass :
	public ShaderClass
{
private:
	struct LightBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;
	};

	struct ClipPlaneBufferType
	{
		XMFLOAT4 clipPlane;
	};

public:
	RefractionShaderClass();
	RefractionShaderClass(const RefractionShaderClass& other) = delete;

	void Render(
		ID3D11DeviceContext* deviceContext, 
		int indexCount, 
		XMMATRIX worldMatrix,
		XMMATRIX viewMatrix, 
		XMMATRIX projectionMatrix, 
		ID3D11ShaderResourceView* texture,
		XMFLOAT3 lightDirection, 
		XMFLOAT4 ambientColor, 
		XMFLOAT4 diffuseColor,
		XMFLOAT4 clipPlane);

private:
	void InitializeShader(ID3D11Device* device, char* vsFilename, char* psFilename);
	void SetShaderParameters(
		ID3D11DeviceContext* deviceContext, 
		XMMATRIX worldMatrix, 
		XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, 
		ID3D11ShaderResourceView* texture,
		XMFLOAT3 lightDirection, 
		XMFLOAT4 ambientColor, 
		XMFLOAT4 diffuseColor,
		XMFLOAT4 clipPlane);

private:
	ComPtr<ID3D11Buffer> _lightBuffer;
	ComPtr<ID3D11Buffer> _clipPlaneBuffer;
};

