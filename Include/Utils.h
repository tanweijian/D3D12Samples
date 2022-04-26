#pragma once
#include <stringapiset.h>

inline char* wchar2char(const wchar_t* wstr)
{
    int len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len];
    WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
    str[len] = '\0';
    return str;
}