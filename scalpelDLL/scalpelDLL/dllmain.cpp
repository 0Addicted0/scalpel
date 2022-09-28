// dllmain.cpp: 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <set>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <detours.h>
#include <windows.h>
#include <string.h>
#include <Winsock2.h>
#include "../../scalpelGUI/scalpelGUI/sharedMem.h"
#pragma comment(lib,"detours.lib")
#pragma comment(lib,"Ws2_32.lib")
//#pragma execution_character_set("utf-8")  
#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#define PRINTNAME
#define WITH_SHARED_MEM
#define FILTER 1
//SYSTEMTIME st;           //unnecessary

#ifdef WITH_SHARED_MEM
HANDLE hLockerSemaphore = OpenSemaphoreA(EVENT_ALL_ACCESS, FALSE, SHARED_LOCKSEM_NAME);
HANDLE hSemaphore = OpenSemaphoreA(EVENT_ALL_ACCESS, FALSE, SHARED_SEM_NAME);
HANDLE hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, NULL, SHARED_MEM_NAME);
LPVOID lpBase = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(scalpel::sharedM::hookedInfo));
scalpel::sharedM::hookedInfo *pkt = (scalpel::sharedM::hookedInfo*)lpBase;
#endif
#ifndef WITH_SHARED_MEM
scalpel::sharedM::hookedInfo* pkt = (scalpel::sharedM::hookedInfo*)malloc(sizeof(scalpel::sharedM::hookedInfo));//for test
#endif


std::set<UINT> HandleSet;//用于记录跟踪过的句柄(去除递归的重复调用) *it = (UINT)
/*
    tool function declare
*/
bool isTraced(HANDLE hHandle);
int deTrace(HANDLE hHandle);
int trace(HANDLE hHandle);
void EnableDetours();
void DisableDetours();
/*
    detours modified list
*/
//消息框操作 MessageBoxA/W
static int (WINAPI* OldMessageBoxW)(_In_opt_ HWND hwnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType) = MessageBoxW;
static int (WINAPI* OldMessageBoxA)(_In_opt_ HWND hwnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType) = MessageBoxA;
//堆操作 HeapCreatE HeapDestroy HeapAlloc HeapFree
static HANDLE(WINAPI* OldHeapCreate)(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize) = HeapCreate;
static BOOL(WINAPI* OldHeapDestroy)(HANDLE) = HeapDestroy;
static LPVOID(WINAPI* OldHeapAlloc)(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes) = HeapAlloc;
static BOOL(WINAPI* OldHeapFree)(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem) = HeapFree;
//文件操作 CreateFileA CreateFileW WriteFile ReadFile _lclose CloseHandle
static HANDLE(WINAPI* OldCreateFileA)(
    LPCSTR lpFileName,                          // 文件名
    DWORD dwDesireAccess,                       // 访问模式
    DWORD dwShareMode,                          // 共享模式
    LPSECURITY_ATTRIBUTES lpSecurityAttributes, // 安全属性(也即销毁方式)
    DWORD dwCreationDispoition,                 // 创建方式
    DWORD dwFlagsAttributes,                    // 文件属性
    HANDLE hTemplateFile                        // 模板文件句柄
    ) = CreateFileA;
static HANDLE(WINAPI* OldCreateFileW)(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) = CreateFileW;
static BOOL(WINAPI* OldWriteFile)(
    HANDLE hFile,
    LPCVOID lpBuffer,
    DWORD nNumberOfBytesToWrite,
    LPDWORD lpNumberOfBytesWritten,
    LPOVERLAPPED lpOverlapped
    ) = WriteFile;
static BOOL(WINAPI* OldReadFile)(
    HANDLE hFile,
    LPVOID lpBuffer,
    DWORD nNumberOfBytesToRead,
    LPDWORD lpNumberOfBytesRead,
    LPOVERLAPPED lpOverlapped
    ) = ReadFile;
static BOOL(WINAPI* _Oldlclose)(HFILE hFile) = _lclose;
static BOOL(WINAPI* OldCloseHandle)(HANDLE hObject) = CloseHandle;
static BOOL(WINAPI* OldCopyFileA)(LPCSTR lpExistingFileName,LPCSTR lpNewFileName, BOOL beFailIfExist) = CopyFileA;
//注册表操作 RegCreateKeyExA RegOpenKeyExA RegCloseKey RegQueryValueA RegSetKeyValueA RegDeleteValueA RegSetValueExA
static LSTATUS(WINAPI* OldRegCreateKeyExA)(
    HKEY hKey,
    LPCSTR lpSubKey,
    DWORD Reserved,
    LPSTR lpClass,
    DWORD dwOptions,
    REGSAM samDesired,
    const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    PHKEY phkResult,
    LPDWORD lpdwDisposition
    ) = RegCreateKeyExA;
static LSTATUS(WINAPI* OldRegCreateKeyA)(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult) = RegCreateKeyA;
static LSTATUS(WINAPI* OldRegOpenKeyExA)(
    HKEY hKey,
    LPCSTR lpSubKey,
    DWORD ulOptions,
    REGSAM samDesired,
    PHKEY phkresult
    ) = RegOpenKeyExA;
static LSTATUS(WINAPI* OldRegOpenKeyA)(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult) = RegOpenKeyA;
static LSTATUS(WINAPI* OldRegCloseKey)(HKEY hKey) = RegCloseKey;
static LSTATUS(WINAPI* OldRegDeleteKeyA)(HKEY hKey, LPCSTR lpSubKey) = RegDeleteKeyA;
static LSTATUS(WINAPI* OldRegQueryValueA)(HKEY hKey, LPCSTR lpsubKey, LPSTR lpData, PLONG lpcbData) = RegQueryValueA;
static LSTATUS(WINAPI* OldRegSetKeyValueA)(
    HKEY hKey,
    LPCSTR lpSubKey,
    LPCSTR lpValueName,
    DWORD dwType,
    LPCVOID lpData,
    DWORD cbData
    ) = RegSetKeyValueA; 
