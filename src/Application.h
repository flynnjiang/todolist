#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <windows.h>

class Application
{
public:
	Application(HINSTANCE instance);
	~Application();

	int exec();

public:
	static HINSTANCE hInst;
};

#endif /* _APPLICATION_H */