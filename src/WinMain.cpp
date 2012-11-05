#include <windows.h>
#include <tchar.h>

#include "Application.h"
#include "ToDoList.h"

int WINAPI WinMain(
			HINSTANCE hInstance,
			HINSTANCE hPrevInstance,
			LPSTR lpCmdLine,
			int nCmdShow
)
{
	Application app(hInstance);

	ToDoList toDoList;
	toDoList.create();
	toDoList.setTitle(_T("To-do List"));
	toDoList.show();
	toDoList.update();

	return app.exec();
}