static LSTATUS(WINAPI* OldRegDeleteValueA)(HKEY hKey, LPCSTR lpValueName) = RegDeleteValueA;
static LSTATUS(WINAPI *OldRegSetValueExA)(HKEY hKey, LPCSTR lpValueName, DWORD Reserved, DWORD dwType, const BYTE* lpData, DWORD cbData) = RegSetValueExA;
static LSTATUS(WINAPI* OldRegSetValueA)(HKEY hKey, LPCSTR lpSubKey, DWORD dwType, LPCSTR lpData, DWORD cbData) = RegSetValueA;
//网络操作 socket send recv connect bind
static SOCKET(WINAPI* Oldsocket)(int af, int type, int protocol) = socket;
static int(WINAPI* Oldsend)(SOCKET s, const char* buf, int len, int flags) = send;
static int(WINAPI* Oldrecv)(SOCKET s, char* buf, int len, int flags) = recv;
static int(WINAPI* Oldconnect)(SOCKET s, const sockaddr* name, int namelen) = connect;
static int(WINAPI* Oldbind)(SOCKET s, const sockaddr* name, int namelen) = bind;
//memory copy
static void* (__cdecl* Oldmemcpy)(void* _Dst, const void* _Src, size_t _Size)  = memcpy;
static void* (__cdecl* Oldmemmove)(void* _Dst, const void* _Src, size_t _Size) = memmove;
/*
    declare for detouredFunc
*/
//MessageBoxA/W
extern "C" __declspec(dllexport)int WINAPI NewMessageBoxA(_In_opt_ HWND hwnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType);
extern "C" __declspec(dllexport)int WINAPI NewMessageBoxW(_In_opt_ HWND hwnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType);
//HeapCreat HeapDestroy HeapAlloc HeapFree
extern "C" __declspec(dllexport)HANDLE WINAPI NewHeapCreate(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize);
extern "C" __declspec(dllexport)BOOL WINAPI NewHeapDestroy(HANDLE hHeap);
extern "C" __declspec(dllexport)LPVOID WINAPI NewHeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes);
extern "C" __declspec(dllexport)BOOL WINAPI NewHeapFree(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem);
//CreateFileA WriteFile ReadFile _lclose CloseHandle
extern "C" __declspec(dllexport)HANDLE WINAPI NewCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
extern "C" __declspec(dllexport)HANDLE WINAPI NewCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
extern "C" __declspec(dllexport)BOOL WINAPI NewWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
extern "C" __declspec(dllexport)BOOL WINAPI NewReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
extern "C" __declspec(dllexport)BOOL WINAPI _Newlclose(HFILE hFile);
extern "C" __declspec(dllexport)BOOL WINAPI NewCloseHandle(HANDLE hObject);
extern "C" __declspec(dllexport)BOOL WINAPI NewCopyFileA(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL beFailIfExist);
//RegCreateKeyExA RegOpenKeyExA RegCloseKey RegQueryValueExA RegSetKeyValueA RegDeleteValueA
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegCreateKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD Reserved, LPSTR lpClass, DWORD dwOptions, REGSAM samDesired, LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition);
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegCreateKeyA(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult);
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegOpenKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkresult);
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegQueryValueA(HKEY hKey, LPCSTR lpsubKey, LPSTR lpData, PLONG lpcbData);
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegOpenKeyA(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult);
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegCloseKey(HKEY hKey);
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegDeleteKeyA(HKEY hKey, LPCSTR lpSubKey);
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegSetKeyValueA(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValueName, DWORD dwType, LPCVOID lpData, DWORD cbData);
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegDeleteValueA(HKEY hKey, LPCSTR lpValueName);
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegSetValueExA(HKEY hKey, LPCSTR lpValueName, DWORD Reserved, DWORD dwType, const BYTE * lpData, DWORD cbData);
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegSetValueA(HKEY hKey, LPCSTR lpSubKey, DWORD dwType, LPCSTR lpData, DWORD cbData);
//extern "C" __declspec(dllexport) = RegSetValueExA;
//socket send recv connect bind
extern "C" __declspec(dllexport)SOCKET WINAPI Newsocket(int af, int type, int protocol);
extern "C" __declspec(dllexport)int WINAPI Newsend(SOCKET s, const char* buf, int len, int flags);
extern "C" __declspec(dllexport)int WINAPI Newrecv(SOCKET s, char* buf, int len, int flags);
extern "C" __declspec(dllexport)int WINAPI Newconnect(SOCKET s, const sockaddr * name, int namelen);
extern "C" __declspec(dllexport)int WINAPI Newbind(SOCKET s, const sockaddr * name, int namelen);
//memcpy memove
extern "C" __declspec(dllexport)void* __cdecl Newmemcpy(void* _Dst, const void* _Src, size_t _Size);
extern "C" __declspec(dllexport)void* __cdecl Newmemmove(void* _Dst, const void* _Src, size_t _Size);
//main
BOOL WINAPI DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        EnableDetours();
    }
    break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
    {
        DetourTransactionBegin();
        DisableDetours();
        break;
    }
    }
    return TRUE;
}
/*
    function implemtation
*/
bool isTraced(HANDLE hHandle)
{
#ifndef NO_FILTER
    return HandleSet.find(((UINT)hHandle)) != HandleSet.end();
#endif // !NO_FILTER

#ifdef NO_FILTER
    return true;
#endif // NO_FILTER
}
int trace(HANDLE hHandle)
{
#ifndef NO_FILTER
    int alert = 0;
    if (HandleSet.find(((UINT)hHandle)) == HandleSet.end()) 
    {
        HandleSet.insert(((UINT)hHandle));
        return NOWARN;
    }
    else
        alert = ReCreate;
    return alert;
#endif // !NO_FILTER
#ifdef NO_FILTER
    return NOWARN;
#endif // NO_FILTER
}
int deTrace(HANDLE hHandle)
{
#ifndef NO_FILTER
    int alert = 0;
    auto iter = HandleSet.find(((UINT)hHandle));
    //printf("destroy: %08x , size: %d\n", hHandle,HandleSet.size());
    if (iter != HandleSet.end())
        HandleSet.erase(iter);//此处需要检查set erase的用法
    else alert = DeTraceW;
    return alert;
#endif // NO_FILTER

#ifdef NO_FILTER
    return NOWARN;
#endif // NO_FILTER
}
void EnableDetours()
{
    DetourAttach(&(PVOID&)OldMessageBoxA, NewMessageBoxA);
    DetourAttach(&(PVOID&)OldMessageBoxW, NewMessageBoxW);

    //堆操作 HeapCreat HeapDestroy HeapAlloc HeapFree
    DetourAttach(&(PVOID&)OldHeapCreate, NewHeapCreate);
    DetourAttach(&(PVOID&)OldHeapDestroy, NewHeapDestroy);
    DetourAttach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);
    DetourAttach(&(PVOID&)OldHeapFree, NewHeapFree);

    //文件操作 CreateFileA WriteFile ReadFile _lclose CloseHandle
    DetourAttach(&(PVOID&)OldCreateFileA, NewCreateFileA);
    DetourAttach(&(PVOID&)OldCreateFileW, NewCreateFileW);
    DetourAttach(&(PVOID&)OldWriteFile, NewWriteFile);
    DetourAttach(&(PVOID&)OldReadFile, NewReadFile);
    DetourAttach(&(PVOID&)OldCloseHandle, NewCloseHandle);
    DetourAttach(&(PVOID&)_Oldlclose, _Newlclose);
    DetourAttach(&(PVOID&)OldCopyFileA, NewCopyFileA);
    // 
    //注册表操作 RegCreateKeyExA RegOpenKeyExA RegCloseKey RegQueryValueExA RegSetKeyValueA RegDeleteValueA
    DetourAttach(&(PVOID&)OldRegCreateKeyExA, NewRegCreateKeyExA);
    DetourAttach(&(PVOID&)OldRegOpenKeyExA, NewRegOpenKeyExA);
    DetourAttach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);
    DetourAttach(&(PVOID&)OldRegDeleteKeyA, NewRegDeleteKeyA);
    DetourAttach(&(PVOID&)OldRegSetKeyValueA, NewRegSetKeyValueA);
    DetourAttach(&(PVOID&)OldRegSetValueExA, NewRegSetValueExA);
    DetourAttach(&(PVOID&)OldRegDeleteValueA, NewRegDeleteValueA);

    DetourAttach(&(PVOID&)OldRegCreateKeyA, NewRegCreateKeyA);
    DetourAttach(&(PVOID&)OldRegSetValueA, NewRegSetValueA);
    DetourAttach(&(PVOID&)OldRegQueryValueA, NewRegQueryValueA);
    DetourAttach(&(PVOID&)OldRegOpenKeyA, NewRegOpenKeyA);
    //网络操作 socket send recv connect bind
    DetourAttach(&(PVOID&)Oldsocket, Newsocket);
    DetourAttach(&(PVOID&)Oldsend, Newsend);
    DetourAttach(&(PVOID&)Oldrecv, Newrecv);
    DetourAttach(&(PVOID&)Oldconnect, Newconnect);
    DetourAttach(&(PVOID&)Oldbind, Newbind);

    //内存操作 memcpy memmove
    //DetourAttach(&(PVOID&)Oldmemcpy, Newmemcpy);
    //DetourAttach(&(PVOID&)Oldmemmove, Newmemmove);
    DetourTransactionCommit();
    return;
}
void DisableDetours()
{
    DetourDetach(&(PVOID&)OldMessageBoxA, NewMessageBoxA);
    DetourDetach(&(PVOID&)OldMessageBoxW, NewMessageBoxW);

    //堆操作 HeapCreat HeapDestroy HeapAlloc HeapFree
    DetourDetach(&(PVOID&)OldHeapCreate, NewHeapCreate);
    DetourDetach(&(PVOID&)OldHeapDestroy, NewHeapDestroy);
    DetourDetach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);
    DetourDetach(&(PVOID&)OldHeapFree, NewHeapFree);

    //文件操作 CreateFileA WriteFile ReadFile _lclose CloseHandle
    DetourDetach(&(PVOID&)OldCreateFileA, NewCreateFileA);
    DetourDetach(&(PVOID&)OldCreateFileW, NewCreateFileW);
    DetourDetach(&(PVOID&)OldWriteFile, NewWriteFile);
    DetourDetach(&(PVOID&)OldReadFile, NewReadFile);
    DetourDetach(&(PVOID&)OldCloseHandle, NewCloseHandle);
    DetourDetach(&(PVOID&)_Oldlclose, _Newlclose);
    DetourDetach(&(PVOID&)OldCopyFileA, NewCopyFileA);

    //注册表操作 RegCreateKeyExA RegOpenKeyExA RegCloseKey RegQueryValueExA RegSetKeyValueA RegDeleteValueA
    DetourDetach(&(PVOID&)OldRegCreateKeyExA, NewRegCreateKeyExA);//
    DetourDetach(&(PVOID&)OldRegOpenKeyExA, NewRegOpenKeyExA);
    DetourDetach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);//
    DetourDetach(&(PVOID&)OldRegDeleteKeyA, NewRegDeleteKeyA);//
    DetourDetach(&(PVOID&)OldRegSetKeyValueA, NewRegSetKeyValueA);//
    DetourDetach(&(PVOID&)OldRegSetValueExA, NewRegSetValueExA);//
    DetourDetach(&(PVOID&)OldRegDeleteValueA, NewRegDeleteValueA);//*

    DetourDetach(&(PVOID&)OldRegCreateKeyA, NewRegCreateKeyA);//
    DetourDetach(&(PVOID&)OldRegSetValueA, NewRegSetValueA);//
    DetourDetach(&(PVOID&)OldRegQueryValueA, NewRegQueryValueA);//
    DetourDetach(&(PVOID&)OldRegOpenKeyA, NewRegOpenKeyA);//
    //网络操作 socket send recv connect bind
    DetourDetach(&(PVOID&)Oldsocket, Newsocket);
    DetourDetach(&(PVOID&)Oldsend, Newsend);
    DetourDetach(&(PVOID&)Oldrecv, Newrecv);
    DetourDetach(&(PVOID&)Oldconnect, Newconnect);
    DetourDetach(&(PVOID&)Oldbind, Newbind);
    //内存操作
    //DetourDetach(&(PVOID&)Oldmemcpy, Newmemcpy);
    //DetourDetach(&(PVOID&)Oldmemmove, Newmemmove);
    DetourTransactionCommit();
    return;
}
//消息框操作
int WINAPI NewMessageBoxA(_In_opt_ HWND hwnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType)
{
#ifdef PRINTNAME
    printf("in NewMessageBoxA\n");
#endif // !PRINTNAME
    //TODO: 
    int ret = OldMessageBoxA(hwnd, lpText, lpCaption, uType);
    scalpel::sharedM::auto_buildHookedInfo(*pkt, MESSAGEBOX_A, 4, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "hwnd", 5);  *((int*)(pkt->param_val[0])) = (int)hwnd;
    memcpy(pkt->param_name[1], "lpText", 7);
    memcpy(pkt->param_name[2], "lpCaption", 10);
    memcpy(pkt->param_name[3], "uType", 6); *((UINT*)(pkt->param_val[3])) = uType;
    int lenp2 = 0, lenp3 = 0;
    if (lpText)      lenp2 = strlen(lpText);
    if (lpCaption)lenp3 = strlen(lpCaption);
    if (lenp2)
    {
        lenp2 = min(lenp2, MAX_VAL_LEN);
        memcpy(pkt->param_val[1], lpText, lenp2);
    }
    if (lenp3)
    {
        lenp3 = min(lenp3, MAX_VAL_LEN);
        memcpy(pkt->param_val[2], lpCaption, lenp3);
    }
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif // NO_SHARED_MEM
    return ret;
}
int WINAPI NewMessageBoxW(_In_opt_ HWND hwnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType)
{
#ifdef PRINTNAME
    printf("in NewMessageBoxW\n");
#endif // !PRINTNAME
    //TODO: 
    int ret = OldMessageBoxW(hwnd, lpText, lpCaption, uType);
    scalpel::sharedM::auto_buildHookedInfo(*pkt, MESSAGEBOX_W, 4, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "hwnd", 5);  *((int*)(pkt->param_val[0])) = (int)hwnd;
    memcpy(pkt->param_name[1], "lpText", 7);
    memcpy(pkt->param_name[2], "lpCaption", 10);
    memcpy(pkt->param_name[3], "uType", 6); *((UINT*)(pkt->param_val[3])) = uType;
    int lenp1 = 0, lenp2 = 0;
    if (lpText)      lenp1 = wcslen(lpText);
    if (lpCaption)lenp2 = wcslen(lpCaption);
    if (lenp1)
    {
        WideCharToMultiByte(CP_ACP, 0, lpText, lenp1, (char*)(pkt->param_val[1]), min(MAX_VAL_LEN,lenp1), NULL, NULL);
        lenp1 = -1;
    }
    if (lenp2)
    {
        WideCharToMultiByte(CP_ACP, 0, lpCaption, lenp2, (char*)(pkt->param_val[2]), min(MAX_VAL_LEN, lenp1), NULL, NULL);
        lenp2 = -1;
    }
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif // NO_SHARED_MEM
    return ret;
}
//堆操作 HeapCreat HeapDestroy HeapAlloc HeapFree
HANDLE WINAPI NewHeapCreate(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize)
{
#ifdef PRINTNAME
    std::cout << "in NewHeapCreate" << std::endl;
#endif // !PRINTNAME
    //TODO: 
    //printf("in heap create!\n");
    HANDLE hHeap = OldHeapCreate(fIOoptions, dwInitialSize, dwMaximumSize);
    int alert = trace(hHeap);
    scalpel::sharedM::auto_buildHookedInfo(*pkt, HEAPCREATE, 3, NOWARN, 0, (uint8_t*)(&hHeap));
    memcpy(pkt->param_name[0], "fIOoptions", 11);   *((DWORD*)(pkt->param_val[0])) = fIOoptions;
    memcpy(pkt->param_name[1], "dwInitialSize", 14); *((SIZE_T*)(pkt->param_val[1])) = dwInitialSize;
    memcpy(pkt->param_name[2], "dwMaximumSize", 14); *((SIZE_T*)(pkt->param_val[2])) = dwMaximumSize;
    if (alert) pkt->warn = alert;
    if (!hHeap)pkt->warn = GetLastError();
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    return hHeap;
}
BOOL WINAPI NewHeapDestroy(HANDLE hHeap)
{
#ifdef PRINTNAME
    std::cout << "in NewHeapDestroy" << std::endl;
#endif // !PRINTNAME
    //TODO: 
    BOOL ret = OldHeapDestroy(hHeap);
    int alert = deTrace(hHeap);
    scalpel::sharedM::auto_buildHookedInfo(*pkt, HEAPDESTROY, 1, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "hHeap", 5);   *((int*)(pkt->param_val[0])) = (int)hHeap;
    if (alert) pkt->warn = alert;
    if (!ret)pkt->warn = GetLastError();
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    return ret;
}
LPVOID WINAPI NewHeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes)
{
#ifdef PRINTNAME
    std::cout << "in NewHeapAlloc" << std::endl;
#endif // !PRINTNAME
    LPVOID ret = OldHeapAlloc(hHeap, dwFlags, dwBytes);
    if (!isTraced(hHeap))return ret;
    scalpel::sharedM::auto_buildHookedInfo(*pkt, HEAPALLOC, 3, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "hHeap", 5);   *((int*)(pkt->param_val[0])) = (int)hHeap;
    memcpy(pkt->param_name[1], "dwFlags", 8); *((DWORD*)(pkt->param_val[1])) = dwFlags;
    memcpy(pkt->param_name[2], "dwBytes", 8); *((DWORD*)(pkt->param_val[2])) = dwBytes;
    if (!ret)  pkt->warn = GetLastError();
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    return ret;
}
BOOL WINAPI NewHeapFree(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem)
{
#ifdef PRINTNAME
    std::cout << "in NewHeapFree" << std::endl;
#endif // !PRINTNAME
    BOOL ret = true;
    if (!isTraced(hHeap))return OldHeapFree(hHeap, dwFlags, lpMem);
    scalpel::sharedM::auto_buildHookedInfo(*pkt, HEAPFREE, 3, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "hHeap", 5);   *((int*)(pkt->param_val[0])) = (int)hHeap;
    memcpy(pkt->param_name[1], "dwFlags", 8); *((DWORD*)(pkt->param_val[1])) = dwFlags;
    memcpy(pkt->param_name[2], "lpMem", 6); *((int*)(pkt->param_val[2])) = (int)lpMem;
    //*((int*)pkt->ret_val) = (int)hHeap;
    if (!ret)  pkt->warn = GetLastError();
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    return OldHeapFree(hHeap, dwFlags, lpMem);
}
//文件操作
HANDLE WINAPI NewCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
#ifdef PRINTNAME
    printf("in NewCreateFileA\n");
