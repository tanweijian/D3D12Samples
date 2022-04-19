#pragma once

#include <stdio.h>
#include <Windows.h>
#include <locale>

#ifdef _CONSOLE
inline void Print(const char* msg) { printf("%s", msg); }
inline void Print(const wchar_t* msg) { wprintf(L"%s", msg); }
#else
inline void Print(const char* msg) { OutputDebugStringA(msg); }
inline void Print(const wchar_t* msg) { OutputDebugStringW(msg); }
#endif // _CONSOLE

inline void Printf(const char* format, ...)
{
    char buffer[256];
    va_list ap;
    va_start(ap, format);
    vsprintf_s(buffer, 256, format, ap);
    va_end(ap);
    Print(buffer);
}

inline void Printf(const wchar_t* format, ...)
{
    wchar_t buffer[256];
    va_list ap;
    va_start(ap, format);
    vswprintf(buffer, 256, format, ap);
    va_end(ap);
    setlocale(LC_ALL, "chs");
    Print(buffer);
    setlocale(LC_ALL, "C");
}

inline void PrintSubMessage(const char* format, ...)
{
    char buffer[256];
    va_list ap;
    va_start(ap, format);
    vsprintf_s(buffer, 256, format, ap);
    va_end(ap);
    Print(buffer);
}

inline void PrintSubMessage(const wchar_t* format, ...)
{
    wchar_t buffer[256];
    va_list ap;
    va_start(ap, format);
    vswprintf(buffer, 256, format, ap);
    va_end(ap);
    Print(buffer);
}

#if NDEBUG
#define LOGERROR(msg, ...) do {} while(0)
#define LOGDEBUG(msg, ...) do {} while(0)
#define ASSERT_SUCCEEDED(hr, ...) (void)(hr)
#else
#define STRINGIFY(x) #x
#define STRINGIFY_BUILTIN(x) STRINGIFY(x)

#define LOGDEBUG(msg, ...) \
        Printf(msg "\n", ##__VA_ARGS__);

#define LOGERROR(msg, ...) \
        Print("\nError reported in " STRINGIFY_BUILTIN(__FILE__) " @ " STRINGIFY_BUILTIN(__LINE__) "\n"); \
        PrintSubMessage(msg, ##__VA_ARGS__); \
        Print("\n");

#define ASSERT_SUCCEEDED(hr, ...) \
        if (FAILED(hr)) { \
            Print("\nHRESULT failed in " STRINGIFY_BUILTIN(__FILE__) " @ " STRINGIFY_BUILTIN(__LINE__) "\n"); \
            PrintSubMessage("hr = 0x%08X", hr); \
            PrintSubMessage(__VA_ARGS__); \
            Print("\n"); \
            __debugbreak(); \
        }
#endif // RELEASE