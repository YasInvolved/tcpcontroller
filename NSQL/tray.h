#pragma once
#undef UNICODE
#include <Windows.h>
#include <Windows.UI.Notifications.h>
#include <wrl/client.h>
#include <cstdio>
#include <memory>
#include <iostream>
#include "resource.h"
#define ID_TRAY1 1
#define CM_JOINED 0x8001

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND createDummyWindow(HINSTANCE hInstance);
void createTray(HWND hWnd);
void showContextMenu(HWND hWnd, POINT pt);
void showJoinNotification(HWND hWnd, char* address);