#endif // !PRINTNAME
    HANDLE hFile = OldCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    if (GetFileType(hFile) != FILE_TYPE_DISK)return hFile;
   
    scalpel::sharedM::auto_buildHookedInfo(*pkt, CREATEFILE_A, 7, NOWARN, 0, (uint8_t*)(&hFile));
    memcpy(pkt->param_name[0], "lpFileName", 11);
    memcpy(pkt->param_name[1], "dwDesiredAccess", 16); *((DWORD*)(pkt->param_val[1])) = dwDesiredAccess;
    memcpy(pkt->param_name[2], "dwShareMode", 12); *((DWORD*)(pkt->param_val[2])) = dwShareMode;
    memcpy(pkt->param_name[3], "lpSecurityAttributes", 21);   *((int*)(pkt->param_val[3])) = (int)lpSecurityAttributes;
    memcpy(pkt->param_name[4], "dwCreationDisposition", 22); *((DWORD*)(pkt->param_val[4])) = dwCreationDisposition;
    memcpy(pkt->param_name[5], "dwFlagsAndAttributes", 21); *((DWORD*)(pkt->param_val[5])) = dwFlagsAndAttributes;
    memcpy(pkt->param_name[6], "hTemplateFile", 14); *((int*)(pkt->param_val[6])) = (int)hTemplateFile;
    int lenp0 = 0, alert = trace(hFile);
    if (lpFileName) lenp0 = strlen(lpFileName);
    if (lenp0)
    {
        lenp0 = min(lenp0, MAX_VAL_LEN);
        memcpy(pkt->param_val[0], lpFileName, lenp0);
    }
    if (hFile == INVALID_HANDLE_VALUE)pkt->warn = GetLastError();
    else if (alert)pkt->warn = alert;
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    return hFile;
}
HANDLE WINAPI NewCreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
#ifdef PRINTNAME
    printf("in NewCreateFileW\n");
