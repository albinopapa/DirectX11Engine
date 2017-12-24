////////////////////////////////////////////////////////////////////////////////
// Filename: Graphics.cpp
////////////////////////////////////////////////////////////////////////////////
#include "lightshaderclass.h"
#include "lightclass.h"
#include "Graphics.h"
#include "Scene.h"

Graphics::Graphics()
{}

Graphics::~Graphics()
{}

void Graphics::Initialize( int screenWidth, int screenHeight, HWND hwnd, Scene* scene )
{
	_D3D = std::make_unique<D3DClass>();
	_D3D->Initialize( screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR );

	_ShaderManager = std::make_unique<ShaderManagerClass>();
	_ShaderManager->Initialize( _D3D->GetDevice() );

	_Camera = std::make_unique<Camera>();
	_Camera->SetPosition( 0.0f, 0.0f, -4.f );
	_Camera->UpdateViewFromPosition();

	InitializeModels( hwnd, screenWidth, screenHeight, scene->_Actors );

	InitializeLights( scene );

	InitializeUI( screenWidth, screenHeight );

	// Initialize global effects
	_globalEffects.clipPlane = XMFLOAT4( 0.0f, 0.f, 0.0f, 0.0f );
	_globalEffects.fogStart = 0.f;
	_globalEffects.fogEnd = 3.f;
}
void Graphics::InitializeLights( Scene* pScene )
{
	// Create the skylight object.
	_Light = std::make_unique<LightClass>();

	// Initialize the light object.
	_Light->Initialize(
		{ 1.0f, 1.0f, 1.0f, 1.0f },		//Diffuse color
		{ 0.15f, 0.15f, 0.15f, 1.0f },	//Ambient color
		{ 1.0f, 1.0f, 1.0f, 1.0f },		//Specular color
		{ 0.f, 0.f, 0.f, 0.f },			//Position
		{ 0.0f, -1.0f, 0.5f },			//Direction
		16.f );							//SpecularPower

	// Create list of point lights
	for( int i = 0; i < NUM_LIGHTS; ++i )
	{
		_Lights.push_back( std::make_unique<LightClass>() );

		const XMFLOAT3 worldPosition = pScene->_LightActors[ i ]->GetMovementComponent()->GetPosition();
		_Lights[ i ]->SetPosition( worldPosition.x, worldPosition.y, worldPosition.z );
	}

	_Lights[ 0 ]->SetDiffuseColor( 1.0f, 0.0f, 0.0f, 1.0f );
	//_Lights[0]->SetPosition(-3.0f, 1.0f, 3.0f);

	_Lights[ 1 ]->SetDiffuseColor( 0.0f, 1.0f, 0.0f, 1.0f );
	//_Lights[1]->SetPosition(3.0f, 1.0f, 3.0f);

	_Lights[ 2 ]->SetDiffuseColor( 0.0f, 0.0f, 1.0f, 1.0f );
	//_Lights[2]->SetPosition(-3.0f, 1.0f, -3.0f);

	_Lights[ 3 ]->SetDiffuseColor( 1.0f, 1.0f, 1.0f, 1.0f );
	//_Lights[3]->SetPosition(3.0f, 1.0f, -3.0f);

	// STORE LIGHT DATA
	for( auto& light : _Lights )
	{
		_LightData.push_back( light.get() );
	}
}
void Graphics::InitializeModels( 
	const HWND &hwnd, 
	int screenWidth, 
	int screenHeight, 
	vector<unique_ptr<Actor>>& sceneActors )
{
	///////////////// DEFAULT APPEARANCE INIT /////////////////////
	for( auto& actor : sceneActors )
	{
		if( actor->bCustomAppearance )
		{
			continue;
		}

		auto model = std::make_unique<Model>();

		actor->SetModel( model.get() );

		vector<char*> defaultTex{
			"../DirectX11Engine/data/marble.png",
			"../DirectX11Engine/data/dirt.dds",
			"../DirectX11Engine/data/light.dds",
			"../DirectX11Engine/data/alpha.dds",
			"../DirectX11Engine/data/bumpMap.dds", // normal map
			"../DirectX11Engine/data/specMap.dds" };

		model->Initialize(
			_D3D->GetDevice(),
			_D3D->GetDeviceContext(),
			"../DirectX11Engine/data/sphere.txt",
			defaultTex,
			EShaderType::ELIGHT_SPECULAR );

		_DefaultModels.emplace_back( std::move( model ) );
	}

	///////////////// CUSTOM WATER DEMO /////////////////////
	_GroundModel = std::make_unique<Model>();

	// Initialize the ground model object.
	vector<char*>groundTex{
		"../DirectX11Engine/data/ground.dds",
		"../DirectX11Engine/data/dirt.dds",
		"../DirectX11Engine/data/light.dds",
		"../DirectX11Engine/data/alpha.dds",
		"../DirectX11Engine/data/blue.dds", // normal map
		"../DirectX11Engine/data/specMap.dds" };

	_GroundModel->Initialize( 
		_D3D->GetDevice(), 
		_D3D->GetDeviceContext(),
		"../DirectX11Engine/data/ground.txt", 
		groundTex, 
		EShaderType::ELIGHT_SPECULAR );

	sceneActors[ sceneActors.size() - 4 ]->SetModel( _GroundModel.get() );

	// Create the wall model object.
	_WallModel = std::make_unique<Model>();

	// Initialize the wall model object.
	vector<char*>wallTex{
		"../DirectX11Engine/data/wall.dds",
		"../DirectX11Engine/data/dirt.dds",
		"../DirectX11Engine/data/light.dds",
		"../DirectX11Engine/data/alpha.dds",
		"../DirectX11Engine/data/bumpMap.dds", // normal map
		"../DirectX11Engine/data/specMap.dds" };

	_WallModel->Initialize( 
		_D3D->GetDevice(), 
		_D3D->GetDeviceContext(), 
		"../DirectX11Engine/data/wall.txt", 
		wallTex, 
		EShaderType::ELIGHT_SPECULAR );

	sceneActors[ sceneActors.size() - 3 ]->SetModel( _WallModel.get() );

	// Create the bath model object.
	_BathModel = std::make_unique<Model>();

	vector<char*>bathTex{
		"../DirectX11Engine/data/marble.png",
		"../DirectX11Engine/data/dirt.dds",
		"../DirectX11Engine/data/light.dds",
		"../DirectX11Engine/data/alpha.dds",
		"../DirectX11Engine/data/blue.dds", // normal map
		"../DirectX11Engine/data/specMap.dds" };

	// Initialize the bath model object.
	_BathModel->Initialize( 
		_D3D->GetDevice(), 
		_D3D->GetDeviceContext(),
		"../DirectX11Engine/data/bath.txt", 
		bathTex, 
		EShaderType::EREFRACTION );

	sceneActors[ sceneActors.size() - 2 ]->SetModel( _BathModel.get() );

	// Create the water model object.
	_WaterModel = std::make_unique<Model>();

	vector<char*> waterTextures{
		"../DirectX11Engine/data/water.dds",
		"../DirectX11Engine/data/water.dds",
		"../DirectX11Engine/data/water.dds" };
	_WaterModel->Initialize( 
		_D3D->GetDevice(), 
		_D3D->GetDeviceContext(),
		"../DirectX11Engine/data/water.txt",
		waterTextures,
		EShaderType::EWATER );

	_WaterModel->GetMaterial()->reflectRefractScale = 0.01f;
	_WaterModel->GetMaterial()->waterHeight = sceneActors[ 3 ]->GetMovementComponent()->GetPosition().y;
	_WaterModel->GetMaterial()->bAnimated = true;

	sceneActors[ sceneActors.size() - 1 ]->SetModel( _WaterModel.get() );

	///////////////////////////////////////////////
	///////////// INIT RENDER TEXTURES //////////// (LATER ENCAPASULATE INTO MATERIALS)
	///////////////////////////////////////////////

	// Create the refraction render to texture object.
	_RefractionTexture = std::make_unique<RenderTextureClass>();

	// Initialize the refraction render to texture object.
	_RefractionTexture->Initialize( _D3D->GetDevice(), screenWidth, screenHeight );

	// Create the reflection render to texture object.
	_ReflectionTexture = std::make_unique<RenderTextureClass>();

	// Initialize the reflection render to texture object.
	_ReflectionTexture->Initialize( _D3D->GetDevice(), screenWidth, screenHeight );

	//////////////////////////////////////////////////////
	////// GLOBAL OBJECTS ////////////////////////////////
	//////////////////////////////////////////////////////

	// Create the frustum object.
	_Frustum = std::make_unique<FrustumClass>();

	// Create the model list object.
	_ModelList = std::make_unique<ModelListClass>();

	// Initialize the model list object.
	_ModelList->Initialize( 20 );
}
void Graphics::InitializeUI( int screenWidth, int screenHeight )
{
	char videoCard[ 128 ];
	int videoMemory;
	char videoString[ 144 ];
	char memoryString[ 32 ];
	char tempString[ 16 ];

	// Create the first font object.
	_Font1 = std::make_unique<FontClass>();

	// Initialize the first font object.
	_Font1->Initialize( 
		_D3D->GetDevice(), 
		_D3D->GetDeviceContext(), 
		"../DirectX11Engine/data/font.txt",
		"../DirectX11Engine/data/font.tga",
		32.0f, 
		3 );

	// Create the text object for the fps string.
	_FpsString = std::make_unique<TextClass>();

	// Initialize the fps text string.
	_FpsString->Initialize( 
		_D3D->GetDevice(), 
		_D3D->GetDeviceContext(), 
		screenWidth, 
		screenHeight, 
		16, 
		false, 
		_Font1.get(),
		"Fps: 0", 
		10, 
		50, 
		0.0f, 
		1.0f, 
		0.0f );

	// Initial the previous frame fps.
	_previousFps = -1;

	// Initialize the position text strings.
	vector<char*> labels = { "X: 0", "Y: 0", "Z: 0", "rX: 0", "rY: 0", "rZ: 0" };
	_PositionStrings.resize( labels.size() );

	for( size_t i = 0; i < labels.size(); ++i )
	{
		auto& posString = _PositionStrings[ i ];
		char* const label = labels[ i ];
		auto& prevPos = _previousPosition[ i ];

		posString = std::make_unique<TextClass>();

		posString->Initialize(
			_D3D->GetDevice(),
			_D3D->GetDeviceContext(),
			screenWidth,
			screenHeight,
			16,
			false,
			_Font1.get(),
			label,
			10,
			310 + ( i * 20 ),
			1.0f,
			1.0f,
			1.0f );

		prevPos = -1;
	}

	// Create the text objects for the render count strings.
	vector<char*> renderLabels = { "Polys Drawn: 0", "Cells Drawn: 0", "Cells Culled: 0" };

	for( int i = 0; i< 3; ++i )
	{
		auto renderStr = std::make_unique<TextClass>();

		renderStr->Initialize( 
			_D3D->GetDevice(), 
			_D3D->GetDeviceContext(), 
			screenWidth, 
			screenHeight, 
			32, 
			false, 
			_Font1.get(), 
			renderLabels[ i ], 
			10, 
			260 + ( i * 20 ), 
			1.0f, 
			1.0f, 
			1.0f );

		_RenderCountStrings.push_back( std::move( renderStr ) );
	}
}
void Graphics::UpdateFrame( float frameTime, Scene* scene, int fps )
{
	// 1. Animate Materials
	for( auto& actor : scene->_Actors )
	{
		actor->GetModel()->GetMaterial()->Animate();
	}

	// 2. Update Camera
	const XMFLOAT3 camPos = scene->GetCamera()->GetMovementComponent()->GetPosition();
	const XMFLOAT3 camRot = scene->GetCamera()->GetMovementComponent()->GetOrientation();
	_Camera->SetPosition( camPos.x, camPos.y, camPos.z );
	_Camera->SetRotation( camRot.x, camRot.y, camRot.z );

	// 3. Update Lights
	for( int i = 0; i < NUM_LIGHTS; ++i )
	{
		const XMFLOAT3 worldPosition = scene->_LightActors[ i ]->GetMovementComponent()->GetPosition();
		_Lights[ i ]->SetPosition( worldPosition.x, worldPosition.y, worldPosition.z );
	}

	// 4. Update UI
	UpdateFpsString( _D3D->GetDeviceContext(), fps );

	UpdatePositionStrings( 
		_D3D->GetDeviceContext(), 
		camPos.x, camPos.y, camPos.z, 
		camRot.x, camRot.y, camRot.z );

	DrawFrame( scene->_Actors, frameTime );
}
void Graphics::DrawFrame( const vector<unique_ptr<Actor>>& sceneActors, float frameTime )
{
	// Render the refraction of the scene to a texture.
	RenderRefractionToTexture( _WaterModel->GetMaterial()->waterHeight );

	// Render the reflection of the scene to a texture.
	RenderReflectionToTexture();

	// Render the scene as normal to the back buffer.
	RenderScene( sceneActors, frameTime );

	_D3D->TurnZBufferOff();

	RenderText();

	_D3D->TurnZBufferOn(); // Turn the Z buffer back on now that all 2D rendering has completed.

	_D3D->EndScene(); // Present the rendered scene to the screen.
}
void Graphics::RenderRefractionToTexture( float surfaceHeight )
{
	//XMFLOAT4 clipPlane;
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Setup a clipping plane based on the height of the water to clip everything above it.
	_globalEffects.clipPlane = XMFLOAT4( 0.0f, -1.0f, 0.0f, surfaceHeight );

	// Set the render target to be the refraction render to texture.
	_RefractionTexture->SetRenderTarget( _D3D->GetDeviceContext(), _D3D->GetDepthStencilView() );

	// Clear the refraction render to texture.
	_RefractionTexture->ClearRenderTarget( _D3D->GetDeviceContext(), _D3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f );

	// Generate the view matrix based on the camera's position.
	_Camera->UpdateViewFromPosition();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	_D3D->GetWorldMatrix( worldMatrix );
	_Camera->GetViewMatrix( viewMatrix );
	_D3D->GetProjectionMatrix( projectionMatrix );

	// Translate to where the bath model will be rendered.
	worldMatrix = DirectX::XMMatrixMultiply( worldMatrix, DirectX::XMMatrixTranslation( 0.0f, 2.0f, 0.0f ) );

	// Put the bath model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	_BathModel->RenderBuffers( _D3D->GetDeviceContext() );

	_ShaderManager->Render( 
		_D3D->GetDeviceContext(), 
		_BathModel->GetIndexCount(), 
		worldMatrix, 
		viewMatrix, 
		projectionMatrix,
		_BathModel->GetMaterial(), 
		_Light.get(), _Lights, _globalEffects );

	_globalEffects.clipPlane = XMFLOAT4( 0, 0, 0, 0 );

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	_D3D->SetBackBufferRenderTarget();
}
void Graphics::RenderReflectionToTexture()
{
	XMMATRIX worldMatrix, projectionMatrix;

	// Set the render target to be the reflection render to texture.
	_ReflectionTexture->SetRenderTarget( _D3D->GetDeviceContext(), _D3D->GetDepthStencilView() );

	// Clear the reflection render to texture.
	_ReflectionTexture->ClearRenderTarget( _D3D->GetDeviceContext(), _D3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f );

	// Use the camera to render the reflection and create a reflection view matrix.
	_Camera->RenderReflection( 2.75f );

	// Get the camera reflection view matrix instead of the normal view matrix.
	XMMATRIX reflectionViewMatrix = _Camera->GetReflectionViewMatrix();

	// Get the world and projection matrices from the d3d object.
	_D3D->GetWorldMatrix( worldMatrix );
	_D3D->GetProjectionMatrix( projectionMatrix );

	// Translate to where the wall model will be rendered.
	worldMatrix = DirectX::XMMatrixMultiply( worldMatrix, DirectX::XMMatrixTranslation( 0.0f, 6.0f, 8.0f ) );

	// Put the wall model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	_WallModel->RenderBuffers( _D3D->GetDeviceContext() );

	_ShaderManager->Render( _D3D->GetDeviceContext(), _WallModel->GetIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix,
		_WallModel->GetMaterial(), _Light.get(), _Lights, _globalEffects );

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	_D3D->SetBackBufferRenderTarget();
}
void Graphics::RenderScene( const vector<unique_ptr<Actor>>& sceneActors, float frameTime )
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, reflectionMatrix;

	// Clear the buffers to begin the scene.
	//@EFFECT - init to fog color here if you want to use fog
	_D3D->BeginScene( 0.0f, 0.0f, 0.0f, 1.0f ); 

	// Generate the view matrix based on the camera's position.
	_Camera->UpdateViewFromPosition();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	_D3D->GetWorldMatrix( worldMatrix );
	_Camera->GetViewMatrix( viewMatrix );
	_D3D->GetProjectionMatrix( projectionMatrix );

	//@TODO: TEMP HACK!!!!!! - MUST ENCAPSULATE!!!!!!!
	sceneActors[ 3 ]->GetModel()->GetMaterial()->GetTextureObject()->GetTextureArray()[ 0 ] = _ReflectionTexture->GetShaderResourceView();
	sceneActors[ 3 ]->GetModel()->GetMaterial()->GetTextureObject()->GetTextureArray()[ 1 ] = _RefractionTexture->GetShaderResourceView();
	//@TODO: TEMP HACK!!!!!! - MUST ENCAPSULATE!!!!!!!

	for( const auto& actor : sceneActors )
	{
		XMFLOAT3 translation = actor->GetMovementComponent()->GetPosition();
		worldMatrix = DirectX::XMMatrixMultiply( 
			worldMatrix, 
			DirectX::XMMatrixTranslation( translation.x, translation.y, translation.z ) );

		actor->GetModel()->RenderBuffers( _D3D->GetDeviceContext() );

		if( actor->GetModel()->GetMaterial()->transparency != 0.f )
			_D3D->EnableAlphaBlending();

		_ShaderManager->Render(
			_D3D->GetDeviceContext(),
			actor->GetModel()->GetIndexCount(),
			worldMatrix,
			viewMatrix,
			projectionMatrix,
			actor->GetModel()->GetMaterial(),
			_Light.get(),
			_Lights,
			_globalEffects,
			XMFLOAT3( 0, 0, 0 ),
			_Camera->GetReflectionViewMatrix() );

		if( actor->GetModel()->GetMaterial()->transparency != 0.f )
			_D3D->DisableAlphaBlending();

		// reset world matrix
		_D3D->GetWorldMatrix( worldMatrix );
	}


