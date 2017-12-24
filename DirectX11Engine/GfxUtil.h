////////////////////////////////////////////
// Filename: GfxUtil.h
// Contains useful functions and operator overloads for working with Graphics
///////////////////////////////////////////////
#pragma once

#include "GlobalIncludes.h"
#include "Texture.h"

namespace GfxUtil
{
	enum EShaderType
	{
		ETEXTURE = 0,
		ELIGHT_SPECULAR = 1,
		EREFLECTION = 2,
		EREFRACTION = 3,
		EWATER = 4,
		EFONT = 5,
		ESIZE = 6
	};

	struct SceneEffects
	{
		float fogStart = 0.f;
		float fogEnd = 0.f;
		XMFLOAT4 clipPlane = XMFLOAT4(0.0f, 0.f, 0.0f, 0.0f);
	};

	class Material
	{
	public:
		Material();

		bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, EShaderType inShaderType, vector<char*> fileNames);

		TextureClass* GetTextureObject();
		ID3D11ShaderResourceView** GetResourceArray();
		
		void Animate(float deltaTime = 1.f);

		unique_ptr<TextureClass> _TextureArray;
		EShaderType shaderType;
		int texArraySize;
		XMFLOAT4 pixelColor = XMFLOAT4(1,1,1,1);

		// Effects @TODO: encapsulate into different struct
		bool bAnimated = false;
		float transparency = 0.f;
		float translation = 0.f;
		float reflectRefractScale = 0.01f;
		float waterHeight = 2.75f;
	};
}