#endif // !PRINTNAME
    HANDLE hFile = OldCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    if (GetFileType(hFile) != FILE_TYPE_DISK) { return hFile; }
    scalpel::sharedM::auto_buildHookedInfo(*pkt, CREATEFILE_W, 7, NOWARN, 0, (uint8_t*)(&hFile));
    memcpy(pkt->param_name[0], "lpFileName", 11);
    memcpy(pkt->param_name[1], "dwDesiredAccess", 16); *((DWORD*)(pkt->param_val[1])) = dwDesiredAccess;
    memcpy(pkt->param_name[2], "dwShareMode", 12); *((DWORD*)(pkt->param_val[2])) = dwShareMode;
    memcpy(pkt->param_name[3], "lpSecurityAttributes", 21);   *((int*)(pkt->param_val[3])) = (int)lpSecurityAttributes;
    memcpy(pkt->param_name[4], "dwCreationDisposition", 22); *((DWORD*)(pkt->param_val[4])) = dwCreationDisposition;
    memcpy(pkt->param_name[5], "dwFlagsAndAttributes", 21); *((DWORD*)(pkt->param_val[5])) = dwFlagsAndAttributes;
    memcpy(pkt->param_name[6], "hTemplateFile", 14); *((int*)(pkt->param_val[6])) = (int)hTemplateFile;
    int lenp0 = 0, alert = trace(hFile);
    if (lpFileName) lenp0 = wcslen(lpFileName);
    if (lenp0 )
    {
        WideCharToMultiByte(CP_ACP, 0, lpFileName, lenp0, (char*)pkt->param_val[0], min(MAX_VAL_LEN,lenp0), NULL, NULL);
    }
    if (hFile == INVALID_HANDLE_VALUE)pkt->warn = GetLastError();
    else if (alert)pkt->warn = alert;
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    return hFile;
}
BOOL WINAPI NewWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
#ifdef PRINTNAME
    printf("in NewWriteFile\n");