#pragma region MULTIMODELS
	//textureTranslation += .004f;	if (textureTranslation > 1.0f) { textureTranslation -= 1.0f; }

	//XMFLOAT4 clipPlane = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
	//float blendAmount = 0.5f;

	// Construct the frustum. //@TODO : is this also needed for reflection?
	//_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

	//float positionX, positionY, positionZ, radius;
	//XMFLOAT4 color;
	//bool renderModel;

	// Get the number of models that will be rendered.
	//int modelCount = _ModelList->GetModelCount();

	// Initialize the count of models that have been rendered.
	//int renderCount = 0;

	//XMMATRIX viewMatLocal;
	//_Camera->GetViewMatrix(viewMatLocal);

	//_D3D->EnableAlphaBlending();

	// Go through all the models and render them only if they can be seen by the camera view.
	//for (int index = 0; index < modelCount; index++)
	//{
	//	// Get the position and color of the sphere model at this index.
	//	_ModelList->GetData(index, positionX, positionY, positionZ, color);

	//	// Set the radius of the sphere to 1.0 since this is already known.
	//	radius = 1.0f;

	//	// Check if the sphere model is in the view frustum.
	//	renderModel = _Frustum->CheckSphere(positionX, positionY, positionZ, radius);

	//	// If it can be seen then render it, if not skip this model and check the next sphere.
	//	if (renderModel)
	//	{
	//		//Move the model to the location it should be rendered at.
	//		worldPosition = DirectX::XMMatrixTranslation(positionX, positionY, positionZ);

	//		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//		_Model->Render(_D3D->GetDeviceContext());

	//		result = _ShaderManager->RenderLightShader(
	//			_D3D->GetDeviceContext(),
	//			_Model->GetIndexCount(),
	//			worldPosition,
	//			viewMatLocal,//viewMatrix,
	//			projectionMatrix,
	//			_Model->GetTextureArrayDDS(),
	//			_Light->GetDirection(),
	//			/*color,*/ _Light->GetAmbientColor(),
	//			color, //_Light->GetDiffuseColor(), 
	//			_Camera->GetPosition(),
	//			/*color,*/ _Light->GetSpecularColor(),
	//			_Light->GetSpecularPower(),
	//			fogStart,
	//			fogEnd,
	//			clipPlane,
	//			textureTranslation,
	//			blendAmount,
	//			_Model->GetTextureArrayDDS()[0], //@TODO: must fix
	//			_Camera->GetReflectionViewMatrix());//viewMatrix); // @TODO: 
	//		if (!result)
	//		{
	//			return false;
	//		}

	//		// Reset to the original world matrix.
	//		_D3D->GetWorldMatrix(worldPosition);

	//		// Since this model was rendered then increase the count for this frame.
	//		renderCount++;
	//	}
	//}

	//_D3D->DisableAlphaBlending();

