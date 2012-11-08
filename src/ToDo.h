#ifndef _TODO_H
#define _TODO_H

#include "tstring.h"

#include "Window.h"
#include "CheckBox.h"
#include "TextEdit.h"

class ToDo : public Window
{
public:
	ToDo(Window *parent = 0);
	~ToDo();

	BOOL isCompleted() const;
	tstring getContent() const;

	void setCompleted(BOOL);
	void setContent(tstring);

protected:
	virtual void winClassDef(WNDCLASS &ws);

	virtual LRESULT CALLBACK winProc(
				HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
private:
	BOOL _isCompleted;
	tstring _content;

	CheckBox *checkBox;
	TextEdit *textEdit;
};

#endif /* _TODO_H */