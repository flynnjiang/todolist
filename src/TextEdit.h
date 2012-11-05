#ifndef _TEXTEDIT_H
#define _TEXTEDIT_H

#include "Window.h"

class TextEdit : public Window
{
public:
	TextEdit(Window *parent = 0 , DWORD style = 0);
	~TextEdit();

	BOOL isEmpty() const;
	int getText(TCHAR *buf, int buf_len) const;

	void setText(TCHAR *);
	void setTextLimit(int count);


protected:
	virtual void winClassDef(WNDCLASS &ws);
};

#endif /* _TEXTEDIT_H */