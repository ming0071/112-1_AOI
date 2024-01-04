// Getting Started.cpp : Defines the entry point for the console application.
//

//===========================================================================//
//                                                                           //
//  Copyright (C) 2006 - 2017                                                //
//  IDS Imaging GmbH                                                         //
//  Dimbacherstr. 6-8                                                        //
//  D-74182 Obersulm-Willsbach                                               //
//                                                                           //
//  The information in this document is subject to change without            //
//  notice and should not be construed as a commitment by IDS Imaging GmbH.  //
//  IDS Imaging GmbH does not assume any responsibility for any errors       //
//  that may appear in this document.                                        //
//                                                                           //
//  This document, or source code, is provided solely as an example          //
//  of how to utilize IDS software libraries in a sample application.        //
//  IDS Imaging GmbH does not assume any responsibility for the use or       //
//  reliability of any portion of this document or the described software.   //
//                                                                           //
//  General permission to copy or modify, but not for profit, is hereby      //
//  granted,  provided that the above copyright notice is included and       //
//  reference made to the fact that reproduction privileges were granted     //
//  by IDS Imaging GmbH.                                                     //
//                                                                           //
//  IDS cannot assume any responsibility for the use or misuse of any        //
//  portion of this software for other than its intended diagnostic purpose  //
//  in calibrating and testing IDS manufactured cameras and software.        //
//                                                                           //
//===========================================================================//

/// Developer Note: I tried to keep it as simple as possible.
/// Therefore there are no functions asking for the newest driver software or freeing memory beforehand, etc.
/// This program is solely intended to show one of the simplest ways to interact with an IDS Camera through the uEye API.
/// Example was made with Visual Studio 2015 and uEye driver 4.90
/// (XS Cameras are not supported)

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include "uEye.h"
#include <conio.h>

#define CONSOLEDISPCLASS    "ConsoleDispClass"
#define CONSOLEDISPWINDOW   "ConsoleDispWindow"

using namespace std;

BOOL CreateDisplayWindow();
LRESULT WINAPI ConsoleDispWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

HIDS hCam = 0;          // 0 for the next available Camera. 1-254 to access a specific camera by ID
HWND hWndDisplay;
ATOM atom;
SENSORINFO sInfo;

char* pcImageMemory;
int MaxImageSizeX, MaxImageSizeY;
int DisplayWidth, DisplayHeight;

int main()
{
    DisplayWidth = 1280;
    DisplayHeight = 960;

    int nRet = 0;
    MSG msg;
    RECT rc;

    // windows function to create a display window
    CreateDisplayWindow();

    // starts the driver and establishes the connection to the camera
    nRet = is_InitCamera(&hCam, hWndDisplay);
    if (nRet != IS_SUCCESS)
    {
        cout << "ERROR" << endl;
    }
    else
    {
        cout << "Camera initialisation was successful!" << endl << endl;
    }

    // you can query information about the sensor type of the camera
    nRet = is_GetSensorInfo(hCam, &sInfo);
    if (nRet == IS_SUCCESS)
    {
        cout << "Cameramodel: \t\t" << sInfo.strSensorName << endl;
        cout << "Maximum image width: \t" << sInfo.nMaxWidth << endl;
        cout << "Maximum image height: \t" << sInfo.nMaxHeight << endl << endl << endl << endl;
    }
    MaxImageSizeX = sInfo.nMaxWidth;
    MaxImageSizeY = sInfo.nMaxHeight;
    DisplayWidth = MaxImageSizeX;
    DisplayHeight = MaxImageSizeY;


    ///*
    // Displaying a picture of a size below 1280x960 (to get the original image size please put the code block in brackets)
    int n = 1;
    do {
        DisplayWidth = MaxImageSizeX / n;
        n++;
    } while (DisplayWidth >= 1280);
    DisplayHeight = MaxImageSizeY / (n - 1);
    ///*/


    int nColorMode;
    int nBitsPerPixel;
    if (sInfo.nColorMode == IS_COLORMODE_BAYER)
    {
        // for color camera models use RGB24 mode
        nColorMode = IS_CM_BGR8_PACKED;
        nBitsPerPixel = 24;
    }
    else if (sInfo.nColorMode == IS_COLORMODE_CBYCRY)
    {
        // for CBYCRY camera models use RGB32 mode
        nColorMode = IS_CM_BGRA8_PACKED;
        nBitsPerPixel = 32;
    }
    else
    {
        // for monochrome camera models use Y8 mode
        nColorMode = IS_CM_MONO8;
        nBitsPerPixel = 8;
    }
    // Sets the color mode to be used when image data are saved or displayed by the graphics card
    is_SetColorMode(hCam, nColorMode);


    // allocates an image memory for an image, activates it and sets the way in which the images will be displayed on the screen
    int nMemoryId;
    is_AllocImageMem(hCam, MaxImageSizeX, MaxImageSizeY, nBitsPerPixel, &pcImageMemory, &nMemoryId);
    is_SetImageMem(hCam, pcImageMemory, nMemoryId);
    is_SetDisplayMode(hCam, IS_SET_DM_DIB);

    // Configuration of the display window
    SetWindowPos(hWndDisplay, NULL, 0, 0, DisplayWidth, DisplayHeight, SWP_NOMOVE | SWP_NOZORDER);
    GetClientRect(hWndDisplay, &rc);
    SetWindowPos(hWndDisplay, NULL, 0, 0, 2 * DisplayWidth - (rc.right - rc.left), 2 * DisplayHeight - (rc.bottom - rc.top), SWP_NOMOVE | SWP_NOZORDER);
    ShowWindow(hWndDisplay, SW_SHOW);

    // Acquires a single image from the camera
    is_CaptureVideo(hCam, IS_WAIT);

    // Output an image from an image memory in the specified window
    int nRenderMode = IS_RENDER_FIT_TO_WINDOW;

    // Dialog with the user
    cout << "Hello, in the following you see a recorded frame of the camera." << endl << endl;
    cout << "To SAVE it please press: \t\t S" << endl;
    cout << "To REFRESH it please press: \t\t R" << endl;
    cout << "And to QUIT the programm please press: \t Q" << endl << endl;
    cout << "HINT: try to hold R. You will get a simple live modus" << endl << endl << endl << endl;
    
    // Awaits input from user (Save, Refresh, Quit)
    while (1)
    {
        if (_kbhit())
        {
            int key = _getch();
            if ((key == 's') || (key == 'S'))
            {
                IMAGE_FILE_PARAMS ImageFileParams;
                ImageFileParams.pwchFileName = NULL;
                ImageFileParams.pnImageID = NULL;
                ImageFileParams.ppcImageMem = NULL;
                ImageFileParams.nQuality = 100;
                ImageFileParams.nFileType = IS_IMG_BMP;
                is_ImageFile(hCam, IS_IMAGE_FILE_CMD_SAVE, (void*)&ImageFileParams, sizeof(ImageFileParams));
            }

            if ((key == 'r') || (key == 'R'))
            {
                is_FreezeVideo(hCam, IS_WAIT);
                is_RenderBitmap(hCam, nMemoryId, hWndDisplay, nRenderMode);
            }

            if ((key == 'q') || (key == 'Q'))
                break;
        }
        PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
        DispatchMessage(&msg);
    }

    // Releases an image memory that was allocated
    nRet = is_FreeImageMem(hCam, pcImageMemory, nMemoryId);
    if (nRet == IS_SUCCESS)
    {
        cout << "The allocated memory has been successfully released!" << endl;
    }

    // Disables the hCam camera handle and releases the data structures and memory areas taken up by the uEye camera
    nRet = is_ExitCamera(hCam);
    if (nRet == IS_SUCCESS)
    {
        cout << "The camera has been successfully logged off!" << endl << endl;
    }



    system("pause");
    return 0;
}


