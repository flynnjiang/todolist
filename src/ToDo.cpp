#include <tchar.h>

#include "resource.h"
#include "ToDo.h"

ToDo::ToDo(Window *parent) :
	Window(parent, (parent ? WS_CHILD : 0))
{
	_isCompleted = 0;
	checkBox = new CheckBox(this);
	textEdit = new TextEdit(this);
}

ToDo::~ToDo()
{
	delete checkBox;
	delete textEdit;
}


BOOL ToDo::isCompleted() const
{
	return _isCompleted;
}


tstring ToDo::getContent() const
{
	return _content;
}

void ToDo::setCompleted(BOOL param)
{
	_isCompleted = param;
	checkBox->setChecked(_isCompleted);
}

void ToDo::setContent(tstring content)
{
	_content = content;
	textEdit->setText(_content);
}


void ToDo::winClassDef(WNDCLASS &ws)
{
	ws.lpszClassName = _T("ToDo");
	ws.style = 0;
	ws.cbClsExtra = 0;
	ws.cbWndExtra = 0;
	ws.hIcon = NULL;
	ws.hCursor = NULL;
	ws.hbrBackground = NULL;//(HBRUSH)GetStockObject(WHITE_BRUSH);
	ws.lpszMenuName = NULL;
}

LRESULT CALLBACK ToDo::winProc(
				HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	HFONT font;

	switch(Message)
	{

		case WM_CREATE:
			resize(180, 48);

			checkBox->create();
			checkBox->resize(16, 24);
			checkBox->move(0, 0);
			checkBox->setEnabled(TRUE);
			checkBox->show();
			
			textEdit->create();
			textEdit->resize(164, 24);
			textEdit->move(16, 0);
			textEdit->setMaxLength(32);
			textEdit->show();

			font = (HFONT)GetStockObject(ANSI_FIXED_FONT); 
			SendMessage(textEdit->getHandle(), WM_SETFONT, (WPARAM)font, TRUE);

			break;


		case WM_COMMAND:
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if ((HWND)lParam == checkBox->getHandle())
				{
					_isCompleted = checkBox->isChecked();
					SendMessage(hParent->getHandle(), WM_TODO_COMPLETED_NOTIFY, (WPARAM)this, 0);
				}
				break;

			case EN_KILLFOCUS:
			//case EN_SETFOCUS:
				SendMessage(hParent->getHandle(), WM_TODO_CONTENT_CHANGED_NOTIFY, (WPARAM)this, 0);
				break;

			case EN_CHANGE:
				_content = textEdit->getText();
				checkBox->setEnabled(!_content.empty());
				break;

			default:
				break;
			}

			break;


		case WM_KEYDOWN:

			break;

		case WM_CTLCOLOREDIT:
		//case WM_CTLCOLORBTN:
			if (_isCompleted)
				SetTextColor((HDC)wParam,RGB(0x99,0x99,0x99));
			else
				SetTextColor((HDC)wParam,RGB(0,0,0));

			SetBkColor((HDC)wParam,RGB(192,192,192));

			return (LONG)GetStockObject(LTGRAY_BRUSH);


		case WM_DESTROY:
			::PostQuitMessage(0);
			break;

		default :
			return ::DefWindowProc(hwnd, Message, wParam, lParam);
	}

	return 0;
}