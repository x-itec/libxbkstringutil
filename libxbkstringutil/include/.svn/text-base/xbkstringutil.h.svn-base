#ifndef xbkstringh
#define xbkstringh

#include<string>
#include<stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include <map>
#include <iostream>

extern int errno;



/*

#define DEBUGME
#define QS

#include "xdebug.c"
#include "xqs.c"
*/


/*
Getenv fuehrt bei Leerwerten zum Absturz,
deswegen meine eigene Implementation
*/
class xbkstringutil
{
public:
string xbkgetenv(string envi);
string stripslashes(string text);
string addslashes(string text);
string trim(string text);
string str_replace(string searchfor,string replacetxt,string origtxt );
vector<string> explodec(string sep,string text);
string URLDecode(string text);
string URLEncode(string text);
};
#endif
