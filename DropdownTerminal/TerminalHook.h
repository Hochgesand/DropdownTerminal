#pragma once
#include "Window.h"
#include <functional>

void toggleTerminal();

class Terminal_hook
{
protected:
	void drop_terminal();
	void hide_terminal();
	BOOL isOpen;
	double calc_drop(const double x, const double k) const;
	long calc_rect_height();
public:
	Terminal_hook();
	Terminal_hook(HWND, std::string);
	RECT terminal_pos_;
	Window terminal_;
	void toggle_terminal();
	void get_terminal_position();
};