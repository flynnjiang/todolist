#include <tchar.h>

#include "resource.h"
#include "ToDo.h"

ToDo::ToDo(Window *parent) :
	Window(parent, (parent ? WS_CHILD : 0))
{
	checkBox = new CheckBox(this);
	textEdit = new TextEdit(this);
}

ToDo::~ToDo()
{
	delete checkBox;
	delete textEdit;
}

BOOL ToDo::isEmpty() const
{
	return textEdit->isEmpty();
}

BOOL ToDo::isCompleted() const
{
	return checkBox->isChecked();
}


int ToDo::getContent(TCHAR *buf, int len) const
{
	return textEdit->getText(buf, len);
}

void ToDo::setCompleted(BOOL isCompleted)
{
	return checkBox->setChecked(isCompleted);
}

void ToDo::setContent(TCHAR *content)
{
	return textEdit->setText(content);
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
			//MessageBox( NULL, TEXT("Create checkBox"), TEXT("Error"), MB_OK | MB_ICONSTOP);
			this->resize(180, 24);

			checkBox->create();
			checkBox->resize(16, 24);
			checkBox->move(0, 0);
			checkBox->show();
			checkBox->setEnabled(FALSE);
			
			textEdit->create();
			textEdit->resize(164, 24);
			textEdit->move(16, 0);
			textEdit->setTextLimit(32);
			textEdit->show();

			font = (HFONT)GetStockObject(ANSI_FIXED_FONT); 
			SendMessage(textEdit->getHandle(), WM_SETFONT, (WPARAM)font, TRUE);
			break;

		case WM_COMMAND:
			switch (HIWORD(wParam))
			{
				case BN_CLICKED:
				{
					checkBox->setChecked(!checkBox->isChecked());
					textEdit->setEnabled(!checkBox->isChecked());
					SendMessage(hParent->getHandle(), WM_TODO_COMPLETED_NOTIFY,
							(WPARAM)this, (LPARAM)(checkBox->isChecked()));
					break;
				}

				case EN_SETFOCUS:
				{
					SendMessage(hParent->getHandle(), WM_TODO_CONTENT_CHANGED_NOTIFY,
							(WPARAM)this, 0);
					break;
				}

				case EN_CHANGE:
				{
					checkBox->setEnabled(!textEdit->isEmpty());
					break;
				}

				default:
					break;
			}
			break;

#if 1
		case WM_CTLCOLOREDIT:
		//case WM_CTLCOLORBTN:
		{
			if (checkBox->isChecked())
			{
				SetTextColor((HDC)wParam,RGB(0x99,0x99,0x99));
			}
			else
			{
				SetTextColor((HDC)wParam,RGB(0,0,0));
			}

			SetBkColor((HDC)wParam,RGB(192,192,192));

			return (LONG)GetStockObject(LTGRAY_BRUSH);

		}
#endif

		case WM_DESTROY:
		{
			::PostQuitMessage(0);
			break;
		}

		default :
		  return ::DefWindowProc(hwnd, Message, wParam, lParam);
	}

	return 0;
}