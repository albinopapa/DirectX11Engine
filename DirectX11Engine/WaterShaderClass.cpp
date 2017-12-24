#include "WaterShaderClass.h"



WaterShaderClass::WaterShaderClass()
{}

void WaterShaderClass::Render(
	ID3D11DeviceContext * deviceContext, 
	int indexCount, 
	XMMATRIX worldMatrix, 
	XMMATRIX viewMatrix, 
	XMMATRIX projectionMatrix,
	XMMATRIX reflectionMatrix, 
	ID3D11ShaderResourceView** textureArray, 
	float waterTranslation, 
	float reflectRefractScale)
{
	// Set the shader parameters that it will use for rendering.
	SetShaderParameters(
		deviceContext,
		worldMatrix,
		viewMatrix,
		projectionMatrix,
		reflectionMatrix,
		textureArray,
		waterTranslation,
		reflectRefractScale );

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);
}

void WaterShaderClass::InitializeShader( ID3D11Device* device, char* vsFilename, char* psFilename)
{
	CompileShaders( device, vsFilename, psFilename, "WaterVertexShader", "WaterPixelShader" );

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	std::vector<D3D11_INPUT_ELEMENT_DESC> polygonLayout( 2 );
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

	// Get a count of the elements in the layout.
	const unsigned int numElements = polygonLayout.size();

	// Create the vertex input layout.
	ThrowHResultIf( device->CreateInputLayout(
		polygonLayout.data(),
		numElements,
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		&_layout ) );

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
	ThrowHResultIf( device->CreateSamplerState( &samplerDesc, &_sampleState ) );

	_matrixBuffer = MakeConstantBuffer<MatrixBufferType>( device );
	_reflectionBuffer = MakeConstantBuffer<ReflectionBufferType>( device );
	_waterBuffer = MakeConstantBuffer<WaterBufferType>( device );
}

void WaterShaderClass::SetShaderParameters(
	ID3D11DeviceContext* deviceContext, 
	XMMATRIX worldMatrix, 
	XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, 
	XMMATRIX reflectionMatrix,
	ID3D11ShaderResourceView** textureArray,
	float waterTranslation, 
	float reflectRefractScale)
{
	{
		unsigned int bufferNumber = 0;

		SetBaseParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, bufferNumber);

		ReflectionBufferType reflectBuffer{ XMMatrixTranspose( reflectionMatrix ) };
		MapBuffer( reflectBuffer, _reflectionBuffer.Get(), deviceContext );

		// Finally set the reflection constant buffer in the vertex shader with the updated values.
		deviceContext->VSSetConstantBuffers( ++bufferNumber, 1, _reflectionBuffer.GetAddressOf() );
	}

	{
		WaterBufferType waterBuffer{ waterTranslation, reflectRefractScale };
		MapBuffer( waterBuffer, _waterBuffer.Get(), deviceContext );

		// Set the position of the water constant buffer in the pixel shader.
		unsigned int bufferNumber = 0;

		// Finally set the water constant buffer in the pixel shader with the updated values.
		deviceContext->PSSetConstantBuffers( bufferNumber, 1, _waterBuffer.GetAddressOf() );
	}

	deviceContext->PSSetShaderResources( 0, 3, textureArray );
}
