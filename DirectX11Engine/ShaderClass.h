#pragma once

////////////////////////////////////////////////////////////////////////////////
// Filename: ShaderClass.h
////////////////////////////////////////////////////////////////////////////////

#pragma comment ( lib, "d3d11.lib" )

//////////////
// INCLUDES //
//////////////
#include "GlobalIncludes.h"
#include "GfxUtil.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: ShaderClass
// Description: Used to invoke the HLSL shaders for drawing the 3D models that are on the GPU.
////////////////////////////////////////////////////////////////////////////////
class ShaderClass
{
protected:
	// cBuffer type that will be used with the vertex shader.
	// This typedef must be exactly the same as the one in the vertex shader as the model data needs to match the typedefs in the shader for proper rendering.
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	// record the location of each vertex that is in the vertex buffer.
	// The GPU then uses the index buffer to quickly find specific vertices in the vertex buffer.
public:
	ShaderClass() = default;
	ShaderClass( const ShaderClass& ) = delete;
	virtual~ShaderClass() = default;

	void Initialize( ID3D11Device* device, char* vsFilename, char* psFilename );

protected:
	void RenderShader( ID3D11DeviceContext* deviceContext, int indexCount );
	void CompileShaders(
		ID3D11Device * device,
		char* vsFilename,
		char* psFilename,
		char* vsDesc,
		char* psDesc );
	void SetBaseParameters(
		ID3D11DeviceContext*,
		XMMATRIX worldMatrix,
		XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix,
		unsigned int& bufferNumber );
	virtual void InitializeShader(
		ID3D11Device* device,
		char* vsFileName,
		char* psFileName ) = 0;

protected:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> _vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> _pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> _layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> _matrixBuffer;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> _sampleState;

	// Temp holders //
	Microsoft::WRL::ComPtr<ID3D10Blob> vertexShaderBuffer, pixelShaderBuffer;
};

template<class BufferType>
void MapBuffer( const BufferType& InData, ID3D11Buffer* pBuffer, ID3D11DeviceContext* deviceContext )
{
	D3D11_MAPPED_SUBRESOURCE mappedResource{};

	ThrowIf( deviceContext->Map( pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource ) );
	memcpy( mappedResource.pData, &InData, sizeof( BufferType ) );
	deviceContext->Unmap( pBuffer, 0 );
}

template<class BufferType>
Microsoft::WRL::ComPtr<ID3D11Buffer> MakeConstantBuffer( ID3D11Device* device )
{
	static_assert( sizeof( BufferType ) % 16 == 0, "Constant buffer isn't a multiple of 16." );

	D3D11_BUFFER_DESC desc{};
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof( BufferType );
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	HRESULT result = device->CreateBuffer( &desc, nullptr, &buffer );
	CHECK( SUCCEEDED( result ), "Failed to create constant buffer." );

	return buffer;
}