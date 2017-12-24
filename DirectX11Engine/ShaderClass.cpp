////////////////////////////////////////////////////////////////////////////////
// Filename: ShaderClass.cpp
////////////////////////////////////////////////////////////////////////////////
//#pragma comment ( lib, "d3dcompiler.lib" )
//#pragma comment ( lib, "d3d11.lib" )

#include "ShaderClass.h"
#include "ShaderCompiler.h"

ShaderClass::ShaderClass()
{}

void ShaderClass::Initialize(ID3D11Device * device, char * vsFilename, char * psFilename)
{
	// Initialize the vertex and pixel shaders.
	InitializeShader( device, vsFilename, psFilename );
}

void ShaderClass::CompileShaders(
	ID3D11Device * device, 
	char* vsFilename, 
	char* psFilename, 
	char* vsDesc, 
	char* psDesc )
{
	ShaderCompiler compiler;

	// Compile the vertex shader code. @TODO!!!!!! WATCH OUT FOR NULL POINTERS HERE!!!!!
	vertexShaderBuffer = compiler.CompileShader( vsFilename, vsDesc, "vs_5_0", device);

	// Create the vertex shader from the buffer.
	ThrowHResultIf( device->CreateVertexShader(
		vertexShaderBuffer->GetBufferPointer(), 
		vertexShaderBuffer->GetBufferSize(), 
		nullptr,
		&_vertexShader ) );

	// Compile the pixel shader code.
	pixelShaderBuffer = compiler.CompileShader( psFilename, psDesc, "ps_5_0", device );

	// Create the vertex shader from the buffer.
	ThrowHResultIf( device->CreatePixelShader(
		pixelShaderBuffer->GetBufferPointer(), 
		pixelShaderBuffer->GetBufferSize(), 
		nullptr,
		&_pixelShader ) );
}

void ShaderClass::SetBaseParameters(
	ID3D11DeviceContext* deviceContext, 
	XMMATRIX worldMatrix, 
	XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, 
	unsigned int& bufferNumber)
{
	// Transpose the matrices to prepare them for the shader.
	const MatrixBufferType matBuffer{
		XMMatrixTranspose( worldMatrix ),
		XMMatrixTranspose( viewMatrix ),
		XMMatrixTranspose( projectionMatrix )
	};

	MapBuffer( matBuffer, _matrixBuffer.Get(), deviceContext );

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);
}

void ShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout( _layout.Get() );

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader( _pixelShader.Get(), nullptr, 0);

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers( 0, 1, _sampleState.GetAddressOf() );

	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);
}

