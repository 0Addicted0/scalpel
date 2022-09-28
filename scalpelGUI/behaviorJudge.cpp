#include "behaviorJudge.h"
#include "md5.h"
#include <qdebug.h>
#pragma execution_character_set("utf-8")  
namespace scalpel
{
	namespace judge
	{
		//open itself
		std::unordered_map<HANDLE, DWORD> fHandle2Access;
		std::unordered_set<HANDLE> exeHandle;
		std::unordered_set<UINT> heapSet;
		std::unordered_set<string>operationDir;
		int readItself, writeItself;
		HANDLE selfHandle;
		uint8_t selfCodePtr;
		std::string selfCode[MAX_CODE_CNT];
		char exesuffix[3][SUFFIXLEN] = { "exe","xco","lld" };
		bool isfirstDir;
		//inline int LCS(char *str1, char *str2)
		//{
		//	//str1.type
		//	return 0;
		//}
		bool CipherJudger(char* text)
		{
			int len = min(strlen(text), MAX_PAYLOAD_LEN);
			bool ret = false;
			for(int i=0;i<len;i++)
				if (*(text + i) <= 32|| *(text + i) >= 127)ret = true;
			return ret;
		}
		void initJudger()
		{
			readItself = writeItself = 0;
			fHandle2Access.clear();
			exeHandle.clear();
			heapSet.clear();
			operationDir.clear();
			selfCodePtr = 0;
			isfirstDir = true;
			return;
		}
		void path2Name(char* path, char* name)
		{
			//qDebug();
			char* tpath = path;
			int len = min(strlen(path), MAX_VAL_LEN);
			tpath = path + len - 1;
			len--;
			while (( * tpath != '/') && (*tpath) != '\\' && len)tpath--, len--;
			if (len)tpath++;
			while (*tpath)
			{
				*name = *tpath;
				name++, tpath++;
			}
			return;
		}
		bool isTraced(HANDLE f)
		{
			return fHandle2Access.find(f) != fHandle2Access.end();
		}
		void Trace(HANDLE f, DWORD acc)
		{
			fHandle2Access[f] = acc;
			return;
		}
		bool isexe(HANDLE f)
		{
			return exeHandle.find(f) != exeHandle.end();
		}
		void addexe(HANDLE f)
		{
			exeHandle.insert(f);
			return;
		}
		void removexe(HANDLE f)
		{
			exeHandle.erase(f);
			return;
		}
		bool DeTrace(HANDLE f)
		{
			auto it = fHandle2Access.find(f);
			if (it != fHandle2Access.end())
			{
				fHandle2Access.erase(it);
				if (isexe(f))removexe(f);
				return true;
			}
			else return false;
		}
		void allocMem(int memptr)
		{
			heapSet.insert(memptr);
			return;
		}
		bool freeMem(int memptr)
		{
			auto iter = heapSet.find(memptr);
			if (iter == heapSet.end())
			{
				return false;
			}
			heapSet.erase(iter);
			return true;
		}
		bool comp(string str)
		{
			//qDebug() << "in comp: " << (int)selfCodePtr << endl;
			bool ret = false;
			for (int i = 0; i < selfCodePtr; i++)
			{
				//std::cout << "str: " << str << "\n";
				//std::cout << "std: " << selfCode[i] << "\n";
				if (str == selfCode[i])
				{
					//qDebug() << "yes" << endl;
					ret = true;
					break;
				}
			}
			return ret;
		}
		void stripDoc(char* path, char* without)
		{
			char* s = path;
			int len = strlen(s);
			char *e= s + len - 1;
			int cnt = 0;
			while ((*e) != '\\' && (*e)!='/' && cnt<len)
			{
				e--;
				cnt++;
			}
			while (s < e)
			{
				*without = *s;
				s++; without++;
			};
			return;
		}
		bool getSuffixByName(char* dst_name)
		{
			char tmp[SUFFIXLEN] = { 0 };
			int len = min(strlen(dst_name), MAX_VAL_LEN);
			//qDebug() << "in func suffix: " << len<<"\n";
			char* tdst = dst_name + len - 1, * ttmp = tmp;
			while (*tdst != '.' && tdst >= dst_name && ttmp-tmp<=SUFFIXLEN)
			{
				//qDebug() << *tdst;
				*ttmp = *tdst;
				tdst--;
				ttmp++;
			}
			bool retv = false;
			//qDebug() << "\nin func suffix: " << tmp;
			for (int i = 0; i < 3; i++)
				if (!strcmp(tmp, exesuffix[i]))
				{
					retv = true;
					break;
				}
			return retv;
		}
		bool fileBehaviorJudge(sharedM::hookedInfo& pkt, QString injectedPath, QString injectedName, QString& ret, int &iswarn)
		{
			bool succeed = (*((HANDLE*)pkt.ret_val) != INVALID_HANDLE_VALUE);
			if (!succeed)return true;
			bool retv = true;
			QByteArray name_ba;
			QString q_dst_name;
			string dst_path;
			char dst_name[MAX_VAL_LEN + 1] = { 0 };
			char md5str[MAX_VAL_LEN + 1] = { 0 };
			char dst_suffix[MAX_VAL_LEN + 1] = { 0 };
			char path_withoutdoc[MAX_VAL_LEN + 1] = { 0 };
			//if (pkt.operation == COPYFILE_A)
			switch (pkt.operation)
			{
			case CREATEFILE_A:
			case CREATEFILE_W:
				name_ba = injectedName.toUtf8();
				//name_ba = injectedName.toLocal8Bit();
				path2Name((char *)(pkt.param_val[0]), dst_name);
				stripDoc((char*)(pkt.param_val[0]), path_withoutdoc);
				dst_path = MD5::MD5(path_withoutdoc).toStr();
				//qDebug() << "in createFileW" << endl;
				if (isfirstDir)
				{
					isfirstDir = false;
					operationDir.insert(dst_path);
				}
				else if(operationDir.find(dst_path)==operationDir.end())
				{
					operationDir.insert(dst_path);
					retv = false;
					ret = "提示: 程序操作了多个目录中的文件";
				}
				if (getSuffixByName(dst_name))
				{
					//qDebug() << "add exe" << endl;
					addexe(*((HANDLE*)pkt.ret_val));
				}
				if (!isTraced((*((HANDLE*)pkt.ret_val))))
				{
					Trace(*((HANDLE*)pkt.ret_val), *((DWORD*)(pkt.param_val[1])));
					//printf ("IN CREATEFILEW: add %08x\n",(UINT)* ((HANDLE*)pkt.ret_val));
					if (memcmp(name_ba.data(), dst_name, min(injectedName.length(), strlen(dst_name))) == 0)
					{
						//to read or write ifself
						selfHandle = *((HANDLE*)pkt.ret_val);
						if (*((DWORD*)pkt.param_val[1]) & GENERIC_READ)
						{
							readItself=1;
							retv = false;
							ret = "提示: 程序疑似存在自读操作";
						}
						if (*((DWORD*)pkt.param_val[1]) & GENERIC_WRITE)
						{
							writeItself=1;
							retv = false;
							ret = "提示: 程序疑似存在自写操作";
						}
					}
				}
				break;
			case READFILE:
				//printf("in fileBehaviorJudge: readFile: handle=%08x\n", (UINT) ( * ((HANDLE*)pkt.param_val[0])));
				if (isTraced(*((HANDLE*)pkt.param_val[0])))
				{
					if (*((HANDLE*)pkt.param_val[0]) == selfHandle && readItself)//may repulicate itself
					{
						retv = false;
						ret = "提示: 程序存在自读操作可能将自我复制";
					}
					DWORD len = *((DWORD*)pkt.param_val[2]);
					if (( * ((LPDWORD*)pkt.param_val[3])))
					{
						len = min(len, *((DWORD*)pkt.param_val[3]));
					}
					//ReadFile();
					memcpy(md5str, (char*)(pkt.param_val[1]), min(len, MAX_VAL_LEN));
					//qDebug() << "judger: [add]" << (int)len << " " << md5str << endl;
					selfCode[selfCodePtr++] = MD5::MD5(md5str).toStr();
					//std::cout << "judger: [add]" << selfCode[selfCodePtr-1] << "\n";
					selfCodePtr %= MAX_CODE_CNT;
				}
				else
				{
					retv = false;
					ret = "提示: 程序使用了未知句柄";
				}
				break;
			case WRITEFILE:
				if (isTraced(*((HANDLE*)pkt.param_val[0])))
				{
					if (readItself)//to open or write itself
					{
						//WriteFile
						DWORD len = *((DWORD*)pkt.param_val[2]);
						//ReadFile
						if ((DWORD*)(pkt.param_val[3]) && *((DWORD*)pkt.param_val[3]))
						{
							len = min(len, *((DWORD*)pkt.param_val[3]));
						}
						memcpy(md5str, (char*)(pkt.param_val[1]), min(len, MAX_VAL_LEN));
						string tmpstr = MD5::MD5(md5str).toStr();
						if (comp(tmpstr))
						{
							retv = false;
							iswarn = 1;
							ret = "警告: 程序可能进行自我复制";
						}
					}
					else if (isexe(*((HANDLE*)pkt.param_val[0])))//修改exe
					{
						retv = false;
						iswarn = 1;
						ret = "警告: 程序修改了可执行文件";
					}
				}
				else
				{
					retv = false;
					ret = "提示: 程序使用了未知句柄";// + QString::number(*((UINT*)pkt.param_val[0]));
				}
				break;
			case COPYFILE_A:
				path2Name((char *)(pkt.param_val[0]), dst_name);
				//q_dst_name = QString::fromLocal8Bit(dst_name);
				q_dst_name = QString::fromUtf8(dst_name);
				if (q_dst_name == injectedName)
				{
					retv = false;
					iswarn = 1;
					ret = "警告: 程序正在自我复制";
				}
				break;
			case CLOSEHANDLE:
				DeTrace(*((HANDLE*)pkt.param_val[0]));
				break;
			default:
				retv = false;
				ret = "提示: 未知调用";
				break;
			}
			return retv;
		}
		bool regBehaviorJudge(sharedM::hookedInfo& pkt, QString& ret, int& iswarn)
		{
			const char* lpSubKeyAutorun = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";//auro run
			const char* lpSubKeyForbid = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System";//ban regedit
			const char* lpSubKeyMainPage = "SOFTWARE\\Microsoft\\Internet Explorer\\Main\\Start Page";//modify main apge
			const char* lpSubKeyDefPage = "Software\\Microsoft\\Internet Explorer\\Main\\Default_Page_URL";//modify default apge
			const char* lpSubKeyTitle = "Software\\Microsoft\\Internet Explorer\\Main\\Window Title";//modify window title
			//const char* lpSubKeyLockPage = "Software\\Policies\\Microsoft\\Internet Explorer\\Control Panel";//lock settings
			const char* forbidName = "DisableRegistryTools";
			bool retv = true;
			char temp[MAX_VAL_LEN + 1] = { 0 };
			switch (pkt.operation)
			{
			case REGOPENKEYEX_A:
			case REGSETKEYVALUE_A:
			case REGSETVALUEEX_A:
			case REGSETVALUE_A:
				//RegSetValueA(,);
				//RegSetValueExA(,,);
				//RegSetKeyValueA(,,);
				//REGSETKEYVALUE_A(,);
				if (!strcmp((const char*)pkt.param_val[1], lpSubKeyAutorun))
				{
					retv = false;
					//strcpy(temp, (char*)pkt.param_val[3]);
					//QString tempstr = QString::fromLocal8Bit(temp);
					//QString tempstr = QString::fromUtf8(temp);
					iswarn = 1;
					ret = "警告: 程序试图添加启动项";
				}
				else if (!strcmp((const char*)pkt.param_val[1], lpSubKeyForbid))// && !strcmp((const char*)pkt.param_val[2], forbidName))
				{
					retv = false;
					iswarn = 1;
					ret = "警告: 程序试图锁定注册表";
				}
				else if (!strcmp((const char*)pkt.param_val[1], lpSubKeyMainPage))
				{
					retv = false;
					iswarn = 1;
					ret = "警告: 程序试图修改主页";
				}
				else if (!strcmp((const char*)pkt.param_val[1], lpSubKeyDefPage))
				{
					retv = false;
					iswarn = 1;
					ret = "警告: 程序试图修改默认主页";
				}
				else if (!strcmp((const char*)pkt.param_val[1], lpSubKeyTitle))
				{
					retv = false;
					iswarn = 1;
					ret = "警告: 程序试图修改标题栏";
				}
				else if (pkt.operation != REGOPENKEYEX_A)
				{
					retv = false;
					ret = "提示: 程序正在设置新键或修改键值值";
				}
				break;
			case REGDELETEVALUE_A:
			case REGDELETEKEY_A:
				strcpy(temp, (char*)pkt.param_val[1]);
				retv = false;
				iswarn = 1;
				ret = "警告: 程序正在删除键(" + QString::fromUtf8(temp) + ")";
				break;
			default:
				break;
			}
			return retv;
		}
		bool netBehaviorTrace(sharedM::hookedInfo& pkt, QString& ret, int& iswarn)
		{
			char temp[MAX_VAL_LEN + 1] = { 0 };
			string md5str;
			bool retv = true;
			int len = 0;
			switch (pkt.operation)
			{
			case NEWSEND:
				len = pkt.payload_len;
				memcpy(temp, (char*)(pkt.payload), min(len, MAX_VAL_LEN));
				//qDebug() << "judger: [get]" << (int)len << " " << temp << endl;
				md5str = MD5::MD5(temp).toStr();
				//std::cout << "judger: [get]" << md5str <<  "\n";
				if (comp(md5str))
				{
					retv = false;
					iswarn = 1;
					ret = "警告: 程序正在将本地文件发送到网络";
				}
				break;
			default:
				break;
			}
			return retv;
		}
		bool memcpyBehaviorTrace(sharedM::hookedInfo& pkt, QString& ret, int& iswarn)
		{
			return true;
		}
		bool heapBehaviorJudge(sharedM::hookedInfo& pkt, QString& ret, int& iswarn)
		{
			bool retv = true;
			if (pkt.operation == HEAPALLOC)
			{
				UINT tp = *((UINT *)pkt.ret_val);
				allocMem(tp);
			}
			else if (pkt.operation == HEAPFREE)
			{
				UINT tp = *((UINT*)pkt.param_val[2]);
				if (!freeMem(tp))
				{
					retv = false;
					iswarn = 1;
					ret = "警告: 二次释放堆上空间";
				}
			}
			else
			{
				switch (pkt.warn)
				{
				case ReCreate:
					ret = "提示: 重复创建相同堆";
					retv = false;
					break;
				case DeTraceW:
					ret = "提示: 重复销毁堆";
					retv = false;
					break;
				default:
					break;
				}
			}
			return retv;
		}
	}
}

