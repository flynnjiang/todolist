#include <windows.h>
#include <wingdi.h>
#include <tchar.h>

#include "resource.h"
#include "Application.h"
#include "ToDoList.h"


ToDoList::ToDoList() :
	Window(0, WS_BORDER | WS_CAPTION | WS_SYSMENU , WS_EX_TOOLWINDOW)
{
	showCompleted = TRUE;
}

ToDoList::~ToDoList()
{
}

void ToDoList::updateItems()
{
	int y = 0;
	RECT rc;

	for (list<ToDo *>::iterator iter = items.begin();
		iter != items.end(); ++iter)
	{

		if (!showCompleted && (*iter)->isCompleted())
		{
			(*iter)->hide();
			continue;
		}

		(*iter)->show();
		(*iter)->resize(180, 24);
		(*iter)->move(0, y);
		(*iter)->update();

		y = y + 24;
	}

	newItem->show();
	newItem->resize(180, 24);
	newItem->move(0, y);
	newItem->update();

	y += newItem->height() + 24;
	resize(180, y);

	SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
	move(rc.right - width(), rc.bottom - height());

	update();
}

void ToDoList::itemUp(ToDo *todo)
{
}

void ToDoList::itemDown(ToDo *todo)
{
}


void ToDoList::loadItemsFromFile()
{

	FILE *fp = NULL;
	TCHAR buf[128] = {0};
	TCHAR content[64] = {0};
	ToDo *todo = NULL;
	int isCompleted = 0;
	errno_t ret;

	ret = _tfopen_s(&fp, _T("todo.list"), _T("rb"));
	if (0 != ret || NULL == fp)
		return;

	while(_fgetts(buf, sizeof(buf), fp))
	{
		_stscanf_s(buf, _T("%d %s"), &isCompleted, content, _countof(content));

		if (0 == isCompleted && 0 != _tcslen(content))
		{
			todo = new ToDo(this);
			todo->create();
			todo->setCompleted(FALSE);
			todo->setContent(tstring(content));

			items.push_back(todo);
		}
	}

	fclose(fp);

}

void ToDoList::saveItemsToFile()
{
	FILE *fp = NULL;
	TCHAR content[128] = {0};
	errno_t ret;

	ret = _tfopen_s(&fp, _T("todo.list"), _T("w+b"));
	if (0 != ret || NULL == fp)
		return;

	for (list<ToDo *>::iterator iter = items.begin();
		iter != items.end(); ++iter)
	{
		if (!(*iter)->getContent().empty())
			_ftprintf(fp, _T("%d %s\n"),
					(*iter)->isCompleted(),
					(*iter)->getContent().c_str());
	}

	fclose(fp);
}


void ToDoList::winClassDef(WNDCLASS &ws)
{
	ws.lpszClassName = _T("ToDoList");
	ws.style = 0;
	ws.cbClsExtra = 0;
	ws.cbWndExtra = 0;
	ws.hIcon = NULL;
	ws.hCursor = NULL;
	ws.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	ws.lpszMenuName = NULL;
}

LRESULT CALLBACK ToDoList::winProc(
				HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	int y = 0;
	ToDo *todo = NULL;
	NOTIFYICONDATA icon;

	switch(Message)
	{
	case WM_CREATE:
		icon.hWnd = hSelf;
		icon.uID = 1;
		icon.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP ;
		icon.uCallbackMessage = WM_TDLIST_ICON_CMD;
		icon.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION)); 
		_tcscpy_s(icon.szTip,_T("To-Do List"));
		Shell_NotifyIcon(NIM_ADD, &icon);

		loadItemsFromFile();

		newItem = new ToDo(this);
		newItem->create();
		newItem->setContent(tstring(_T("")));

		updateItems();
		break;


	case WM_SYSCOMMAND:
		if (SC_CLOSE == wParam)
			saveItemsToFile();
		
		return ::DefWindowProc(hwnd, Message, wParam, lParam);


	case WM_TODO_COMPLETED_NOTIFY:
		todo = (ToDo *)wParam;
		items.remove(todo);
		if (todo->isCompleted())
			items.push_back(todo);
		else
			items.push_front(todo);

		updateItems();

		break;


	case WM_TODO_CONTENT_CHANGED_NOTIFY:
		if ((ToDo *)wParam == newItem && !newItem->getContent().empty())
		{
			todo = new ToDo(this);
			todo->create();
			todo->setContent(newItem->getContent());
			items.push_front(todo);

			newItem->setContent(tstring(_T("")));

			updateItems();
		}
		break;


	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_NCDESTROY:
		icon.hWnd = hSelf;
		icon.uID = 1;
		Shell_NotifyIcon(NIM_DELETE, &icon);

		for (list<ToDo *>::iterator iter = items.begin(); iter != items.end(); ++iter)
			delete *iter;

		items.clear();

		delete newItem;

	default :
	  return ::DefWindowProc(hwnd, Message, wParam, lParam);
	}

	return 0;
}