
////////////////////////////////////////////////////////////////////////////////
// Filename: model.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Model.h"

using namespace GfxUtil;

Model::Model()
	:
	_vertexBuffer(0),
	_indexBuffer(0)//,
	//_TextureArray(0)
{}

Model::Model(const Model& other)
{}

Model::~Model()
{}

bool Model::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* modelFilename, char* textureFilename1, char* textureFilename2, char* lightMapFileName3, char* alphaFileName4, char* normalMapFilename5, char* specMapFilename6)
{
	bool result;

	// Load in the model data
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	// Calculate the normal, tangent, and binormal vectors for the model.
	CalculateModelVectors();

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	//TODO: ENCAPSULATE INTO INIT MATERIAL
	//result = LoadTextures(device, deviceContext, textureFilename1, textureFilename2, lightMapFileName3, alphaFileName4, normalMapFilename5, specMapFilename6);
	//if (!result){return false;}

	// Loads the textures in the material. Material now holds pointer to texture class
	_material.reset(new Material);

	vector<char*> fileNames{ textureFilename1, textureFilename2, lightMapFileName3, alphaFileName4, normalMapFilename5, specMapFilename6 };
	_material->Initialize(device, deviceContext, EShaderType::ELIGHT_SPECULAR, fileNames);

	// TESTING AREA for different string functions
	//WCHAR* wstr = (WCHAR*)fileName1; // not working
	//WCHAR* wstr2 = reinterpret_cast<WCHAR*>(fileName1); // not working
	//const WCHAR *pwcsName2 = charToWChar(fileName1); // works even though Microsoft doesnt like it
	//const WCHAR *pwcsName3 = charToWChar_S(fileName1); // BAD
	// TESTING AREA

	return true;
}

Material* Model::GetMaterial()
{
	return _material.get();
}

void Model::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();
}

void Model::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

int Model::GetIndexCount()
{
	return _indexCount;
}

ID3D11ShaderResourceView** Model::GetTextureArray()
{
	//return _TextureArray->GetTextureArray();

	return _material->GetResourceArray();
}

