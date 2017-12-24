////////////////////////////////////////////////////////////////////////////////
// Filename: TextureShaderClass.cpp
////////////////////////////////////////////////////////////////////////////////

#include "TextureShaderClass.h"

TextureShaderClass::TextureShaderClass()
{}

void TextureShaderClass::Render(
	ID3D11DeviceContext* deviceContext, 
	int indexCount, 
	XMMATRIX worldMatrix, 
	XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, 
	ID3D11ShaderResourceView* texture)
{
	// Set the shader parameters that it will use for rendering.
	SetShaderParameters(
		deviceContext, 
		worldMatrix, 
		viewMatrix, 
		projectionMatrix, 
		texture);

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);
}

// The layout will need the match the VertexType in the modelclass.h file as well as the one defined in the color.vs file.
void TextureShaderClass::InitializeShader( ID3D11Device* device, char* vsFilename, char* psFilename )
{
	CompileShaders( device, vsFilename, psFilename, "TextureVertexShader", "TexturePixelShader" );

	// Create the vertex input layout description. This setup needs to match the VertexType stucture in the ModelClass and in the shader.
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
	ThrowIf( device->CreateInputLayout(
		polygonLayout.data(),
		numElements,
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		&_layout ) );

	/////////////////////////////////////
	// Setup Sampler State Description //
	/////////////////////////////////////
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

	_matrixBuffer = MakeConstantBuffer<MatrixBufferType>( device );
}

void TextureShaderClass::SetShaderParameters(
	ID3D11DeviceContext* deviceContext, 
	XMMATRIX worldMatrix, 
	XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, 
	ID3D11ShaderResourceView* texture)
{
	unsigned int bufferNumber = 0;

	SetBaseParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, bufferNumber);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
}