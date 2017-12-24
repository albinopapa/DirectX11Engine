////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include "Actor.h"
#include <directxmath.h> 
using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: LightClass
////////////////////////////////////////////////////////////////////////////////
class LightClass : public Actor
{
public:
	LightClass();

	void Initialize(
		const XMFLOAT4& Diffuse,
		const XMFLOAT4& Ambient,
		const XMFLOAT4& Specular,
		const XMFLOAT4& Position,
		const XMFLOAT3& Direction,
		float SpecularPower );
	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetPosition(float x, float y, float z);
	void SetDirection(float, float, float);
	void SetSpecularColor(float, float, float, float);
	void SetSpecularPower(float);

	XMFLOAT4 GetAmbientColor() const;
	XMFLOAT4 GetDiffuseColor() const;
	XMFLOAT4 GetPosition() const;
	XMFLOAT3 GetDirection() const;
	XMFLOAT4 GetSpecularColor() const;
	float GetSpecularPower() const;

private:
	XMFLOAT4 _ambientColor;
	XMFLOAT4 _diffuseColor;
	XMFLOAT3 _direction;
	XMFLOAT4 _position;
	XMFLOAT4 _specularColor;
	float _specularPower;
};