#include <windows.h>
#include <tchar.h>
#include <assert.h>

#include "Window.h"
#include "Application.h"

Window::Window(Window *parent, DWORD style, DWORD exStyle)
{
	hParent = parent;
	hSelf = NULL;
	flags = style;
	exFlags = exStyle;
}

Window::~Window()
{
}


void Window::create()
{
	WNDCLASS ws;

	ws.style = 0;
	ws.cbClsExtra = 0;
	ws.cbWndExtra = 0;
	ws.hIcon = NULL;
	ws.hCursor = NULL;
	ws.hbrBackground = NULL;
	ws.lpszMenuName = NULL;
	//ws.lpszClassName = cs.lpszClass;

	winClassDef(ws);

	if (!::GetClassInfo(Application::hInst, ws.lpszClassName, &ws))
	{
		ws.hInstance = Application::hInst;
		ws.lpfnWndProc = _winProc;

		::RegisterClass(&ws);
		//MessageBox( NULL, TEXT("GetClassInfo Failed"), TEXT("Error"), MB_OK | MB_ICONSTOP);
	}

	mcs.lParam = (LPARAM)this;
	hSelf = ::CreateWindowEx(
			exFlags,
			ws.lpszClassName,
			_T(""),
			flags,
			0, 0,
			64, 64,
			(hParent ? hParent->getHandle() : NULL),
			NULL,
			Application::hInst,
			&mcs);
}

void Window::destory()
{
	if (NULL != hSelf)
		::DestroyWindow(hSelf);

	hSelf = NULL;
}

void Window::show()
{
	if (NULL != hSelf)
	{
		::ShowWindow(hSelf, SW_SHOW);
		::UpdateWindow(hSelf);
	}
}

void Window::hide()
{
	if (NULL != hSelf)
		::ShowWindow(hSelf, SW_HIDE);
}

void Window::update()
{
	if (NULL != hSelf)
	{
		::InvalidateRect(hSelf, NULL, TRUE);
		::UpdateWindow(hSelf);
	}
}

void Window::resize(int w, int h)
{
	::MoveWindow(hSelf, x(), y(), w, h, TRUE);
}

void Window::move(int x, int y)
{
	::MoveWindow(hSelf, x, y, width(), height(), TRUE);
}

void Window::setTitle(const TCHAR *title)
{
	::SetWindowText(hSelf, title);
}

void Window::setEnabled(bool isEnabled)
{
	EnableWindow(hSelf, isEnabled);
}

BOOL Window::isEnabled() const
{
	return IsWindowEnabled(hSelf);
}

RECT Window::size() const
{
	RECT rc;

	::GetWindowRect(hSelf, &rc);

	return rc;
}

int Window::width() const
{
	RECT rc;

	::GetWindowRect(hSelf, &rc);

	return (rc.right - rc.left);
}

int Window::height() const
{
	RECT rc;

	::GetWindowRect(hSelf, &rc);

	return (rc.bottom - rc.top);
}

int Window::x() const
{
	return size().left;
}

int Window::y() const
{
	return size().top;
}

HWND Window::getHandle() const
{
	return hSelf;
}


LRESULT CALLBACK Window::winProc(
				HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case WM_DESTROY:
		{
			::PostQuitMessage(0);
			break;
		}

		default :
		  return ::DefWindowProc(hwnd, Message, wParam, lParam);
	}

	return FALSE;
}

LRESULT CALLBACK Window::_winProc(
				HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	Window* pWindow;

	if ( Message == WM_NCCREATE )
	{
		assert( ! IsBadReadPtr((void *) lParam, sizeof(CREATESTRUCT)) );

		MDICREATESTRUCT * pMDIC = (MDICREATESTRUCT *)
			((LPCREATESTRUCT) lParam)->lpCreateParams;

		pWindow = (Window *) (pMDIC->lParam);

		assert( ! IsBadReadPtr(pWindow, sizeof(Window)) );

		pWindow->hSelf = hwnd; // very important, it's too late to get
							   // this value after CreateWindow return!

		SetWindowLong(hwnd, GWL_USERDATA, (LONG) pWindow);
	}
	else
		pWindow=(Window *)GetWindowLong(hwnd, GWL_USERDATA);

	if ( pWindow )
		return pWindow->winProc(hwnd, Message, wParam, lParam);
	else
		return DefWindowProc(hwnd, Message, wParam, lParam);
}


