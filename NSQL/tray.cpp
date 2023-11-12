#include "tray.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    printf("%d", msg);
    switch (msg)
    {
    case WM_CREATE:
        createTray(hWnd);
        break;
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_USER:
        switch (LOWORD(lParam))
        {
            case WM_RBUTTONUP:
            {
                POINT pt;
                GetCursorPos(&pt);
                showContextMenu(hWnd, pt);
                break;
            }
        }
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case WM_USER + 1:
            DestroyWindow(hWnd);
            exit(0);
            break;
        }
        break;
    case CM_JOINED:
        showJoinNotification(hWnd, (char*)wParam);
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}

HWND createDummyWindow(HINSTANCE hInstance)
{
    WNDCLASSEX wc = {
    .cbSize = sizeof(WNDCLASSEX),
    .style = 0,
    .lpfnWndProc = WndProc,
    .cbClsExtra = 0,
    .cbWndExtra = 0,
    .hInstance = hInstance,
    .hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE),
    .hCursor = LoadCursor(NULL, IDC_ARROW),
    .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
    .lpszMenuName = NULL,
    .lpszClassName = "chuj123",
    .hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE),
    };
    RegisterClassEx(&wc);
    HWND hWnd = CreateWindowEx(WS_EX_WINDOWEDGE, "chuj123", "okno spoko    ok?", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 10, 10, NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, SW_HIDE);
    UpdateWindow(hWnd);
    return hWnd;
}

void createTray(HWND hWnd)
{
    NOTIFYICONDATA nid = {
        .cbSize = sizeof(NOTIFYICONDATA),
        .hWnd = hWnd,
        .uID = ID_TRAY1,
        .uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO,
        .uCallbackMessage = WM_USER,
        .hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE),
        .dwInfoFlags = NIIF_USER,
    };
    lstrcpy(nid.szTip, "NSQL Server");
    lstrcpy(nid.szInfoTitle, "Hello");
    lstrcpy(nid.szInfo, "World");
    BOOL r;
    r = Shell_NotifyIcon(NIM_ADD, &nid);
    if (!r) MessageBox(hWnd, "No i chuj zjeba³o siê :(", "blad", MB_ICONEXCLAMATION);
}

void showContextMenu(HWND hWnd, POINT pt)
{
    HMENU hMenu = CreatePopupMenu();
    AppendMenu(hMenu, MF_STRING, WM_USER + 1, "exit");
    SetForegroundWindow(hWnd);
    TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
    DestroyMenu(hMenu);
}

void showJoinNotification(HWND hWnd, char* address)
{
    NOTIFYICONDATA nid = {
        .cbSize = sizeof(nid),
        .hWnd = hWnd,
        .uID = 1,
        .uFlags = NIF_INFO,
        .hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE),
        .uTimeout = 5000,
        .dwInfoFlags = NIIF_USER,
    };
    lstrcpy(nid.szInfoTitle, "New connection");
    std::string ad = address;
    ad += " Joined";
    lstrcpy(nid.szInfo, ad.c_str());
    BOOL r = Shell_NotifyIcon(NIM_MODIFY, &nid);
}