////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "lightclass.h"

LightClass::LightClass()
{
}

void LightClass::Initialize( 
	const XMFLOAT4& Diffuse, 
	const XMFLOAT4& Ambient, 
	const XMFLOAT4& Specular,
	const XMFLOAT4& Position, 
	const XMFLOAT3& Direction, 
	float SpecularPower )
{
	_ambientColor = Ambient;
	_diffuseColor = Diffuse;
	_specularColor = Specular;
	_specularPower = SpecularPower;
	_position = Position;
	_direction = Direction;
}

void LightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
	_ambientColor = XMFLOAT4(red, green, blue, alpha);
}

void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	_diffuseColor = XMFLOAT4(red, green, blue, alpha);
}

void LightClass::SetPosition(float x, float y, float z)
{
	_position = XMFLOAT4(x, y, z, 1.f);
}


void LightClass::SetDirection(float x, float y, float z)
{
	_direction = XMFLOAT3(x, y, z);
	return;
}

void LightClass::SetSpecularColor(float red, float green, float blue, float alpha)
{
	_specularColor = XMFLOAT4(red, green, blue, alpha);
	return;
}

void LightClass::SetSpecularPower(float power)
{
	_specularPower = power;
	return;
}

XMFLOAT4 LightClass::GetAmbientColor() const 
{
	return _ambientColor;
}

XMFLOAT4 LightClass::GetDiffuseColor() const
{
	return _diffuseColor;
}

XMFLOAT4 LightClass::GetPosition() const
{
	return _position;
}

XMFLOAT3 LightClass::GetDirection() const
{
	return _direction;
}

XMFLOAT4 LightClass::GetSpecularColor() const
{
	return _specularColor;
}

float LightClass::GetSpecularPower() const
{
	return _specularPower;
}