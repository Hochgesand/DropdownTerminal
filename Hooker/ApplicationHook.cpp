#include "ApplicationHook.h"

Application_Hook::Application_Hook() = default;

Application_Hook::Application_Hook(const HWND hwnd, std::string appname)
{
	terminal_ = HookedWindow(hwnd, appname);
	GetWindowRect(terminal_.getHwnd(), &terminalPos);
}

long Application_Hook::calcRectHeight() const
{
	return terminalPos.bottom - terminalPos.top;
}

void Application_Hook::refreshTerminalPosition()
{
	GetWindowRect(terminal_.getHwnd(), &terminalPos);
}

std::shared_ptr<RECT> Application_Hook::getApplicationRect()
{
	return std::make_shared<RECT>(terminalPos);
}

std::shared_ptr<HookedWindow> Application_Hook::getApplicationInformation()
{
	return std::make_shared<HookedWindow>(terminal_);
}