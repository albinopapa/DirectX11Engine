////////////////////////////////////////////////////////////////////////////////
// Filename: shadermanagerclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "shadermanagerclass.h"

ShaderManagerClass::ShaderManagerClass()
	:
	_TextureShader( std::make_unique<TextureShaderClass>() ),
	_LightShader( std::make_unique<LightShaderClass>() ),
	_FontShader( std::make_unique<FontShaderClass>() ),
	_ReflectionShader( std::make_unique<ReflectionShaderClass>() ),
	_WaterShader( std::make_unique<WaterShaderClass>() ),
	_RefractionShader( std::make_unique<RefractionShaderClass>() )
{}

void ShaderManagerClass::Initialize( ID3D11Device* device )
{
	// Initialize the texture shader object.
	_TextureShader->Initialize(
		device, 
		"../DirectX11Engine/texture_vs.hlsl", 
		"../DirectX11Engine/texture_ps.hlsl");

	// Initialize the light shader object.
	_LightShader->Initialize(
		device, 
		"../DirectX11Engine/Light_vs.hlsl", 
		"../DirectX11Engine/Light_ps.hlsl");

	// Initialize the bump map shader object.
	_FontShader->Initialize(
		device, 
		"../DirectX11Engine/font.vs", 
		"../DirectX11Engine/font.ps");
	
	// Initialize the bump map shader object.
	_ReflectionShader->Initialize(
		device, 
		"../DirectX11Engine/Reflection_vs.hlsl", 
		"../DirectX11Engine/Reflection_ps.hlsl");
	
	_WaterShader->Initialize(
		device, 
		"../DirectX11Engine/WaterShader_vs.hlsl", 
		"../DirectX11Engine/WaterShader_ps.hlsl");
	
	_RefractionShader->Initialize(
		device, 
		"../DirectX11Engine/Refraction_vs.hlsl", 
		"../DirectX11Engine/Refraction_ps.hlsl");	
}

// @TODO the params at teh end need to be encapsulated
void ShaderManagerClass::Render(
	ID3D11DeviceContext * device, 
	int indexCount, 
	XMMATRIX worldMatrix, 
	XMMATRIX viewMatrix, 
	XMMATRIX projectionMatrix, 
	Material * material, 
	LightClass * light, 
	const std::vector<std::unique_ptr<LightClass>>& lights,
	SceneEffects effects, 
	XMFLOAT3 cameraPos, 
	XMMATRIX reflectionMatrix, 
	ID3D11ShaderResourceView * reflectionTexture, 
	ID3D11ShaderResourceView * refractionTexture, 
	ID3D11ShaderResourceView * normalTexture)
{
	//@TODO: GO THROUGH EVERY SINGLE FUCKING CALL IN GRAPHICS.CPP AND MAKE SURE THEY MATCH WITH THE CALLS BELOW
	// (may have passed in wrong textures in water or reflection shader)
	switch (material->shaderType)
	{
	case EShaderType::ETEXTURE:
		_TextureShader->Render(
			device, 
			indexCount, 
			worldMatrix, 
			viewMatrix, 
			projectionMatrix, 
			material->GetResourceArray()[0]);
		break;

	case EShaderType::ELIGHT_SPECULAR:
		_LightShader->Render(
			device, 
			indexCount, 
			worldMatrix, 
			viewMatrix, 
			projectionMatrix,
			material->GetResourceArray(), 
			light->GetDirection(), 
			light->GetAmbientColor(), 
			light->GetDiffuseColor(), 
			lights, 
			cameraPos, 
			light->GetSpecularColor(), 
			light->GetSpecularPower(), 
			effects.fogStart, 
			effects.fogEnd, 
			effects.clipPlane, 
			material->translation, 
			material->transparency);
		break;

	case EShaderType::EREFLECTION:
		_ReflectionShader->Render(
			device, 
			indexCount, 
			worldMatrix, 
			viewMatrix, 
			projectionMatrix,
			material->GetResourceArray()[0], 
			reflectionTexture, 
			reflectionMatrix);
		break;

	case EShaderType::EREFRACTION:
		_RefractionShader->Render(
			device, 
			indexCount, 
			worldMatrix, 
			viewMatrix, 
			projectionMatrix,
			material->GetResourceArray()[0], 
			light->GetDirection(), 
			light->GetAmbientColor(), 
			light->GetDiffuseColor(), 
			effects.clipPlane);
		break;

	case EShaderType::EWATER:
		_WaterShader->Render(
			device, 
			indexCount, 
			worldMatrix, 
			viewMatrix, 
			projectionMatrix, 
			reflectionMatrix,
			material->GetResourceArray(), 
			material->translation, 
			material->reflectRefractScale);
		break;

	case EShaderType::EFONT:
		_FontShader->Render(
			device, 
			indexCount, 
			worldMatrix, 
			viewMatrix, 
			projectionMatrix,
			material->GetResourceArray()[0], 
			material->pixelColor);
		break;

	default:
		_TextureShader->Render(
			device, 
			indexCount, 
			worldMatrix, 
			viewMatrix, 
			projectionMatrix, 
			material->GetResourceArray()[0]);
		break;
	}
}

FontShaderClass * ShaderManagerClass::GetFontShader()
{
	return _FontShader.get();
}

void ShaderManagerClass::RenderFontShader(
	ID3D11DeviceContext* deviceContext, 
	int indexCount, 
	XMMATRIX worldMatrix, 
	XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, 
	ID3D11ShaderResourceView* texture, 
	XMFLOAT4 pixelColor)
{
	// Render the model using the bump map shader. 
	_FontShader->Render(
		deviceContext, 
		indexCount, 
		worldMatrix, 
		viewMatrix, 
		projectionMatrix, 
		texture, 
		pixelColor);
}