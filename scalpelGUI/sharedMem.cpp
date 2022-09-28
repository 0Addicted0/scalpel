#include "sharedMem.h"
namespace  scalpel
{
    namespace  sharedM
    {
        inline void init_hookedInfo(hookedInfo &info)
        {
            info.operation = 0xff;
            info.warn = 0xff;
            memset(info.param_name,0,MAX_PARAM_NUM*(MAX_PARAM_LEN+1));
            memset(info.param_val,0,MAX_PARAM_NUM*(MAX_VAL_LEN+1));
            memset(info.ret_val,0,MAX_RET_LEN);
            memset(info.payload,0,(MAX_PAYLOAD_LEN+1));
            return;
        }

        inline bool CreateSharedMem(size_t size,HANDLE &hSemaphore,HANDLE &hMapFile,LPVOID &lpBase)
        {
            hSemaphore = CreateSemaphoreA(NULL,0,1,SHARED_MEM_NAME);
            if(!hSemaphore)return false;
            hMapFile = CreateFileMappingA(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,size,SHARED_MEM_NAME);
            if(!hMapFile)return false;
            lpBase = MapViewOfFile(hMapFile,FILE_MAP_ALL_ACCESS,0,0,0);
            if(!lpBase)return false;
            return true;
        }

        inline bool GetInfoFromSharedMem(HANDLE hSemaphore,DWORD dwMs_timel,LPVOID srcp,LPVOID dstp,size_t size)
        {
            if(WaitForSingleObject(hSemaphore,dwMs_timel) == WAIT_OBJECT_0)
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
        inline void auto_buildHookedInfo(hookedInfo &pkt,uint8_t operation,uint8_t param_num,uint8_t warn,uint16_t payload_len,uint8_t *ret_val)
        {
            //attention payload_data , param_name , param_val isn't build
            init_hookedInfo(pkt);
            GetLocalTime(&(pkt.st));
            pkt.operation = operation;
            pkt.param_num = param_num;
            pkt.warn = warn;
            pkt.payload_len = payload_len;
            memcpy(&(pkt.ret_val),ret_val,MAX_RET_LEN);
            return;
        }
        inline void manual_buildHookedInfo(hookedInfo &pkt,uint8_t operation,uint8_t param_num,uint8_t warn,uint16_t payload_len)
        {
            //attention payload_data , param_name , param_val isn't build
            GetLocalTime(&(pkt.st));
            pkt.operation = operation;
            pkt.param_num = param_num;
            pkt.warn = warn;                //beacuse the val of warn depends on execute the function so it may be modified
            pkt.payload_len = payload_len;
            return;
        }
    }
}
