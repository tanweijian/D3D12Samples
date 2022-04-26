#pragma once

#include <stringapiset.h>

std::unique_ptr<char[]> wchar2char(const wchar_t* wstr)
{
    int len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
    std::unique_ptr<char[]> str(new char[len]);
    WideCharToMultiByte(CP_ACP, 0, wstr, -1, str.get(), len, NULL, NULL);
    return str;
}