#pragma once

#include "GlobalIncludes.h"

class ShaderCompiler
{
public:
	ShaderCompiler() = default;

	ComPtr<ID3DBlob> CompileShader( 
		std::string Filename,
		std::string EntryPoint,
		std::string TargetShaderVersion, 
		ID3D11Device* pDevice )const;

private:
	void OutputErrorMessage( const std::string& Filename, ComPtr<ID3DBlob>& ErrorMessage )const;

};

