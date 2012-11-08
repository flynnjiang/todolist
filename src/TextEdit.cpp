#include <tchar.h>
#include "tstring.h"
#include "TextEdit.h"

TextEdit::TextEdit(Window *parent, DWORD style) :
	Window(parent, style | (parent ? WS_CHILD : 0))
{
}

TextEdit::~TextEdit()
{
}

void TextEdit::winClassDef(WNDCLASS &ws)
{
	ws.lpszClassName = _T("EDIT");
}

BOOL TextEdit::isEmpty() const
{
	TCHAR buffer[64];
	buffer[0] = 64;

	return (0 == SendMessage(hSelf, EM_GETLINE, 0, (LPARAM)buffer));
}


tstring TextEdit::getText() const
{
	LRESULT result;
	TCHAR *buf = NULL;
	tstring str;

	result = SendMessage(hSelf, EM_LINELENGTH, 0, 0);
	if (result > 0)
	{
		buf = new TCHAR[result+1]();
		buf[0] = result;
		result = SendMessage(hSelf, EM_GETLINE, 0, (LPARAM)buf);
		if (result > 0)
			str = buf;
		delete [] buf;
	}

	return str;
}

void TextEdit::setText(tstring s)
{
	SendMessage(hSelf, EM_SETSEL, 0, -1);
	SendMessage(hSelf, EM_REPLACESEL, FALSE, (LPARAM)s.c_str());
}



void TextEdit::setMaxLength(int count)
{
	SendMessage(hSelf, EM_LIMITTEXT, (WPARAM)count, 0);
}