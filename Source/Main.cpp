//-----------------------------------------------------------------------------
// File: Main.cpp
//
// Desc: Main application entry & handling source file.
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Main Module Includes
//-----------------------------------------------------------------------------
#include "Main.h"
#include "CGameApp.h"

//-----------------------------------------------------------------------------
// Global Variable Definitions
//-----------------------------------------------------------------------------
HINSTANCE	g_hInst;	// Global instance

//-----------------------------------------------------------------------------
// Name : WinMain() (Application Entry Point)
// Desc : Entry point for program, App flow starts here.
//-----------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int iCmdShow )
{
	int retCode;

	// Enable Memory Leak Checking
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	//_crtBreakAlloc = 87;

	// initialize global instance
	g_hInst = hInstance;

    // create core game application processing engine
    CGameApp::Create();

	// Initialize the engine.
	if (!CGameApp::Get()->InitInstance( lpCmdLine, iCmdShow )) return 1;
	
	// Begin the gameplay process. Will return when app due to exit.
	retCode = CGameApp::Get()->BeginGame();

	// Shut down the engine, just to be polite, before exiting.
	if ( !CGameApp::Get()->ShutDown() )  MessageBox( 0, _T("Failed to shut system down correctly, please check file named 'debug.txt'.\r\n\r\nIf the problem persists, please contact technical support."), _T("Non-Fatal Error"), MB_OK | MB_ICONEXCLAMATION );

    // Destroy the game instance
    CGameApp::Destroy();

	// Return the correct exit code.
	return retCode;
}