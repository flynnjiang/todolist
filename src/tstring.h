#ifndef _TSTRING_H
#define _TSTRING_H

#include <string>

#ifdef _UNICODE 
using std::wstring;
#define tstring wstring 
#else 
using std::string;
#define tstring string 
#endif

#endif /* _TSTRING_H */