bool Model::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Create the vertex array.
	vertices = new VertexType[_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (int i = 0; i < _vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(_model.get()[i].x, _model.get()[i].y, _model.get()[i].z);
		vertices[i].texture = XMFLOAT2(_model.get()[i].tu, _model.get()[i].tv);
		vertices[i].normal = XMFLOAT3(_model.get()[i].nx, _model.get()[i].ny, _model.get()[i].nz);
		vertices[i].tangent = XMFLOAT3(_model.get()[i].tx, _model.get()[i].ty, _model.get()[i].tz);
		vertices[i].binormal = XMFLOAT3(_model.get()[i].bx, _model.get()[i].by, _model.get()[i].bz);

		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void Model::ShutdownBuffers()
{
	// Release the index buffer.
	if (_indexBuffer)
	{
		_indexBuffer->Release();
		_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (_vertexBuffer)
	{
		_vertexBuffer->Release();
		_vertexBuffer = 0;
	}

	return;
}

void Model::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;
	
	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

//bool Model::LoadTextures(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fileName1, char* fileName2, char* fileName3, char* fileName4, char* normalMapFileName, char* specMapFilename6)
//{
//	bool result;
//
//	// Create the texture object.
//	//_TextureArray = new TextureClass;
//	//if (!_TextureArray)
//	//{
//	//	return false;
//	//}
//
//	
//	
//	
//	
//	
//	
//
//	//vector<char*> fileNames{ fileName1, fileName2, fileName3, fileName4, normalMapFileName, specMapFilename6 };
//
//	//result = _TextureArray->InitializeArray(device, deviceContext, fileNames);
//	//if (!result)
//	//{
//	//	return false;
//	//}
//
//	return true;
//}

void Model::CalculateModelVectors()
{
	int faceCount, index;
	TempVertexType vertex1, vertex2, vertex3;
	VectorType tangent, binormal, normal;

	// Calculate the number of faces in the model.
	faceCount = _vertexCount / 3;

	// Initialize the index to the model data.
	index = 0;

	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for (int i = 0; i < faceCount; i++)
	{
		// Get the three vertices for this face from the model.
		vertex1.x = _model.get()[index].x;
		vertex1.y = _model.get()[index].y;
		vertex1.z = _model.get()[index].z;
		vertex1.tu = _model.get()[index].tu;
		vertex1.tv = _model.get()[index].tv;
		vertex1.nx = _model.get()[index].nx;
		vertex1.ny = _model.get()[index].ny;
		vertex1.nz = _model.get()[index].nz;
		index++;

		vertex2.x = _model.get()[index].x;
		vertex2.y = _model.get()[index].y;
		vertex2.z = _model.get()[index].z;
		vertex2.tu = _model.get()[index].tu;
		vertex2.tv = _model.get()[index].tv;
		vertex2.nx = _model.get()[index].nx;
		vertex2.ny = _model.get()[index].ny;
		vertex2.nz = _model.get()[index].nz;
		index++;

		vertex3.x = _model.get()[index].x;
		vertex3.y = _model.get()[index].y;
		vertex3.z = _model.get()[index].z;
		vertex3.tu = _model.get()[index].tu;
		vertex3.tv = _model.get()[index].tv;
		vertex3.nx = _model.get()[index].nx;
		vertex3.ny = _model.get()[index].ny;
		vertex3.nz = _model.get()[index].nz;
		index++;

		// Calculate the tangent and binormal of that face.
		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		// Calculate the new normal using the tangent and binormal.
		CalculateNormal(tangent, binormal, normal);

		// Store the normal, tangent, and binormal for this face back in the model structure.
		_model.get()[index - 1].nx = normal.x;
		_model.get()[index - 1].ny = normal.y;
		_model.get()[index - 1].nz = normal.z;
		_model.get()[index - 1].tx = tangent.x;
		_model.get()[index - 1].ty = tangent.y;
		_model.get()[index - 1].tz = tangent.z;
		_model.get()[index - 1].bx = binormal.x;
		_model.get()[index - 1].by = binormal.y;
		_model.get()[index - 1].bz = binormal.z;

		_model.get()[index - 2].nx = normal.x;
		_model.get()[index - 2].ny = normal.y;
		_model.get()[index - 2].nz = normal.z;
		_model.get()[index - 2].tx = tangent.x;
		_model.get()[index - 2].ty = tangent.y;
		_model.get()[index - 2].tz = tangent.z;
		_model.get()[index - 2].bx = binormal.x;
		_model.get()[index - 2].by = binormal.y;
		_model.get()[index - 2].bz = binormal.z;

		_model.get()[index - 3].nx = normal.x;
		_model.get()[index - 3].ny = normal.y;
		_model.get()[index - 3].nz = normal.z;
		_model.get()[index - 3].tx = tangent.x;
		_model.get()[index - 3].ty = tangent.y;
		_model.get()[index - 3].tz = tangent.z;
		_model.get()[index - 3].bx = binormal.x;
		_model.get()[index - 3].by = binormal.y;
		_model.get()[index - 3].bz = binormal.z;
	}

	return;
}

void Model::CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3,
	VectorType& tangent, VectorType& binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];
	float den;
	float length;


	// Calculate the two vectors for this face.
	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;

	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	// Calculate the tu and tv texture space vectors.
	tuVector[0] = vertex2.tu - vertex1.tu;
	tvVector[0] = vertex2.tv - vertex1.tv;

	tuVector[1] = vertex3.tu - vertex1.tu;
	tvVector[1] = vertex3.tv - vertex1.tv;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// Normalize the normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// Normalize the normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;

	return;
}

void Model::CalculateNormal(VectorType tangent, VectorType binormal, VectorType& normal)
{
	float length;


	// Calculate the cross product of the tangent and binormal which will give the normal vector.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// Calculate the length of the normal.
	length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// Normalize the normal.
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;

	return;
}

bool Model::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;

	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> _vertexCount;

	// Set the number of indices to be the same as the vertex count.
	_indexCount = _vertexCount;

	// Create the model using the vertex count that was read in.
	//_model = new ModelType[_vertexCount];
	_model.reset(new ModelType[_vertexCount]);
	if (!_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i < _vertexCount; i++)
	{
		fin >> _model.get()[i].x >>  _model.get()[i].y >> _model.get()[i].z;
		fin >> _model.get()[i].tu >> _model.get()[i].tv;
		fin >> _model.get()[i].nx >> _model.get()[i].ny >> _model.get()[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}