
///////////////////////////////////////////////////////////////////////////////
// Filename: fontclass.cpp
///////////////////////////////////////////////////////////////////////////////
#include "fontclass.h"

FontClass::FontClass()
	:
_Font(0),
_Texture(0)
{
}

FontClass::FontClass(const FontClass& other)
{
}

FontClass::~FontClass()
{
}

bool FontClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fontFilename, char* textureFilename,
	float fontHeight, int spaceSize)
{
	bool result;


	// Store the height of the font.
	_fontHeight = fontHeight;

	// Store the size of spaces in pixel size.
	_spaceSize = spaceSize;

	// Load in the text file containing the font data.
	result = LoadFontData(fontFilename);
	if (!result)
	{
		return false;
	}

	// Load the texture that has the font characters on it.
	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::Shutdown()
{
	// Release the font texture.
	ReleaseTexture();

	// Release the font data.
	ReleaseFontData();

	return;
}

bool FontClass::LoadFontData(char* filename)
{
	ifstream fin;
	int i;
	char temp;

	// Create the font spacing buffer.
	_Font = new FontType[95];
	if (!_Font)
	{
		return false;
	}

	// Read in the font size and spacing between chars.
	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	// Read in the 95 used ascii characters for text.
	for (i = 0; i<95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> _Font[i].left;
		fin >> _Font[i].right;
		fin >> _Font[i].size;
	}

	// Close the file.
	fin.close();

	return true;
}


void FontClass::ReleaseFontData()
{
	// Release the font data array.
	if (_Font)
	{
		delete[] _Font;
		_Font = 0;
	}

	return;
}


bool FontClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;


	// Create the texture object.
	_Texture = new TextureClass;
	if (!_Texture)
	{
		return false;
	}

	// Initialize the texture object.


	vector<char*> filenames{ filename };
	result = _Texture->InitializeArray(device, deviceContext, filenames);

	if (!result)
	{
		return false;
	}

	return true;
}


void FontClass::ReleaseTexture()
{
	// Release the texture object.
	if (_Texture)
	{
		delete _Texture;
		_Texture = 0;
	}

	return;
}


ID3D11ShaderResourceView* FontClass::GetTexture()
{
	//return m_Texture->GetTexture();
	ID3D11ShaderResourceView** views = _Texture->GetTextureArray();
	return views[0];
}

void FontClass::BuildVertexArray(void* vertices, std::string&& sentence, float drawX, float drawY)
{
	// Coerce the input vertices into a VertexType structure.
	VertexType* vertexPtr = ( VertexType* )vertices;

	// Initialize the index to the vertex array.
	int index = 0;

	auto SetVertex = [ &index, vertexPtr ]( const XMFLOAT3& Position, const XMFLOAT2& Texcoord )
	{
		vertexPtr[ index ].position = Position;
		vertexPtr[ index ].texture = Texcoord;
		++index;
	};

	// Draw each letter onto a quad.
	const auto str = std::move( sentence );
	const auto left = drawX;
	const auto top = drawY;
	const auto bottom = drawY - _fontHeight;

	for (const auto c : str )
	{
		const auto letter = c - 32;

		// If the letter is a space then just move over three pixels.		
		if (letter == 0)
		{
			drawX = drawX + (float)_spaceSize;
			continue;
		}

		const auto& type = _Font[ letter ];
		const auto right = drawX + type.size;

		// First triangle in quad.
		// Top left.
		SetVertex( XMFLOAT3( left, top, 0.0f ), XMFLOAT2( type.left, 0.0f ) );
		// Bottom right.
		SetVertex( XMFLOAT3( right, bottom, 0.0f ), XMFLOAT2( type.right, 1.0f ) );
		// Bottom left.
		SetVertex( XMFLOAT3( left, bottom, 0.f ), XMFLOAT2( type.left, 1.0f ) );

		// Second triangle in quad.
		// Top left.
		SetVertex( XMFLOAT3( top, left, 0.0f ), XMFLOAT2( type.left, 0.0f ) );
		// Top right.
		SetVertex( XMFLOAT3( right, top, 0.f ), XMFLOAT2( type.right, 0.f ) );
		// Bottom right.
		SetVertex( XMFLOAT3( right, bottom, 0.f ), XMFLOAT2( type.right, 1.f ) );

		// Update the x location for drawing by the size of the letter and one pixel.
		drawX += ( type.size + 1 );
	}
}

int FontClass::GetSentencePixelLength( const std::string& sentence )const
{
	int pixelLength = 0;

	for( const auto c : sentence )
	{
		const auto letter = c - 32;

		// If the letter is a space then count it as three pixels.
		pixelLength += letter == 0 ? _spaceSize : ( _Font[ letter ].size + 1 );
	}

	return pixelLength;
}

int FontClass::GetFontHeight()const
{
	return (int)_fontHeight;
}