//////////////////////////////////////////////////////   Windows Code   //////////////////////////////////////////////////////
BOOL CreateDisplayWindow()
{
    WNDCLASSEX wcx;
    HMODULE hInstance = GetModuleHandle(NULL);
    if (GetClassInfoEx(hInstance, _T(CONSOLEDISPCLASS), &wcx) == 0)
    {
        // Fill in the window class structure with parameters that describe the main window.
        wcx.cbSize = sizeof(wcx);                 // size of structure
        wcx.style = CS_HREDRAW | CS_NOCLOSE | CS_SAVEBITS | CS_VREDRAW | WS_OVERLAPPED;
        wcx.lpfnWndProc = ConsoleDispWndProc;     // points to window procedure
        wcx.cbClsExtra = 0;                       // no extra class memory
        wcx.cbWndExtra = 0;                       // no extra window memory
        wcx.hInstance = hInstance;                // handle to instance
        wcx.hIcon = NULL;                         // no icon
        wcx.hCursor = NULL;                       // no cursor
        wcx.lpszMenuName = NULL;                  // name of menu resource
        wcx.lpszClassName = _T(CONSOLEDISPCLASS); // name of window class
        wcx.hIconSm = NULL;                       // small class icon
        wcx.hbrBackground = NULL;

        // Register the window class.
        atom = RegisterClassEx(&wcx);

        if (atom != NULL)
        {
            // create our display window
            hWndDisplay = CreateWindow(_T(CONSOLEDISPCLASS), // name of window class
                L"SimpleSingleGrab",        // title-bar string
                WS_OVERLAPPEDWINDOW,        // top-level window
                CW_USEDEFAULT,              // default horizontal position
                CW_USEDEFAULT,              // default vertical position
                DisplayWidth,               // default width
                DisplayHeight,              // default height
                (HWND)NULL,                 // no owner window
                (HMENU)NULL,                // use class menu
                hInstance,                  // handle to application instance
                (LPVOID)NULL);              // no window-creation data

            if (!hWndDisplay)
            {
                DWORD a = GetLastError();
                LPVOID lpMsgBuf;
                FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS, NULL, a, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
					(LPTSTR)&lpMsgBuf, 0, NULL);

                // Free the buffer.
                LocalFree(lpMsgBuf);
                ::UnregisterClass(_T(CONSOLEDISPCLASS), hInstance);
                atom = NULL;
            }
        }
    }
    return (atom != NULL);
}

LRESULT WINAPI ConsoleDispWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    RECT rc;

    switch (msg)
    {
    case WM_CREATE:
        // Initialize the window.
        return 0;

    case WM_ERASEBKGND:
        if (GetUpdateRect(hwnd, &rc, FALSE))
        {
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOW + 1));
            EndPaint(hwnd, &ps);
        }
        return 1;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}