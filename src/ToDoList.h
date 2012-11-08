#ifndef _TODOLIST_H
#define _TODOLIST_H

#include <list>
using std::list;


#include "Window.h"
#include "ToDo.h"


class ToDoList : public Window
{
public:
	ToDoList();
	~ToDoList();

protected:
	virtual void winClassDef(WNDCLASS &ws);

	virtual LRESULT CALLBACK winProc(
				HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

	void itemUp(ToDo *todo);
	void itemDown(ToDo *todo);

	void loadItemsFromFile();
	void saveItemsToFile();

	void updateItems();

private:
	BOOL showCompleted;
	ToDo *newItem;

	std::list<ToDo *> items;
};

#endif /* _TODOLIST_H */