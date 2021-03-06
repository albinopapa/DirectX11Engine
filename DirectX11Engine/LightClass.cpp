////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "lightclass.h"

LightClass::LightClass()
{
}

LightClass::LightClass(const LightClass& other)
{
}

LightClass::~LightClass()
{
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
}

void LightClass::SetSpecularColor(float red, float green, float blue, float alpha)
{
	_specularColor = XMFLOAT4(red, green, blue, alpha);
}

void LightClass::SetSpecularPower(float power)
{
	_specularPower = power;
}

void LightClass::SetLookAt(float x, float y, float z)
{
	_lookAt.x = x;
	_lookAt.y = y;
	_lookAt.z = z;
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

void LightClass::GenerateViewMatrix()
{
	XMFLOAT3 up;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Create the view matrix from the three vectors.
	XMFLOAT3 pos(_position.x, _position.y, _position.z);
	_viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&pos), XMLoadFloat3(&_lookAt), XMLoadFloat3(&up));
}

void LightClass::GenerateProjectionMatrix(float screenDepth, float screenNear)
{
	float fieldOfView = (float)XM_PI / 2.0f;
	float screenAspect = 1.0f;

	// Create the projection matrix for the light.
	_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
}

XMMATRIX LightClass::GetViewMatrix() const
{
	return _viewMatrix;
}

XMMATRIX LightClass::GetProjectionMatrix() const
{
	return _projectionMatrix;
}
