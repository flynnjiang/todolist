#include <tchar.h>

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

int TextEdit::getText(TCHAR *buf, int buf_len) const
{
	buf[0] = buf_len;
	return SendMessage(hSelf, EM_GETLINE, 0, (LPARAM)buf);
}

void TextEdit::setText(TCHAR *s)
{
	SendMessage(hSelf, EM_SETSEL, 0, -1);
	SendMessage(hSelf, EM_REPLACESEL, FALSE, (LPARAM)s);
}

void TextEdit::setTextLimit(int count)
{
	SendMessage(hSelf, EM_LIMITTEXT, (WPARAM)count, 0);
}