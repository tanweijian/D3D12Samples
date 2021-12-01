#ifndef __LOG_H__
#define __LOG_H__

#include <cstdarg>
#include <tchar.h>
#include <debugapi.h>

void DebugPrint(LPCWSTR msg)
{
    OutputDebugString(msg);
}

void DebugPrintf(LPCWSTR format, ...)
{
    TCHAR s_str[1024] = {};
    va_list args;
    va_start(args, format);
    _vstprintf_s(s_str, sizeof(s_str), format, args);
    va_end(args);
    OutputDebugString(s_str);
}

#endif // !__LOG_H__

