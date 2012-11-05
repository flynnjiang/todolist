#ifndef _WINDOW_H
#define _WINDOW_H

#include <windows.h>

class Window
{
protected:
	Window(Window *parent = NULL, DWORD style = 0, DWORD exStyle = 0);

public:	
	~Window();

	virtual void create();
	virtual void destory();

	virtual void show();
	virtual void hide();
	virtual void update();
	virtual void resize(int w, int h);
	virtual void move(int x, int y);
	virtual void setTitle(const TCHAR *title);
	void setEnabled(bool);

	BOOL isEnabled() const;
	RECT size() const;
	int width() const;
	int height() const;
	int x() const;
	int y() const;
	HWND getHandle() const;

protected:
	virtual void winClassDef(WNDCLASS &ws) = 0;

	virtual LRESULT CALLBACK winProc(
				HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK _winProc(
				HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

protected:
	Window *hParent;
	HWND hSelf;
	MDICREATESTRUCT mcs;
	DWORD flags;
	DWORD exFlags;
};

#endif /* _WINDOW_H */