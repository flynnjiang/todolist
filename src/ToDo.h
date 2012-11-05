#ifndef _TODO_H
#define _TODO_H

#include "Window.h"
#include "CheckBox.h"
#include "TextEdit.h"

class ToDo : public Window
{
public:
	ToDo(Window *parent = 0);
	~ToDo();

	BOOL isEmpty() const;
	BOOL isCompleted() const;
	int getContent(TCHAR *buf, int len) const;

	void setCompleted(BOOL isCompleted);
	void setContent(TCHAR *content);

protected:
	virtual void winClassDef(WNDCLASS &ws);

	virtual LRESULT CALLBACK winProc(
				HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
private:
	TCHAR content[128];
	CheckBox *checkBox;
	TextEdit *textEdit;
};

#endif /* _TODO_H */