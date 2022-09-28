# scalpel
A simple windows API hooker

## framework
### GUI
1. main thread to response and refresh GUI powered by [Qt 5.14.0](https://www.zhihu.com/question/489217941/answer/2480213166)
2. one thread as injector to startup detected procedure and analyse hooked data
3. one thread to get detected message
### DLL
1. hooker powered by [Detours](https://github.com/microsoft/Detours) -->[usage](https://1.2.3.4)
### Tester
1. MessageBoxA/W
2. HeapCreat HeapDestroy HeapAlloc HeapFree
3. CreateFileA/W WriteFile ReadFile _lclose CloseHandle CopyFileA
4. RegCreateKeyExA RegCreateKeyA RegOpenKeyExA RegOpenKeyA RegCloseKey DeleteKeyA RegSetKeyValueA RegDeleteValueA RegSetValueExA RegSetValueA RegQueryValueA
5. socket send recv connect bind
## document organization
### [scalpelGUI](https://1.2.3.4) build the GUI
#### widget.h(.cpp)         --> to build the GUI
#### sharedMem.h            --> connect injector thread and detected procedure, define how to use the shared memory space
#### behaviorJudger.h(.cpp) --> analyse hooked message
#### others [MD5](https://github.com/JieweiWei/md5)
### [scalpelDLL](https://1.2.3.4) build the dll
#### dllmain.cpp            --> define the hooker
### [scalpel](https://1.2.3.4) build the Tester
#### scalpeTest.cpp         --> the test application
## reference
#### [Qt in VS2022](https://www.zhihu.com/question/489217941/answer/2480213166)
#### [IPC](https://blog.csdn.net/ypy_datou/article/details/122806141)
#### [releaseSemaphore and WaitForSingleObject](https://learn.microsoft.com/zh-cn/windows/win32/api/synchapi/nf-synchapi-releasesemaphore?redirectedfrom=MSDN)
#### [Qt primary](https://www.bilibili.com/video/BV1N34y1H7x7/?spm_id_from=333.337.search-card.all.click)
#### [Qt trick1]([https://www.likecs.com/show-305686727.html](https://blog.csdn.net/weixin_42887343/article/details/122344366))
#### [Qt trick2](https://www.likecs.com/show-305686727.html)
## simple diagram  
                         [_Dll_]  
                            |  
      [_GUI_]---->injector thread---->Process(detected procedure)  
         | main thread                   | (shared memory)  
         |                               |  
      judger  <--------------------------     
