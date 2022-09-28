# scalpel
simple windows API hooker

## framework
### GUI
1. main thread to response and refresh GUI
2. one thread as injector to startup detected procedure and analyse hooked data
3. one thread to get detected 
### DLL
1. hooker powered by [Detours](https://github.com/microsoft/Detours) -->[usage]()
