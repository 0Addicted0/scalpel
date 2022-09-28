#ifndef _BEHAVIORJUDGE_
#define _BEHAVIORJUDGE_
#include <cstdio>
#include <Windows.h>
#include <iostream>
#include <Qstring>
#include <unordered_map>
#include <unordered_set>
#include "sharedMem.h"
#define SUFFIXLEN 32
#define MAX_CODE_CNT 128
namespace scalpel
{
	namespace judge
	{
		extern std::unordered_map<HANDLE, DWORD>fHandle2Access;
		extern std::unordered_set<HANDLE>exeHandle;
		extern int readItself, writeItself;
		extern HANDLE selfHandle;
		extern uint8_t selfCodePtr;
		extern std::string selfCode[MAX_CODE_CNT];
		extern bool CipherJudger(char* text);
		extern void initJudger();
		extern bool heapBehaviorJudge(sharedM::hookedInfo& pkt, QString& ret, int& iswarn);
		extern bool fileBehaviorJudge(sharedM::hookedInfo& pkt, QString injectedPath, QString injectedName, QString& ret, int& iswarn);
		extern bool regBehaviorJudge(sharedM::hookedInfo& pkt, QString& ret, int& iswarn);
		extern bool netBehaviorTrace(sharedM::hookedInfo& pkt, QString& ret, int& iswarn);
		extern bool memcpyBehaviorTrace(sharedM::hookedInfo& pkt, QString& ret, int& iswarn);
	}
}
#endif