#endif // !PRINTNAME
    BOOL ret = OldWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
    if (!isTraced(hFile) || GetFileType(hFile) != FILE_TYPE_DISK) return ret;
    scalpel::sharedM::auto_buildHookedInfo(*pkt, WRITEFILE, 5, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "hFile", 6); *((int*)(pkt->param_val[0])) = (int)hFile;
    memcpy(pkt->param_name[1], "lpBuffer", 9);
    memcpy(pkt->param_name[2], "nNumberOfBytesToWrite", 22); *((DWORD*)(pkt->param_val[2])) = nNumberOfBytesToWrite;
    memcpy(pkt->param_name[3], "lpNumberOfBytesWritten", 23);   *((int*)(pkt->param_val[3])) = (int)lpNumberOfBytesWritten;
    memcpy(pkt->param_name[4], "lpOverlapped", 13); *((DWORD*)(pkt->param_val[4])) = (int)lpOverlapped;
    int lenp1 = 0;
    if (lpBuffer) lenp1 = strlen((const char*)lpBuffer);
    if (lenp1)
    {
        memcpy(pkt->param_val[1], lpBuffer, min(lenp1,MAX_VAL_LEN));
    }
    if (hFile == INVALID_HANDLE_VALUE)pkt->warn = GetLastError();
    if (!ret)pkt->warn = GetLastError();
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    return ret;
}
BOOL WINAPI NewReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
#ifdef PRINTNAME
    printf("in NewReadFile\n");
