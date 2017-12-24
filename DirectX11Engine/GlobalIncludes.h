#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define _XM_NO_INTRINSICS_ 

#include <directxmath.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace DirectX;
using Microsoft::WRL::ComPtr;
using namespace std;

class RuntimeException
{
public:
	RuntimeException() = default;
	RuntimeException( std::string Message, std::string Filename, const size_t LineNumber )
	{
		std::stringstream ss;

		ss << Message
			<< "\nLine number: "
			<< LineNumber
			<< "\nFilename: "
			<< CaptureFilename( Filename );
		error = ss.str();
	}
		
	std::string GetError()const
	{
		return error;
	}
	std::wstring GetWindowsError()const
	{
		return std::wstring( error.begin(), error.end() );
	}

protected:
	std::string CaptureFilename( const std::string& Filename )
	{
		const auto beg = Filename.find_last_of( '\\' ) + 1;
		return Filename.substr( beg );
	}

protected:
	std::string error;
};
class CompileShaderException :public RuntimeException
{
public:
	CompileShaderException( std::string ShaderFilename, std::string ShaderErrorMsg, std::string OutputLog )
		:
		RuntimeException( "Error compiling " + ShaderFilename + ".  Error logged to " + OutputLog +
			"\nShader error message: \n" + ShaderErrorMsg )
	{}
};
class MissingShaderException :public RuntimeException
{
public:
	MissingShaderException( std::string ShaderFilename )
		:
		RuntimeException( "Missing Shader File " + ShaderFilename )
	{}
};
class HResultException : public RuntimeException
{
public:
	HResultException( HRESULT HResult, std::string Filename, const size_t LineNumber )
	{
		std::stringstream ss;

		ss << "HRESULT "
			<< std::showbase
			<< std::hex
			<< HResult
			<< std::dec
			<< " was returned.\nLine number: "
			<< LineNumber
			<< "\nFilaname: "
			<< CaptureFilename( Filename );
		error = ss.str();
	}
};

// Don't end the macros with ; this way it will force you to put them in code
#define ThrowRuntime( msg ) throw RuntimeException( (msg), __FILE__, __LINE__)
#define ThrowHResultIf( hr ) if( FAILED( (hr) ) ) throw HResultException( (hr), __FILE__, __LINE__)

template<class InString, class OutString>
auto ConvertTo( const InString text )->OutString
{	
	return OutString( text.begin(), text.end() );
}