#pragma endregion

}
void Graphics::RenderText()
{
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

	_D3D->GetWorldMatrix( worldMatrix );
	_Camera->GetBaseViewMatrix( baseViewMatrix );
	_D3D->GetOrthoMatrix( orthoMatrix );

	_D3D->EnableAlphaBlending();

	_FpsString->Render( _D3D->GetDeviceContext(), _ShaderManager.get(), worldMatrix, baseViewMatrix, orthoMatrix, _Font1->GetTexture() );
	for( const auto& posStr : _PositionStrings )
	{
		posStr->Render( 
			_D3D->GetDeviceContext(), 
			_ShaderManager.get(), 
			worldMatrix, 
			baseViewMatrix, 
			orthoMatrix, 
			_Font1->GetTexture() );
	}
	for( const auto& counterStr : _RenderCountStrings )
	{
		counterStr->Render( 
			_D3D->GetDeviceContext(), 
			_ShaderManager.get(), 
			worldMatrix, 
			baseViewMatrix, 
			orthoMatrix, 
			_Font1->GetTexture() );
	}

	_D3D->DisableAlphaBlending();
}
void Graphics::UpdateFpsString( ID3D11DeviceContext* deviceContext, int fps )
{
	// Check if the fps from the previous frame was the same, if so don't need to update the text string.
	if( _previousFps == fps )
	{
		return;
	}

	// Store the fps for checking next frame.
	_previousFps = fps;

	// Truncate the fps to below 100,000.
	if( fps > 99999 )
	{
		fps = 99999;
	}

	// Convert the fps integer to string format.
	// Setup the fps string.
	std::string finalString = "Fps: " + std::to_string( fps );

	// If fps is 60 or above set the fps color to green.
	const XMFLOAT3 color = [ fps ]()
	{
		if( fps >= 60 )
		{
			return XMFLOAT3( 0.f, 1.f, 0.f );
		}
		else if( fps < 60 && fps >= 30)
		{
			return XMFLOAT3( 1.f, 1.f, 0.f );
		}
		else
		{
			return XMFLOAT3( 1.f, 0.f, 0.f );
		}
	}( );

	// @TODO: Set material values here

	// Update the sentence vertex buffer with the new string information.
	_FpsString->UpdateSentence( 
		deviceContext, 
		_Font1.get(), 
		finalString.c_str(), 
		10, 
		50, 
		color.x, 
		color.y, 
		color.z );
}
void Graphics::UpdatePositionStrings( ID3D11DeviceContext* deviceContext, float posX, float posY, float posZ,
	float rotX, float rotY, float rotZ )
{
	int positionX, positionY, positionZ, rotationX, rotationY, rotationZ;
	char tempString[ 16 ];
	char finalString[ 16 ];
	bool result;

	// Initialize the position text strings.
	vector<char*> labels = { "X: ", "Y: ", "Z: ", "rX: ", "rY: ", "rZ: " };
	vector<float> posRot = { posX, posY, posZ, rotX, rotY, rotZ };
	char offset = 0;
	for( int i = 0; i < 6; ++i )
	{
		_previousPosition[ i ] = ( int )posRot[ i ];
		_itoa_s( posRot[ i ], tempString, 10 );
		strcpy_s( finalString, labels[ i ] );
		strcat_s( finalString, tempString );
		result = _PositionStrings[ i ]->UpdateSentence( deviceContext, _Font1.get(), finalString, 10, 100 + offset, 1.0f, 1.0f, 1.0f );
		if( FAILED( result ) )
		{
			throw std::runtime_error( "Could not update sentence number " + to_string( i ) + " - line " + std::to_string( __LINE__ ) );
			return false;
		}

		offset += 20;
	}

	return true;
}
void Graphics::UpdateRenderCounts( ID3D11DeviceContext* deviceContext, int renderCount, int nodesDrawn, int nodesCulled )
{
	char tempString[ 32 ];
	char finalString[ 32 ];
	bool result;

	// Convert the render count integer to string format.
	_itoa_s( renderCount, tempString, 10 );

	// Setup the render count string.
	strcpy_s( finalString, "Polys Drawn: " );
	strcat_s( finalString, tempString );

	// Update the sentence vertex buffer with the new string information.
	result = _RenderCountStrings[ 0 ]->UpdateSentence( deviceContext, _Font1.get(), finalString, 10, 260, 1.0f, 1.0f, 1.0f );
	if( !result )
	{
		return false;
	}

	// Convert the cells drawn integer to string format.
	_itoa_s( nodesDrawn, tempString, 10 );

	// Setup the cells drawn string.
	strcpy_s( finalString, "Cells Drawn: " );
	strcat_s( finalString, tempString );

	// Update the sentence vertex buffer with the new string information.
	result = _RenderCountStrings[ 1 ]->UpdateSentence( deviceContext, _Font1.get(), finalString, 10, 280, 1.0f, 1.0f, 1.0f );
	if( !result )
	{
		return false;
	}

	// Convert the cells culled integer to string format.
	_itoa_s( nodesCulled, tempString, 10 );

	// Setup the cells culled string.
	strcpy_s( finalString, "Cells Culled: " );
	strcat_s( finalString, tempString );

	// Update the sentence vertex buffer with the new string information.
	result = _RenderCountStrings[ 2 ]->UpdateSentence( deviceContext, _Font1.get(), finalString, 10, 300, 1.0f, 1.0f, 1.0f );
	if( !result )
	{
		return false;
	}

	return true;
}