#endif // !PRINTNAME
    BOOL ret = OldReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
    if ((!isTraced(hFile)) || GetFileType(hFile) != FILE_TYPE_DISK) return ret;
    scalpel::sharedM::auto_buildHookedInfo(*pkt, READFILE, 5, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "hFile", 6); *((int*)(pkt->param_val[0])) = (int)hFile;
    memcpy(pkt->param_name[1], "lpBuffer", 9);
    memcpy(pkt->param_name[2], "nNumberOfBytesToRead", 21); *((DWORD*)(pkt->param_val[2])) = nNumberOfBytesToRead;
    memcpy(pkt->param_name[3], "lpNumberOfBytesRead", 20);   *((int*)(pkt->param_val[3])) = (int)lpNumberOfBytesRead;
    memcpy(pkt->param_name[4], "lpOverlapped", 13); *((DWORD*)(pkt->param_val[4])) = (int)lpOverlapped;
    int lenp1 = 0;
    if (lpBuffer) lenp1 = strlen((const char*)lpBuffer);
    if (lenp1)
    {
        memcpy(pkt->param_val[1], lpBuffer, min(lenp1,MAX_VAL_LEN));
    }
    if (hFile == INVALID_HANDLE_VALUE)pkt->warn = GetLastError();
    if (!ret)pkt->warn = GetLastError();
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    return ret;
}
BOOL WINAPI _Newlclose(HFILE hFile)
{
#ifdef PRINTNAME
    printf("in _Newlclose\n");
#endif // !PRINTNAME
    BOOL ret = _Oldlclose(hFile);
    scalpel::sharedM::auto_buildHookedInfo(*pkt, LCLOSE, 1, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "hFile", 6); *((int*)(pkt->param_val[0])) = hFile;
    if (!ret)pkt->warn = GetLastError();
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    return ret;
}
BOOL WINAPI NewCloseHandle(HANDLE hObject)
{
#ifdef PRINTNAME
    printf("in NewCloseHandle\n");
#endif // !PRINTNAME
    BOOL ret = OldCloseHandle(hObject);
    if (!isTraced(hObject))return ret;
    deTrace(hObject);
    scalpel::sharedM::auto_buildHookedInfo(*pkt, CLOSEHANDLE, 1, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "hObject", 8); *((int*)(pkt->param_val[0])) = (int)hObject;
    if (!ret)pkt->warn = GetLastError();
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    return ret;
}
BOOL WINAPI NewCopyFileA(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL beFailIfExist)//可中断
{
#ifdef PRINTNAME
    printf("in NewCopyFileA\n");
#endif // !PRINTNAME
    BOOL ret = false;
    //if (GetFileType(hFile) != FILE_TYPE_DISK) return ret;
    scalpel::sharedM::auto_buildHookedInfo(*pkt, COPYFILE_A, 3, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "lpExistingFileName", 19); 
    memcpy(pkt->param_name[1], "lpNewFileName", 14);
    memcpy(pkt->param_name[2], "beFailIfExist", 14); *((BOOL*)(pkt->param_val[2])) = beFailIfExist;
    int lenp0 = 0,lenp1 = 0;
    if (lpExistingFileName) lenp0 = strlen(lpExistingFileName);
    if (lenp0)
    {
        lenp0 = min(lenp0, MAX_VAL_LEN);
        memcpy(pkt->param_val[0], lpExistingFileName, lenp0);
    }
    if (lpNewFileName) lenp1 = strlen(lpNewFileName);
    {
        lenp1 = min(lenp1, MAX_VAL_LEN);
        memcpy(pkt->param_val[1], lpNewFileName, lenp1);
    }
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    ret = OldCopyFileA(lpExistingFileName, lpNewFileName, beFailIfExist);
    return ret;
}
//注册表操作
LSTATUS WINAPI NewRegCreateKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD Reserved, LPSTR lpClass, DWORD dwOptions, REGSAM samDesired, LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition)
{
#ifdef PRINTNAME
    printf("in NewRegCreateKeyExA\n");
#endif // !PRINTNAME
    LSTATUS ret = OldRegCreateKeyExA(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
    scalpel::sharedM::auto_buildHookedInfo(*pkt, REGCREATEKEYEX_A, 9, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "hKey", 5);     *((int*)(pkt->param_val[0])) = (int)hKey;
    memcpy(pkt->param_name[1], "lpSubKey", 9);
    memcpy(pkt->param_name[2], "Reserved", 9); *((DWORD*)(pkt->param_val[2])) = Reserved;
    memcpy(pkt->param_name[3], "lpClass", 8);
    memcpy(pkt->param_name[4], "dwOptions", 10); *((DWORD*)(pkt->param_val[4])) = dwOptions;
    memcpy(pkt->param_name[5], "samDesired", 11); *((DWORD*)(pkt->param_val[5])) = samDesired;
    memcpy(pkt->param_name[6], "lpSecurityAttributes", 21); *((int*)(pkt->param_val[6])) = (int)lpSecurityAttributes;
    memcpy(pkt->param_name[7], "phkResult", 10); *((int*)(pkt->param_val[7])) = (int)phkResult;
    memcpy(pkt->param_name[8], "lpdwDisposition", 16); *((int*)(pkt->param_val[8])) = (int)lpdwDisposition;

    int lenp1 = 0, lenp3 = 0;
    if (lpSubKey) lenp1 = strlen(lpSubKey);
    if (lpClass) lenp3 = strlen(lpClass);
    if (lenp1)
    {
        memcpy(pkt->param_val[1], lpSubKey, min(lenp1,MAX_VAL_LEN));
    }
    if (lenp3)memcpy(pkt->param_val[3], lpClass, min(lenp3, MAX_VAL_LEN)); 
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    return ret;
}
LSTATUS WINAPI NewRegCreateKeyA(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult)
{
#ifdef PRINTNAME
    printf("in NewRegCreateKeyExA\n");
#endif // !PRINTNAME
    LSTATUS ret = OldRegCreateKeyA(hKey, lpSubKey, phkResult);
    scalpel::sharedM::auto_buildHookedInfo(*pkt, REGCREATEKEY_A, 3, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "hKey", 5);     *((UINT*)(pkt->param_val[0])) = (UINT)hKey;
    memcpy(pkt->param_name[1], "lpSubKey", 9);
    memcpy(pkt->param_name[2], "phkResult", 10); *((UINT*)(pkt->param_val[2])) = (UINT)phkResult;
    int lenp1 = 0;
    if (lpSubKey) lenp1 = strlen(lpSubKey);
    if (lenp1)
    {
        memcpy(pkt->param_val[1], lpSubKey, min(lenp1, MAX_VAL_LEN));
    }
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    return ret;
}
LSTATUS WINAPI NewRegOpenKeyExA(HKEY hKey, LPCSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkresult)
{
#ifdef PRINTNAME
    printf("in NewRegOpenKeyExA\n");
#endif // !PRINTNAME
    LSTATUS ret = OldRegOpenKeyExA(hKey, lpSubKey, ulOptions, samDesired, phkresult);
    //DisableDetours();
    scalpel::sharedM::auto_buildHookedInfo(*pkt, REGOPENKEYEX_A, 5, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "hKey", 5);     *((UINT*)(pkt->param_val[0])) = (UINT)hKey;
    memcpy(pkt->param_name[1], "lpSubKey", 9);
    memcpy(pkt->param_name[2], "ulOptions", 10);     *((DWORD*)(pkt->param_val[2])) = ulOptions;
    memcpy(pkt->param_name[3], "samDesired", 11);   *((DWORD*)(pkt->param_val[3])) = samDesired;
    memcpy(pkt->param_name[4], "phkresult", 10); *((UINT*)(pkt->param_val[4])) = (UINT)phkresult;

    int lenp1 = 0, alert = 0;
    if (lpSubKey) lenp1 = strlen(lpSubKey);
    if (lenp1)
    {
        memcpy(pkt->param_val[1], lpSubKey, min(lenp1, MAX_VAL_LEN));
    }
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    //EnableDetours();
    return ret;
}
LSTATUS WINAPI NewRegOpenKeyA(HKEY hKey, LPCSTR lpSubKey, PHKEY phkResult)
{
#ifdef PRINTNAME
    cout << "in NewRegOpenKeyA" << endl;
#endif // !PRINTNAME
    LSTATUS ret = OldRegOpenKeyA(hKey, lpSubKey, phkResult);
    //DisableDetours();
    scalpel::sharedM::auto_buildHookedInfo(*pkt, REGOPENKEY_A, 3, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "hKey", 5);     *((UINT*)(pkt->param_val[0])) = (UINT)hKey;
    memcpy(pkt->param_name[1], "lpSubKey", 9);
    memcpy(pkt->param_name[2], "phkResult", 10);     *((UINT*)(pkt->param_val[2])) = (UINT)phkResult;

    int lenp1 = 0;
    if (lpSubKey) lenp1 = strlen(lpSubKey);
    if (lenp1)
    {
        memcpy(pkt->param_val[1], lpSubKey, min(lenp1, MAX_VAL_LEN));
    }
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    //EnableDetours();
    return ret;
}
LSTATUS WINAPI NewRegCloseKey(HKEY hKey)
{
#ifdef PRINTNAME
    printf("in NewRegCloseKey\n");
#endif // !PRINTNAME
    LSTATUS ret = OldRegCloseKey(hKey);
    //DisableDetours();
    scalpel::sharedM::auto_buildHookedInfo(*pkt, REGCLOSEKEY, 1, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "hKey", 5);     *((int*)(pkt->param_val[0])) = (int)hKey;
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    //EnableDetours();
    return ret;
}
LSTATUS WINAPI NewRegDeleteKeyA(HKEY hKey, LPCSTR lpSubKey)
{
#ifdef PRINTNAME
    printf("in NewRegDeleteA\n");
#endif // !PRINTNAME
    LSTATUS ret = OldRegDeleteKeyA(hKey, lpSubKey);
    //DisableDetours();
    scalpel::sharedM::auto_buildHookedInfo(*pkt, REGDELETEKEY_A, 2, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "hKey", 5);     *((int*)(pkt->param_val[0])) = (int)hKey;
    memcpy(pkt->param_name[1], "lpSubKey", 9);
    int lenp1 = 0;
    if (lpSubKey) lenp1 = strlen(lpSubKey);
    if (lenp1)memcpy(pkt->param_val[1], lpSubKey, min(lenp1, MAX_VAL_LEN));
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    //EnableDetours();
    return ret;
}
LSTATUS WINAPI NewRegQueryValueA(HKEY hKey, LPCSTR lpsubKey, LPSTR lpData, PLONG lpcbData)
{
#ifdef PRINTNAME
    printf("in NewRegQueryValueA\n");
#endif // !PRINTNAME
    LSTATUS ret = OldRegQueryValueA(hKey, lpsubKey, lpData, lpcbData);
    //DisableDetours();
    scalpel::sharedM::auto_buildHookedInfo(*pkt, REGQUERYVALUE_A, 4, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "hKey", 5);     *((int*)(pkt->param_val[0])) = (int)hKey;
    memcpy(pkt->param_name[1], "lpsubKey", 9);
    memcpy(pkt->param_name[2], "lpData", 7); 
    memcpy(pkt->param_name[3], "lpcbData", 9); 
        if (lpcbData) *((LONG*)(pkt->param_val[3])) = *lpcbData;
        else *((PLONG*)(pkt->param_val[3])) = lpcbData;
    int lenp1 = 0, lenp2 = 0;
    if (lpsubKey) lenp1 = strlen(lpsubKey);
    if (lpcbData) lenp2 = *lpcbData;
    if (lenp1)memcpy(pkt->param_val[1], lpsubKey, min(lenp1, MAX_VAL_LEN));
    if (lenp2)memcpy(pkt->param_val[2], lpData,   min(lenp2, MAX_VAL_LEN));
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    //EnableDetours();
    return ret;
}
LSTATUS WINAPI NewRegSetKeyValueA(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValueName, DWORD dwType, LPCVOID lpData, DWORD cbData)
{
#ifdef PRINTNAME
    printf("in NewRegSetKeyValueA\n");
#endif // !PRINTNAME
    LSTATUS ret = OldRegSetKeyValueA(hKey, lpSubKey, lpValueName, dwType, lpData, cbData);
    scalpel::sharedM::auto_buildHookedInfo(*pkt, REGSETKEYVALUE_A, 6, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "hKey", 5);     *((int*)(pkt->param_val[0])) = (int)hKey;
    memcpy(pkt->param_name[1], "lpSubKey", 9);
    memcpy(pkt->param_name[2], "lpValueName", 12);
    memcpy(pkt->param_name[3], "dwType", 7);    *((DWORD*)(pkt->param_val[3])) = dwType;
    memcpy(pkt->param_name[4], "lpData", 7);
    memcpy(pkt->param_name[5], "cbData", 7); *((DWORD*)(pkt->param_val[5])) = cbData;

    int lenp1 = 0, lenp2 = 0, lenp4 = 0;
    if (lpSubKey)
    {
        lenp1 = min(strlen(lpSubKey), MAX_VAL_LEN);
        memcpy(pkt->param_val[1], lpSubKey, lenp1);
    }
    if (lpValueName)
    {
        lenp2 = min(strlen(lpValueName), MAX_VAL_LEN);
        memcpy(pkt->param_val[2], lpValueName, lenp2);
    }
    if (lpData && cbData)
    {
        lenp4 = min(cbData, MAX_VAL_LEN);
        memcpy(pkt->param_val[4], lpData, lenp4);
    }
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    return ret;
}
LSTATUS WINAPI NewRegDeleteValueA(HKEY hKey, LPCSTR lpValueName)
{
#ifdef PRINTNAME
    printf("in NewRegDeleteValueA\n");
#endif // !PRINTNAME
    LSTATUS ret = OldRegDeleteValueA(hKey, lpValueName);
    //DisableDetours();
    scalpel::sharedM::auto_buildHookedInfo(*pkt, REGDELETEVALUE_A, 2, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "hKey", 5);     *((int*)(pkt->param_val[0])) = (int)hKey;
    memcpy(pkt->param_name[1], "lpValueName", 12);
    int lenp1 = 0;
    if (lpValueName)
    {
        lenp1 = min(strlen(lpValueName), MAX_VAL_LEN);
        memcpy(pkt->param_val[1], lpValueName, lenp1);
    }
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    //EnableDetours();
    return ret;
}
LSTATUS WINAPI NewRegSetValueExA(HKEY hKey, LPCSTR lpValueName, DWORD Reserved, DWORD dwType, const BYTE* lpData, DWORD cbData)
{
#ifdef PRINTNAME
    printf("in NewRegSetValueExA\n");
#endif // !PRINTNAME
    LSTATUS ret = OldRegSetValueExA(hKey, lpValueName,Reserved, dwType, lpData, cbData);
    scalpel::sharedM::auto_buildHookedInfo(*pkt, REGSETVALUEEX_A, 6, NOWARN, 0, (uint8_t*)(&ret));

    memcpy(pkt->param_name[0], "hKey", 5);     *((int*)(pkt->param_val[0])) = (int)hKey;
    memcpy(pkt->param_name[1], "lpValueName", 12);
    memcpy(pkt->param_name[2], "Reserved", 9); *((DWORD*)(pkt->param_val[2])) = Reserved;
    memcpy(pkt->param_name[3], "dwType", 7);    *((DWORD*)(pkt->param_val[3])) = dwType;
    memcpy(pkt->param_name[4], "lpData", 7);
    memcpy(pkt->param_name[5], "cbData", 7); *((DWORD*)(pkt->param_val[5])) = cbData;

    int lenp1 = 0, lenp4 = 0;
    if (lpValueName)
    {
        lenp1 = min(strlen(lpValueName), MAX_VAL_LEN);
        memcpy(pkt->param_val[1], lpValueName, lenp1);
    }
    if (lpData && cbData)
    {
        lenp4 = min(cbData, MAX_VAL_LEN);
        memcpy(pkt->param_val[4], lpData, lenp4);
        printf("lpData: %s\n", pkt->param_val[4]);
    }
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    return ret;
}
LSTATUS WINAPI NewRegSetValueA(HKEY hKey, LPCSTR lpSubKey, DWORD dwType, LPCSTR lpData, DWORD cbData)
{
#ifdef PRINTNAME
    printf("in NewRegSetValueA\n");
#endif // !PRINTNAME
    LSTATUS ret = OldRegSetValueA(hKey, lpSubKey, dwType, lpData, cbData);
    scalpel::sharedM::auto_buildHookedInfo(*pkt, REGSETVALUE_A, 5, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "hKey", 5);     *((UINT*)(pkt->param_val[0])) = (UINT)hKey;
    memcpy(pkt->param_name[1], "lpSubKey", 9);
    memcpy(pkt->param_name[2], "dwType", 7); *((DWORD*)(pkt->param_val[2])) = dwType;
    memcpy(pkt->param_name[3], "lpData", 7);    ;
    memcpy(pkt->param_name[4], "cbData", 7);    *((DWORD*)(pkt->param_val[4])) = cbData;

    int lenp1 = 0, lenp3 = 0;
    if (lpSubKey)
    {
        lenp1 = min(strlen(lpSubKey), MAX_VAL_LEN);
        memcpy(pkt->param_val[1], lpSubKey, lenp1);
    }
    if (cbData)
    {
        lenp3 = min(cbData, MAX_VAL_LEN);
        memcpy(pkt->param_val[3], lpData, lenp3);
        //printf("lpData: %s\n", pkt->param_val[3]);
    }
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    return ret;
}
//网络操作
SOCKET WINAPI Newsocket(int af, int type, int protocol)
{
#ifdef PRINTNAME
    printf("in Newsocket\n");
#endif // !PRINTNAME
    SOCKET ret = Oldsocket(af, type, protocol);
    scalpel::sharedM::auto_buildHookedInfo(*pkt, NEWSOCKET, 3, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "af", 3);       *((int*)(pkt->param_val[0])) = af;
    memcpy(pkt->param_name[1], "type", 5);     *((int*)(pkt->param_val[1])) = type;
    memcpy(pkt->param_name[2], "protocol", 9); *((int*)(pkt->param_val[2])) = protocol;
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    return ret;
}
int WINAPI Newsend(SOCKET s, const char* buf, int len, int flags)
{
#ifdef PRINTNAME
    printf("in Newsend\n");
#endif // !PRINTNAME
    int ret = Oldsend(s, buf, len, flags);
    scalpel::sharedM::auto_buildHookedInfo(*pkt, NEWSEND, 3, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "s", 2);       *((int*)(pkt->param_val[0])) = (int)s;
    memcpy(pkt->param_name[1], "buf", 4);   *((int*)(pkt->param_val[1])) = (int)buf;
    memcpy(pkt->param_name[2], "flags", 6); *((int*)(pkt->param_val[2])) = flags;
    int lenp1 = 0;
    if (buf)
    {
        lenp1 = min(strlen(buf), MAX_PAYLOAD_LEN);
        memcpy(pkt->payload, buf, lenp1);
        pkt->payload_len = lenp1;
    }
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    return ret;
}
int WINAPI Newrecv(SOCKET s, char* buf, int len, int flags)
{
#ifdef PRINTNAME
    printf("in Newrecv\n");
#endif // !PRINTNAME
    int ret = Oldrecv(s, buf, len, flags);
    scalpel::sharedM::auto_buildHookedInfo(*pkt, NEWRECV, 3, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "s", 2);     *((int*)(pkt->param_val[0])) = (int)s;
    memcpy(pkt->param_name[1], "buf", 4);   *((int*)(pkt->param_val[1])) = (int)buf;
    memcpy(pkt->param_name[2], "flags", 6); *((int*)(pkt->param_val[2])) = flags;
    int lenp1 = 0;
    if (buf)
    {
        lenp1 = min(strlen(buf), MAX_PAYLOAD_LEN);
        memcpy(pkt->payload, buf, lenp1);
        pkt->payload_len = lenp1;
    }
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    return ret;
}
int WINAPI Newconnect(SOCKET s, const sockaddr* name, int namelen)
{
    printf("in Newconnect\n");
    int ret = Oldconnect(s, name, namelen);
    sockaddr_in* addr_in = (sockaddr_in*)name;
    unsigned short port = ntohs(addr_in->sin_port);
    std::string ip = std::string(inet_ntoa(addr_in->sin_addr));
    scalpel::sharedM::auto_buildHookedInfo(*pkt, NEWCONNECT, 4, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "s", 2);       *((int*)(pkt->param_val[0])) = (int)s;
    memcpy(pkt->param_name[1], "ip", 3);   memcpy(pkt->param_val[1], ip.c_str(), ip.length());
    memcpy(pkt->param_name[2], "port", 5); *((unsigned short*)(pkt->param_val[2])) = port;
    memcpy(pkt->param_name[3], "namelen", 8); *((int*)(pkt->param_val[3])) = namelen;
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    return ret;
}
int WINAPI Newbind(SOCKET s, const sockaddr* name, int namelen)
{
#ifdef PRINTNAME
    printf("in Newbind\n");
#endif // !PRINTNAME
    int ret = Oldbind(s, name, namelen);
    sockaddr_in* addr_in = (sockaddr_in*)name;
    unsigned short port = ntohs(addr_in->sin_port);
    std::string ip = std::string(inet_ntoa(addr_in->sin_addr));
    scalpel::sharedM::auto_buildHookedInfo(*pkt, NEWBIND, 4, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "s", 2);       *((int*)(pkt->param_val[0])) = (int)s;
    memcpy(pkt->param_name[1], "ip", 3);   memcpy(pkt->param_val[1], ip.c_str(), ip.length());
    memcpy(pkt->param_name[2], "port", 5); *((unsigned short*)(pkt->param_val[2])) = port;
    memcpy(pkt->param_name[3], "namelen", 8); *((int*)(pkt->param_val[3])) = namelen;
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    return ret;
}
void* __cdecl Newmemcpy(void* _Dst, const void* _Src, size_t _Size)
{
#ifdef PRINTNAME
    printf("in Newmemcpy\n");
#endif // !PRINTNAME
    DisableDetours();
    void* ret = NULL;
    //scalpel::sharedM::auto_buildHookedInfo(*pkt, NEWMEMCPY, 3, NOWARN, 0, (uint8_t*)(&ret));
    scalpel::sharedM::manual_buildHookedInfo(*pkt, NEWMEMCPY, 3, NOWARN, 0);
    memcpy(pkt->param_name[0], "_Dst", 5); *((int*)(pkt->param_val[0])) = (int)_Dst;
    memcpy(pkt->param_name[1], "_Src", 5);
    memcpy(pkt->param_name[2], "_Size", 6); *((size_t*)(pkt->param_val[2])) = _Size;
    int len = min(MAX_VAL_LEN, _Size);
    if (len)memcpy(pkt->param_val[1], _Src, len);
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    ret = memcpy(_Dst, _Src, _Size);
    EnableDetours();
    return ret;
}
void* __cdecl Newmemmove(void* _Dst, const void* _Src, size_t _Size)
{
#ifdef PRINTNAME
    printf("in Newmemmove\n");
#endif // !PRINTNAME
    //DisableDetours();
    void* ret = Oldmemmove(_Dst, _Src, _Size);
    scalpel::sharedM::auto_buildHookedInfo(*pkt, NEWMEMMOVE, 3, NOWARN, 0, (uint8_t*)(&ret));
    memcpy(pkt->param_name[0], "_Dst", 5); *((int*)(pkt->param_val[0])) = (int)_Dst;     
    memcpy(pkt->param_name[1], "_Src", 5);
    memcpy(pkt->param_name[2], "_Size", 6); *((size_t*)(pkt->param_val[2])) = _Size;
    int len = min(MAX_VAL_LEN, _Size);
    if (len)memcpy(pkt->param_val[1], _Src, len);
#ifdef WITH_SHARED_MEM
    ReleaseSemaphore(hSemaphore, 1, NULL);
    while (WaitForSingleObject(hLockerSemaphore, 0) != WAIT_OBJECT_0);
#endif
    //EnableDetours();
    return ret;
}