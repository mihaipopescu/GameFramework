//-----------------------------------------------------------------------------
// File: Main.h
//
// Desc: Main application entry / handling header file.
//
// Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
//-----------------------------------------------------------------------------

#ifndef _MAIN_H_
#define _MAIN_H_

//-----------------------------------------------------------------------------
// Main Application Includes
//-----------------------------------------------------------------------------
#define CRTDBG_MAP_ALLOC
#include "..\\Res\\resource.h"
#include <windows.h>
#include <crtdbg.h>
#include <assert.h> 
#include "Commdlg.h"
#include <tchar.h>
#include <stdio.h>
#include <math.h>


//-----------------------------------------------------------------------------
// Common defines
//-----------------------------------------------------------------------------
#define CAPS1			94
#define C1_TRANSPARENT	1


#define EPS 1e-3 // epsilon (the smallest float value used)
#define PI 3.14159265358979323846
#define DEG2RAD(deg) (PI * (deg) / 180.0)
#define RAD2DEG(rad) ((rad) * 180.0 / PI)



#endif // _MAIN_H_