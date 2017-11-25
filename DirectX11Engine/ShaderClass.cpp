////////////////////////////////////////////////////////////////////////////////
// Filename: ShaderClass.cpp
////////////////////////////////////////////////////////////////////////////////
#pragma comment ( lib, "d3dcompiler.lib" )
#pragma comment ( lib, "d3d11.lib" )

#include "ShaderClass.h"

ShaderClass::ShaderClass()
	:
	_vertexShader(0),
	_pixelShader(0),
	_layout(0),
	_matrixBuffer(0),
	_sampleState(0)
{}

ShaderClass::ShaderClass(const ShaderClass& other)
{
}

ShaderClass::~ShaderClass()
{
}

// pass in the name of the HLSL shader files
//bool ShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
//{
//	bool result;
//
//	// Initialize the vertex and pixel shaders.
//	result = InitializeShader(
//		device,
//		hwnd,
//		L"../DirectX11Engine/texture_vs.hlsl",
//		L"../DirectX11Engine/texture_ps.hlsl");
//	if (!result)
//	{
//		return false;
//	}
//
//	return true;
//}

void ShaderClass::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}

//bool ShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
//	XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
//{
//	bool result;
//
//	// Set the shader parameters that it will use for rendering.
//	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture);
//	if (!result)
//	{
//		return false;
//	}
//
//	// Now render the prepared buffers with the shader.
//	RenderShader(deviceContext, indexCount);
//
//	return true;
//}

// The layout will need the match the VertexType in the modelclass.h file as well as the one defined in the color.vs file.
//bool ColorShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, std::wstring vsFilename, std::wstring psFilename)
//bool ShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
//{
//	HRESULT result;
//	ID3D10Blob* errorMessage;
//	ID3D10Blob* vertexShaderBuffer;
//	ID3D10Blob* pixelShaderBuffer;
//	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
//	unsigned int numElements;
//	D3D11_BUFFER_DESC matrixBufferDesc;
//	D3D11_SAMPLER_DESC samplerDesc;
//
//	// Initialize the pointers this function will use to null.
//	errorMessage = 0;
//	vertexShaderBuffer = 0;
//	pixelShaderBuffer = 0;
//
//	// Compile the vertex shader code.
//	result = D3DCompileFromFile(vsFilename, NULL, NULL, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
//		&vertexShaderBuffer, &errorMessage);
//	if (FAILED(result))
//	{
//		// If the shader failed to compile it should have writen something to the error message.
//		if (errorMessage)
//		{
//			//wchar_t cName = vsFilename.c_str;
//			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
//		}
//		// If there was  nothing in the error message then it simply could not find the shader file itself.
//		else
//		{
//			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
//		}
//
//		return false;
//	}
//
//	// Compile the pixel shader code.
//	result = D3DCompileFromFile(psFilename, NULL, NULL, "TexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
//		&pixelShaderBuffer, &errorMessage);
//	if (FAILED(result))
//	{
//		// If the shader failed to compile it should have writen something to the error message.
//		if (errorMessage)
//		{
//			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
//		}
//		// If there was nothing in the error message then it simply could not find the file itself.
//		else
//		{
//			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
//		}
//
//		return false;
//	}
//
//	// Once the vertex shader and pixel shader code has successfully compiled into buffers we then use those buffers to create the shader objects themselves.
//	// Create the vertex shader from the buffer. 
//	// We will use these pointers to interface with the vertex and pixel shader from this point forward.
//	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader);
//	if (FAILED(result))
//	{
//		return false;
//	}
//
//	// Create the pixel shader from the buffer.
//	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader);
//	if (FAILED(result))
//	{
//		return false;
//	}
//
//	// Create the vertex input layout description.
//	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
//	polygonLayout[0].SemanticName = "POSITION";
//	polygonLayout[0].SemanticIndex = 0;
//	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
//	polygonLayout[0].InputSlot = 0;
//	polygonLayout[0].AlignedByteOffset = 0;
//	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
//	polygonLayout[0].InstanceDataStepRate = 0;
//
//	polygonLayout[1].SemanticName = "TEXCOORD";
//	polygonLayout[1].SemanticIndex = 0;
//	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
//	polygonLayout[1].InputSlot = 0;
//	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
//	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
//	polygonLayout[1].InstanceDataStepRate = 0;
//
//	// Get a count of the elements in the layout.
//	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
//
//	// Create the vertex input layout.
//	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
//		vertexShaderBuffer->GetBufferSize(), &_layout);
//
//	if (FAILED(result))
//	{
//		return false;
//	}
//
//	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
//	vertexShaderBuffer->Release();
//	vertexShaderBuffer = 0;
//
//	pixelShaderBuffer->Release();
//	pixelShaderBuffer = 0;
//
//	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
//	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC; // we will be updating it each frame so use dynamic
//	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
//	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	matrixBufferDesc.MiscFlags = 0;
//	matrixBufferDesc.StructureByteStride = 0;
//
//	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
//	result = device->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer);
//	if (FAILED(result))
//	{
//		return false;
//	}
//
//	/////////////////////////////////////
//	// Setup Sampler State Description //
//	/////////////////////////////////////
//	// Create a texture sampler state description.
//	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.MipLODBias = 0.0f;
//	samplerDesc.MaxAnisotropy = 1;
//	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
//	samplerDesc.BorderColor[0] = 0;
//	samplerDesc.BorderColor[1] = 0;
//	samplerDesc.BorderColor[2] = 0;
//	samplerDesc.BorderColor[3] = 0;
//	samplerDesc.MinLOD = 0;
//	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
//
//	// Create the texture sampler state.
//	result = device->CreateSamplerState(&samplerDesc, &_sampleState);
//	if (FAILED(result))
//	{
//		return false;
//	}
//
//	return true;
//}

//void ShaderClass::ShutdownShader()
//{
//	// Release the sampler state.
//	if (_sampleState)
//	{
//		_sampleState->Release();
//		_sampleState = 0;
//	}
//
//	// Release the matrix constant buffer.
//	if (_matrixBuffer)
//	{
//		_matrixBuffer->Release();
//		_matrixBuffer = 0;
//	}
//
//	// Release the layout.
//	if (_layout)
//	{
//		_layout->Release();
//		_layout = 0;
//	}
//
//	// Release the pixel shader.
//	if (_pixelShader)
//	{
//		_pixelShader->Release();
//		_pixelShader = 0;
//	}
//
//	// Release the vertex shader.
//	if (_vertexShader)
//	{
//		_vertexShader->Release();
//		_vertexShader = 0;
//	}
//
//	return;
//}

void ShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	ofstream fout;

	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

bool ShaderClass::SetShaderParameters(D3D11_MAPPED_SUBRESOURCE* const mappedResource, ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, /*ID3D11ShaderResourceView* texture*/unsigned int& bufferNumber)
{
	HRESULT result;
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	//unsigned int bufferNumber;

	// Transpose the matrices to prepare them for the shader.
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, /*&*/mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)(*mappedResource).pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer.
	deviceContext->Unmap(_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	/////////////////////////////////////////////////////////////
	/////////////////////// VS BUFFERS //////////////////////////
	/////////////////////////////////////////////////////////////

	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);

	/////////////////////////////////////////////////////////////
	/////////////////////// PS BUFFERS //////////////////////////
	/////////////////////////////////////////////////////////////
	// Set shader texture resource in the pixel shader.
	//deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}

void ShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(_layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(_vertexShader, NULL, 0);
	deviceContext->PSSetShader(_pixelShader, NULL, 0);

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &_sampleState);

	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}

