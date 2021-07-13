#include "WindowGrabber.h"

#include <Windows.h>
#include <iostream>

#include "HookedWindow.h"

std::map<HWND, std::string> openWindows;

BOOL CALLBACK enumWindowCallback(HWND hWnd, LPARAM lparam) {
	const int length = GetWindowTextLength(hWnd);
    TCHAR* buffer = (TCHAR*) (malloc(1 + length));
    GetWindowText(hWnd, buffer, length + 1);

    // List visible windows with a non-empty title
    if (IsWindowVisible(hWnd) && length != 0) {
        const std::unique_ptr<HookedWindow> foundWindow = std::make_unique<HookedWindow>(HookedWindow(hWnd, buffer));
        openWindows.insert({ foundWindow->getHwnd(), foundWindow->getAppName() });
    }
    free(buffer);
    return TRUE;
}

std::map<HWND, std::string> WindowGrabber::getOpenWindowsApplications() {
    openWindows = {};
    EnumWindows(enumWindowCallback, NULL);
    return openWindows;
}

