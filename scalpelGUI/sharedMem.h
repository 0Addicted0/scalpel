#ifndef __SHARED_MEM__H_
#define __SHARED_MEM__H_
#include<stdio.h>
#include<stdint.h>
#include<windows.h>
#define SHARED_SEM_NAME "scalpel_s_sem"
#define SHARED_LOCKSEM_NAME "scalpel_s_lcoksem"
#define SHARED_MEM_NAME "scalpel_s_mem"
#define MAX_PARAM_NUM 10
#define MAX_PARAM_LEN 31
#define MAX_VAL_LEN 260
#define MAX_PAYLOAD_LEN 512
#define MAX_RET_LEN  4
// MessageBoxA/W
#define MESSAGEBOX_A 0
#define MESSAGEBOX_W 1
// HeapCreat HeapDestroy HeapAlloc HeapFree
#define HEAPCREATE 2
#define HEAPDESTROY 3
#define HEAPALLOC 4
#define HEAPFREE 5
// CreateFileA CreateFileW WriteFile ReadFile _lclose CloseHandle
#define CREATEFILE_A 6
#define CREATEFILE_W 7
#define WRITEFILE 8
#define READFILE 9
#define LCLOSE 10
#define CLOSEHANDLE 11
#define COPYFILE_A 12
// RegCreateKeyExA RegOpenKeyExA RegCloseKey RegQueryValueExA RegSetKeyValueA RegSetValueExA RegDeleteValueA 
#define REGCREATEKEYEX_A 13
#define REGOPENKEYEX_A 14
#define REGCLOSEKEY 15
#define REGQUERYVALUE_A 16
#define REGSETKEYVALUE_A 17
#define REGDELETEVALUE_A 18
#define REGSETVALUEEX_A 19
#define REGSETVALUE_A 20
#define REGDELETEKEY_A 21
#define REGOPENKEY_A 22
#define REGCREATEKEY_A 23
//socket send recv connect bind
#define NEWSOCKET 24
#define NEWSEND 25
#define NEWRECV 26
#define NEWCONNECT 27
#define NEWBIND 28
//memcpy memmove
#define NEWMEMCPY 29
#define NEWMEMMOVE 30
// unknown warn
#define ReCreate 0xFFFF//"重复申请了相同空间\n"
// unknown error
#define DeTraceW 0xFFFF+1//"取消不存在的跟踪\n"
#define UnknownedHeapHandle 0xFFFF+2
// normal
#define NOWARN 0
namespace scalpel
{
    namespace sharedM
    {
        struct hookedInfo
        {
            //uint8_t seperated;
            uint8_t operation;                               //name of hooked function 
            uint8_t param_num;
            uint8_t param_name[MAX_PARAM_NUM][MAX_PARAM_LEN + 1];//name of parameter 
            uint8_t param_val[MAX_PARAM_NUM][MAX_VAL_LEN + 1];  //value of parameter 
            DWORD warn;                                      //special warning
            SYSTEMTIME st;                                   //hook time
            uint8_t ret_val[MAX_RET_LEN];                    //return value of hooked function
            uint16_t payload_len;
            uint8_t payload[MAX_PAYLOAD_LEN + 1];                //file r/w and socket r/s infomation
        };

        inline void init_hookedInfo(hookedInfo& info)
        {
            info.operation = 0xff;
            info.warn = 0xff;
            memset(info.param_name, 0, MAX_PARAM_NUM * (MAX_PARAM_LEN + 1));
            memset(info.param_val, 0, MAX_PARAM_NUM * (MAX_VAL_LEN + 1));
            memset(info.ret_val, 0, MAX_RET_LEN);
            memset(info.payload, 0, (MAX_PAYLOAD_LEN + 1));
        }
        inline bool CreateSharedMem(size_t size, HANDLE& hSemaphore, HANDLE& hMapFile, LPVOID& lpBase)
        {
            hSemaphore = CreateSemaphoreA(NULL, 0, 1, SHARED_SEM_NAME);
            if (!hSemaphore)return false;
            hMapFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, SHARED_MEM_NAME);
            if (!hMapFile)return false;
            lpBase = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
            if (!lpBase)return false;
            return true;
        }
        inline bool CreateLocker(HANDLE& hSemaphore)
        {
            hSemaphore = CreateSemaphoreA(NULL, 0, 1, SHARED_LOCKSEM_NAME);
            if (!hSemaphore)return false;
            return true;
        }
        inline bool GetInfoFromSharedMem(HANDLE hSemaphore, DWORD dwMs_timel, LPVOID srcp, LPVOID dstp, size_t size)
        {
            if (WaitForSingleObject(hSemaphore, dwMs_timel) == WAIT_OBJECT_0)
            {
                /*
                *  Semaphore's val vary from 1 to 0
                */
                memcpy(dstp, srcp, size);
                /*
                * uncertain: whether to check info is pre info by st==lastst?
                */
                return true;
            }
            return false;
        }
        inline void auto_buildHookedInfo(hookedInfo& pkt, uint8_t operation, uint8_t param_num, uint8_t warn, uint16_t payload_len, uint8_t* ret_val)
        {
            //attention payload_data , param_name , param_val isn't build
            init_hookedInfo(pkt);
            GetLocalTime(&(pkt.st));
            pkt.operation = operation;
            pkt.param_num = param_num;
            pkt.warn = warn;
            pkt.payload_len = payload_len;
            memcpy(&(pkt.ret_val), ret_val, MAX_RET_LEN);
            return;
        }
        inline void manual_buildHookedInfo(hookedInfo& pkt, uint8_t operation, uint8_t param_num, uint8_t warn, uint16_t payload_len)
        {
            //attention payload_data , param_name , param_val isn't build
            init_hookedInfo(pkt);
            GetLocalTime(&(pkt.st));
            pkt.operation = operation;
            pkt.param_num = param_num;
            pkt.warn = warn;                //beacuse the val of warn depends on execute the function so it may be modified
            pkt.payload_len = payload_len;
            return;
        }
    }//sharedM
}//scalpel
#endif //__SHARED_MEM__
