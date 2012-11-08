#include <windows.h>
#include <tchar.h>

#include "Application.h"
#include "CheckBox.h"

CheckBox::CheckBox(Window *parent, DWORD style) :
	Window(parent, style | BS_AUTOCHECKBOX | (parent ? WS_CHILD : 0), 0)
{
}

CheckBox::~CheckBox()
{
}


BOOL CheckBox::isChecked() const
{
	return (BST_CHECKED  == ::SendMessage(hSelf, BM_GETCHECK, 0, 0));
}

void CheckBox::setChecked(BOOL b)
{
	::SendMessage(hSelf, BM_SETCHECK, (b ? BST_CHECKED : BST_UNCHECKED), 0);
}


void CheckBox::winClassDef(WNDCLASS &ws)
{
	ws.lpszClassName = _T("BUTTON");
}