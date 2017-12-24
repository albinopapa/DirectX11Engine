#pragma once
#include "ShaderClass.h"

class WaterShaderClass :
	public ShaderClass
{
private:
	struct ReflectionBufferType
	{
		XMMATRIX reflection;
	};

	struct WaterBufferType
	{
		float waterTranslation;
		float reflectRefractScale;
		XMFLOAT2 padding;
	};

public:
	WaterShaderClass();
	WaterShaderClass( const WaterShaderClass& ) = delete;

	void Render(
		ID3D11DeviceContext*,
		int,
		XMMATRIX,
		XMMATRIX,
		XMMATRIX,
		XMMATRIX,
		ID3D11ShaderResourceView** textureArray,
		float,
		float );

private:
	void InitializeShader(ID3D11Device*, char*, char*) override;

	void SetShaderParameters(
		ID3D11DeviceContext*, 
		XMMATRIX, 
		XMMATRIX, 
		XMMATRIX, 
		XMMATRIX,
		ID3D11ShaderResourceView** textureArray, 
		float, 
		float);

private:
	ComPtr<ID3D11Buffer> _reflectionBuffer;
	ComPtr<ID3D11Buffer> _waterBuffer;
};

