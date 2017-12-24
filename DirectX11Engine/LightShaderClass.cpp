////////////////////////////////////////////////////////////////////////////////
// Filename: lightshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////

#include "lightshaderclass.h"

LightShaderClass::LightShaderClass()
{}

void LightShaderClass::Render(
	ID3D11DeviceContext* deviceContext, 
	int indexCount, 
	XMMATRIX worldMatrix, 
	XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, 
	ID3D11ShaderResourceView** textureArray, 
	XMFLOAT3 lightDirection, 
	XMFLOAT4 ambientColor, 
	XMFLOAT4 diffuseColor, 
	const std::vector<std::unique_ptr<LightClass>>& lights,
	XMFLOAT3 cameraPosition, 
	XMFLOAT4 specularColor, 
	float specularPower, 
	float fogStart, 
	float fogEnd, 
	XMFLOAT4 clipPlane, 
	float translation, 
	float transparency )
{
	// Set the shader parameters that it will use for rendering.
	SetShaderParameters(
		deviceContext, 
		worldMatrix, 
		viewMatrix, 
		projectionMatrix, 
		textureArray, 
		lightDirection, 
		ambientColor, 
		diffuseColor, 
		lights, 
		cameraPosition, 
		specularColor, 
		specularPower, 
		fogStart, 
		fogEnd, 
		clipPlane, 
		translation, 
		transparency );
	
	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);
}

void LightShaderClass::InitializeShader(
	ID3D11Device* device, 
	HWND hwnd, 
	char* vsFilename, 
	char* psFilename )
{
	CompileShaders( device, vsFilename, psFilename, "LightVertexShader", "LightPixelShader" );

	// VERTEX SHADER LAYOUT DESCRIPTION SETUP //
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.	
	std::vector<D3D11_INPUT_ELEMENT_DESC> polygonLayout( 5 );
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "TANGENT";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	polygonLayout[4].SemanticName = "BINORMAL";
	polygonLayout[4].SemanticIndex = 0;
	polygonLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[4].InputSlot = 0;
	polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[4].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	const unsigned int numElements = polygonLayout.size();

	// Create the vertex input layout.
	ThrowIf( device->CreateInputLayout( polygonLayout.data(), numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &_layout ) );

	// Create a texture sampler state description.
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	ThrowIf( device->CreateSamplerState( &samplerDesc, &_sampleState ) );

	_buffers.emplace_back( MakeConstantBuffer<MatrixBufferType>( device ) );
	_buffers.emplace_back( MakeConstantBuffer<CameraBufferType>( device ) );
	_buffers.emplace_back( MakeConstantBuffer<LightBufferType>( device ) );
	_buffers.emplace_back( MakeConstantBuffer<LightColorBufferType>( device ) );
	_buffers.emplace_back( MakeConstantBuffer<LightPositionBufferType>( device ) );
	_buffers.emplace_back( MakeConstantBuffer<FogBufferType>( device ) );
	_buffers.emplace_back( MakeConstantBuffer<ClipPlaneBufferType>( device ) );
	_buffers.emplace_back( MakeConstantBuffer<TranslateBufferType>( device ) );
	_buffers.emplace_back( MakeConstantBuffer<TransparentBufferType>( device ) );
}

void LightShaderClass::SetShaderParameters(
	ID3D11DeviceContext* deviceContext, 
	XMMATRIX worldMatrix, 
	XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, 
	ID3D11ShaderResourceView** textureArray, 
	XMFLOAT3 lightDirection, 
	XMFLOAT4 ambientColor, 
	XMFLOAT4 diffuseColor,
	const std::vector<std::unique_ptr<LightClass>>& lights,
	XMFLOAT3 cameraPosition, 
	XMFLOAT4 specularColor, 
	float specularPower, 
	float fogStart, 
	float fogEnd, 
	XMFLOAT4 clipPlane, 
	float translation, 
	float transparency)
{
	auto MapAndSetVSBuffer = [ deviceContext ]( const auto InData, Microsoft::WRL::ComPtr<ID3D11Buffer>& Buffer, const unsigned int BufferNumber )
	{
		MapBuffer( InData, Buffer.Get(), deviceContext );

		// Now set the constant buffer in the vertex shader with the updated values
		deviceContext->VSSetConstantBuffers( BufferNumber, 1, Buffer.GetAddressOf() );
	};
	auto MapAndSetPSBuffer = [ deviceContext ]( const auto InData, Microsoft::WRL::ComPtr<ID3D11Buffer>& Buffer, const unsigned int BufferNumber )
	{
		MapBuffer( InData, Buffer.Get(), deviceContext );

		// Now set the constant buffer in the vertex shader with the updated values
		deviceContext->PSSetConstantBuffers( BufferNumber, 1, Buffer.GetAddressOf() );
	};

	// Set VertexShader constant buffers
	{
		unsigned int bufferNumber = 0;
		ShaderClass::SetBaseParameters( deviceContext, worldMatrix, viewMatrix, projectionMatrix, bufferNumber );

		CameraBufferType cameraBuffer{ cameraPosition };
		MapAndSetVSBuffer( cameraBuffer, _cameraBuffer, bufferNumber++ );

		LightPositionBufferType lightPosBuffer{
			lights[ 0 ]->GetPosition(),
			lights[ 1 ]->GetPosition(),
			lights[ 2 ]->GetPosition(),
			lights[ 3 ]->GetPosition()
		};
		MapAndSetVSBuffer( lightPosBuffer, _lightPositionBuffer, bufferNumber++ );

		ClipPlaneBufferType clipPlaneBuffer{ clipPlane };
		MapAndSetVSBuffer( clipPlaneBuffer, _clipPlaneBuffer, bufferNumber++ );

		FogBufferType fogBuffer{ fogStart,fogEnd };
		MapAndSetVSBuffer( fogBuffer, _fogBuffer, bufferNumber++ );
	}

	// Set PixelShader constant buffers
	{
		unsigned int bufferNumber = 0;
		LightBufferType lightBuffer{
			ambientColor, diffuseColor, lightDirection, specularPower, specularColor
		};
		MapAndSetPSBuffer( lightBuffer, _lightBuffer, bufferNumber++ );

		LightColorBufferType lightColorBuffer{
			lights[ 0 ]->GetDiffuseColor(),
			lights[ 1 ]->GetDiffuseColor(),
			lights[ 2 ]->GetDiffuseColor(),
			lights[ 3 ]->GetDiffuseColor()
		};
		MapAndSetPSBuffer( lightColorBuffer, _lightColorBuffer, bufferNumber++ );

		TranslateBufferType translateBuffer{ translation };
		MapAndSetPSBuffer( translateBuffer, _translateBuffer, bufferNumber++ );

		TransparentBufferType transparentBuffer{ transparency };
		MapAndSetPSBuffer( transparentBuffer, _transparentBuffer, bufferNumber );
	}

	/////////////////////// SET TEXTURE RESOURCES //////////////////////
	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources( 0, 6, textureArray ); // sextuple tex with lightmap
}