////////////////////////////////////////////////////////////////////////////////
// Filename: fontshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _FONTSHADERCLASS_H_
#define _FONTSHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>
using namespace DirectX;
using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Class name: FontShaderClass
////////////////////////////////////////////////////////////////////////////////
class FontShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct PixelBufferType
	{
		XMFLOAT4 pixelColor;
	};

public:
	FontShaderClass();
	FontShaderClass(const FontShaderClass&);
	~FontShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT4 pixelColor);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT4);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_pixelBuffer;
};

#endif









//////////////////////////////////////////////////////////////////////////////////
//// Filename: fontshaderclass.h
//////////////////////////////////////////////////////////////////////////////////
//#pragma once
//
////////////////
//// INCLUDES //
////////////////
//#include <d3d11.h>
////#include <d3dx10math.h>
//#include <DirectXMath.h>
//#include "d3dcompiler.h"
//#include "lightshaderclass.h"
////#include <d3dx11async.h>
//#include <fstream>
//using namespace std;
//using namespace DirectX;
//
//////////////////////////////////////////////////////////////////////////////////
//// Class name: FontShaderClass
//////////////////////////////////////////////////////////////////////////////////
//class FontShaderClass
//{
//private:
//	struct ConstantBufferType
//	{
//		XMMATRIX world;
//		XMMATRIX view;
//		XMMATRIX projection;
//	};
//
//	struct PixelBufferType
//	{
//		XMFLOAT4 pixelColor;
//	};
//
//public:
//	FontShaderClass();
//	FontShaderClass(const FontShaderClass&);
//	~FontShaderClass();
//
//	bool Initialize(ID3D11Device*, HWND);
//	void Shutdown();
//	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT4);
//
//private:
//	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
//	void ShutdownShader();
//	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
//
//	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, XMFLOAT4);
//	void RenderShader(ID3D11DeviceContext*, int);
//
//private:
//	ID3D11VertexShader* _vertexShader;
//	ID3D11PixelShader* _pixelShader;
//	ID3D11InputLayout* _layout;
//	ID3D11Buffer* _constantBuffer;
//	ID3D11SamplerState* _sampleState;
//	ID3D11Buffer* _pixelBuffer;
//};