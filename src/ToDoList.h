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

	virtual void create();
	virtual void destory();
	virtual void update();

protected:
	virtual void winClassDef(WNDCLASS &ws);

	virtual LRESULT CALLBACK winProc(
				HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

	void itemUp(ToDo *todo);
	void itemDown(ToDo *todo);
	void setCompleted(ToDo *todo);

	void loadData();
	void saveData();

private:
	int showCompleted;
	std::list<ToDo *> toDoList;
	std::list<ToDo *> completed;
};

#endif /* _TODOLIST_H */