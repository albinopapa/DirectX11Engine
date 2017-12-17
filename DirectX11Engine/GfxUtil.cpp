#include "GfxUtil.h"

GfxUtil::Material::Material()
	{
		shaderType = ETEXTURE;
		texArraySize = 1;
	}

bool GfxUtil::Material::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, EShaderType inShaderType, vector<char*> fileNames)
{
	texArraySize = fileNames.size();

	shaderType = inShaderType;

	_TextureArray.reset(new TextureClass);
	if (!_TextureArray)
	{
		return false;
	}

	bool result = _TextureArray->InitializeArray(device, deviceContext, fileNames);
	if (!result)
	{
		return false;
	}

	return true;
}

ID3D11ShaderResourceView** GfxUtil::Material::GetResourceArray()
{
	if (_TextureArray)
	{
		return _TextureArray->GetTextureArray();
	}
}