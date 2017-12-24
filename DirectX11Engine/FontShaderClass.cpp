////////////////////////////////////////////////////////////////////////////////
// Filename: fontshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "fontshaderclass.h"

FontShaderClass::FontShaderClass()
{
}


void FontShaderClass::Render(
	ID3D11DeviceContext* deviceContext,
	int indexCount,
	XMMATRIX worldMatrix,
	XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView* texture,
	XMFLOAT4 pixelColor )
{
	// Set the shader parameters that it will use for rendering.
	SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, pixelColor);

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);
}

void FontShaderClass::InitializeShader(
	ID3D11Device* device, 
	char* vsFilename, 
	char* psFilename )
{
	// Compile the vertex shader code.
	CompileShaders( device, vsFilename, psFilename, "FontVertexShader", "FontPixelShader" );

	// Create the vertex input layout description.
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
	const auto numElements = polygonLayout.size();

	// Create the vertex input layout.
	ThrowIf( device->CreateInputLayout( 
		polygonLayout.data(), 
		numElements, 
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), 
		&_layout ) );

	_matrixBuffer = MakeConstantBuffer<MatrixBufferType>( device );

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

	// Setup the description of the dynamic pixel constant buffer that is in the pixel shader.
	_pixelBuffer = MakeConstantBuffer<PixelBufferType>( device );
}

void FontShaderClass::SetShaderParameters(
	ID3D11DeviceContext* deviceContext, 
	XMMATRIX worldMatrix, 
	XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, 
	ID3D11ShaderResourceView* texture, 
	XMFLOAT4 pixelColor )
{
	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources( 0, 1, &texture );

	unsigned int bufferNumber = 0u;
	ShaderClass::SetBaseParameters(
		deviceContext,
		worldMatrix,
		viewMatrix,
		projectionMatrix,
		bufferNumber );

	const PixelBufferType pixelBuffer{ pixelColor };
	MapBuffer( pixelBuffer, _pixelBuffer.Get(), deviceContext );

	// Set the position of the pixel constant buffer in the pixel shader.
	bufferNumber = 0;

	// Now set the pixel constant buffer in the pixel shader with the updated value.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &_pixelBuffer);
}