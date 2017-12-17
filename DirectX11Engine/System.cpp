#include "System.h"
#include <dinput.h>
#include <DirectXMath.h>
#include "Position.h"

System::System()
{}

System::System(const System& other)
{}

System::~System()
{}

bool System::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	_Input.reset(new Input);
	if (!_Input){return false;}

	// Initialize the input object.
	result = _Input->Initialize(_hinstance, _hwnd, screenWidth, screenHeight);
	if (FAILED(result))
	{
		throw std::runtime_error("Could not initialize the input object. - line " + std::to_string(__LINE__));
		return false;
	}

	_Physics.reset(new Physics);
	if (!_Physics) return false;

	// Initialize the physics oject.
	result = _Physics->Initialize();
	if (!result)
	{
		throw std::runtime_error("Could not initialize the physics object. - line " + std::to_string(__LINE__));
	}

	_Scene.reset(new Scene);
	if (_Scene) result = _Scene->Initialize();

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	_Graphics.reset(new Graphics);
	if (!_Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	result = _Graphics->Initialize(screenWidth, screenHeight, _hwnd);
	if (!result)
	{
		throw std::runtime_error("Could not initialize the graphics object. - line " + std::to_string(__LINE__));
		return false;
	}

	// Create the timer object.
	_Timer.reset(new TimerClass);
	if (!_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = _Timer->Initialize();
	if (!result)
	{
		throw std::runtime_error("Could not initialize the timer object. - line " + std::to_string(__LINE__));
		return false;
	}

	_UI.reset(new UI);
	result = _UI->Initialize();

	return true;
}

void System::Shutdown()
{

	// Shutdown the window.
	ShutdownWindows();

	return;
}

void System::Run()
{
	MSG msg;
	bool done, result;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Tick();
			if (!result)
			{
				throw std::runtime_error("Frame Processing Failed. - line " + std::to_string(__LINE__));
				done = true;
			}
		}

		//@TODO: should this also be in process input?
		// Check if the user pressed escape and wants to quit.
		if (_Input->IsEscapePressed() == true)
		{
			done = true;
		}
	}
	
	return;
}

bool System::Tick()
{
	_Timer->Tick();

	_Input->Tick();

	_Scene->Tick(_Timer->GetTime(), _Input.get());

	_Graphics->UpdateFrame(_Timer->GetTime(), _Scene.get(), _UI->_Fps->GetFps(), _Scene->_Camera->GetMovementComponent()->GetPosition().x, _Scene->_Camera->GetMovementComponent()->GetPosition().y, _Scene->_Camera->GetMovementComponent()->GetPosition().z, _Scene->_Camera->GetMovementComponent()->GetOrientation().x, _Scene->_Camera->GetMovementComponent()->GetOrientation().y, _Scene->_Camera->GetMovementComponent()->GetOrientation().z);

	_UI->Tick();

	return true;
}

LRESULT CALLBACK System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void System::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.
	ApplicationHandle = this;

	// Get the instance of this application.
	_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	_applicationName = "Engine";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = _applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	_hwnd = CreateWindowEx(WS_EX_APPWINDOW, _applicationName, _applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, _hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(_hwnd, SW_SHOW);
	SetForegroundWindow(_hwnd);
	SetFocus(_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}

void System::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(_hwnd);
	_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(_applicationName, _hinstance);
	_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}