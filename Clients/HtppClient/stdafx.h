// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#define DEBUG
#define UNICODE
#include "targetver.h"
#include <windows.h>
#include <tchar.h>
#include <memory>
#include <iostream>
#include <sstream>
#include <vector>
#include <functional>
#include <yvals.h>
#include "..\..\Lib\Connector\Connector.h"

#ifdef DEBUG
    #pragma comment(lib, "../../Lib/Connector/Debug/Connector.lib")
#else
    #pragma comment(lib, "../../Lib/Connector/Release/Connector.lib")
#endif
