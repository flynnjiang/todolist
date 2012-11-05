#include <windows.h>

#include "Application.h"

HINSTANCE Application::hInst = NULL;

Application::Application(HINSTANCE instance)
{
	hInst = instance;
}

Application::~Application()
{
}

int Application::exec()
{
	MSG msg;

	msg.wParam = 0;
	while (::GetMessage(&msg, NULL, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}