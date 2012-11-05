#include <windows.h>
#include <wingdi.h>
#include <tchar.h>

#include "resource.h"
#include "Application.h"
#include "ToDoList.h"


ToDoList::ToDoList() :
	Window(0, WS_BORDER | WS_CAPTION | WS_SYSMENU , WS_EX_TOOLWINDOW)
{
}

ToDoList::~ToDoList()
{
	for (list<ToDo *>::iterator iter = toDoList.begin();
		iter != toDoList.end(); ++iter)
	{
		delete *iter;
	}

	for (list<ToDo *>::iterator iter = completed.begin();
		iter != completed.end(); ++iter)
	{
		delete *iter;
	}

	destory();
}

void ToDoList::create()
{
	NOTIFYICONDATA icon;

	Window::create();

	icon.hWnd = hSelf;
	icon.uID = 1;
	icon.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP ;
	icon.uCallbackMessage = WM_TDLIST_ICON_CMD;
	icon.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION)); 
	_tcscpy_s(icon.szTip,_T("To-Do List"));

	Shell_NotifyIcon(NIM_ADD, &icon);
}

void ToDoList::destory()
{
	NOTIFYICONDATA icon;

	icon.hWnd = hSelf;
	icon.uID = 1;

	Shell_NotifyIcon(NIM_DELETE, &icon);
}

void ToDoList::update()
{
	int y = 0;
	RECT rc;

	//resize(0, 0);

	for (list<ToDo *>::iterator iter = toDoList.begin();
		iter != toDoList.end(); ++iter)
	{
		(*iter)->move(0, y);
		(*iter)->show();

		y = y + (*iter)->height();

		//resize((*iter)->width(), height() + (*iter)->height() + 1);
	}

	for (list<ToDo *>::iterator iter = completed.begin();
		iter != completed.end(); ++iter)
	{
		(*iter)->move(0, y);
		(*iter)->show();

		y = y + (*iter)->height();

		//resize((*iter)->width(), height() + (*iter)->height() + 1);
	}

	resize(180, y + 24);

	SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
	move(rc.right - width(), rc.bottom - height());

	Window::update();
}

void ToDoList::itemUp(ToDo *todo)
{
}

void ToDoList::itemDown(ToDo *todo)
{
}

void ToDoList::setCompleted(ToDo *todo)
{
}

void ToDoList::loadData()
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
		_stscanf_s(buf, _T("%d %s"), &isCompleted, content);

		if (0 == isCompleted && 0 != _tcslen(content))
		{
			todo = new ToDo(this);
			todo->create();
			todo->setCompleted(FALSE);
			todo->setContent(content);
			toDoList.push_back(todo);
		}
	}

	fclose(fp);
}

void ToDoList::saveData()
{
	FILE *fp = NULL;
	TCHAR content[128] = {0};
	errno_t ret;

	ret = _tfopen_s(&fp, _T("todo.list"), _T("w+b"));
	if (0 != ret || NULL == fp)
		return;

	for (list<ToDo *>::iterator iter = toDoList.begin();
		iter != toDoList.end(); ++iter)
	{
		if (0 != (*iter)->getContent(content, 128))
			_ftprintf(fp, _T("%d %s\n"), (*iter)->isCompleted(), content);
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

	switch(Message)
	{
		case WM_CREATE:
		{
			loadData();

			ToDo *todo = new ToDo(this);
			todo->create();
			todo->setCompleted(FALSE);
			toDoList.push_back(todo);

			break;
		}

		case WM_SYSCOMMAND:
		{
			if (SC_CLOSE == wParam)
				saveData();
			
			return ::DefWindowProc(hwnd, Message, wParam, lParam);
		}

		case WM_TODO_COMPLETED_NOTIFY:
		{
			ToDo *toDo = (ToDo *)wParam;

			if (toDo->isCompleted())
			{
				toDoList.remove(toDo);
				completed.push_back(toDo);
			}
			else
			{
				completed.remove(toDo);
				toDoList.push_front(toDo);
			}

			update();
			
			break;
		}

		case WM_TODO_CONTENT_CHANGED_NOTIFY:
		{
			ToDo *toDo;
			list<ToDo *>::iterator iter;

			for (iter = toDoList.begin();
				iter != toDoList.end(); ++iter)
			{
				if ((*iter)->isEmpty() && (*iter) != (ToDo *)wParam)
					break;
			}

			if (iter == toDoList.end())
			{
				toDo = new ToDo(this);
				
				toDo->create();
				toDoList.push_back(toDo);
				update();
			}

			break;
		}

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