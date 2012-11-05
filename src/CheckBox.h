#ifndef _CHECKBOX_H
#define _CHECKBOX_H

#include "Window.h"

class CheckBox : public Window
{
public:
	CheckBox(Window *parent = NULL, DWORD style = 0);
	~CheckBox();

	BOOL isChecked() const;
	void setChecked(BOOL);

private:
	virtual void winClassDef(WNDCLASS &ws);
};

#endif /* _CHECKBOX_H */