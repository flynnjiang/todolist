#ifndef _TEXTEDIT_H
#define _TEXTEDIT_H


#include "Window.h"
#include "tstring.h"

class TextEdit : public Window
{
public:
	TextEdit(Window *parent = 0 , DWORD style = 0);
	~TextEdit();

	BOOL isEmpty() const;
	tstring getText() const;

	void setText(tstring);
	void setMaxLength(int count);


protected:
	virtual void winClassDef(WNDCLASS &ws);
};

#endif /* _TEXTEDIT_H */