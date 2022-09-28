#include "md5.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
//#include <winsock2.h>
//#pragma execution_character_set("utf-8")  
#pragma comment(lib, "ws2_32.lib")  
#define PAGE_SIZE	4096
using namespace std;
void headRepeatedRelease();
void selfReplication();
void modifyStartupRegistry();
void openAnotherFolder();
void recvData();
void sendData(const char *str);
void sendFile(bool iEncrypt);
//void memoryOperation();
void showMenu()
{
	//printf("\n*************************************************************************************\n");
	printf("--------------------------------请输入对应序号--------------------------------\n");
	printf("1.基本操作\n");
	printf("\t1.消息框测试\n");
	printf("\t2.文件操作测试\n");
	printf("\t3.堆上操作测试\n");
	printf("\t4.注册表操作测试\n");
	printf("\t5.网络通信测试\n");
	printf("2.恶意操作\n");
	printf("\t1.恶意文件操作\n");
	printf("\t2.恶意修改注册表\n");
	printf("\t3.上传文件到网络\n");
	printf("\t4.二次释放\n");
	printf("3.退出程序\n");
	printf("------------------------------------------------------------------------------\n");
}
int main()
{
	int op_class;
	int op = 0;
	bool flag = true;
	//MessageBoxA(NULL, "Can not open the file", "Playwav", MB_OK);
	while (flag)
	{
		showMenu();
		scanf_s("%d%d", &op_class, &op);
		switch (op_class)
		{
		case 1://basic
		{
			if (op == 1)
			{
				printf("MessageBoxA 测试...\n");
				MessageBoxA(NULL, "I'm MessageBoxA", "I'm MessageBoxA's title", MB_OK);
				printf("MessageBoxA 测试结束\n");
				printf("MessageBoxW 测试...\n");
				MessageBoxW(NULL, L"I'm MessageBoxW", L"I'm MessageBoxW's title", MB_OK);
				printf("MessageBoxW 测试结束\n");
			}
			else if (op == 2)
			{
				getchar();
				const char* testSrc = "scalpel writeFile test!";
				printf("CreateFileA 测试...");
				getchar();
				HANDLE f1 = CreateFileA("scalpelTest.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
				printf("CreateFileA 测试结束\n\n");
				DWORD cbData = 0;
				printf("WriteFile 测试...");
				getchar();
				WriteFile(f1, testSrc, strlen(testSrc), &cbData, NULL);
				if (cbData)printf("写入的内容: %s\n", testSrc);
				printf("WriteFile 测试结束\n\n");
				FlushFileBuffers(f1);
				CloseHandle(f1);

				char buffer[128] = { 0 };
				cbData = 0;
				printf("ReadFile 测试...");
				getchar();
				HANDLE f2 = CreateFileA("scalpelTest.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
				DWORD fsize = GetFileSize(f2, NULL);
				if (ReadFile(f2, buffer, fsize, &cbData, NULL))
				{
					printf("读取的内容: %s\n", buffer);
				}
				printf("ReadFile 测试结束\n\n");
				printf("CloseHandle 测试...");
				getchar();
				CloseHandle(f2);
				printf("CloseHandle 测试结束\n\n");
			}
			else if (op == 3)
			{
				getchar();
				printf("HeapCreate 测试...");
				getchar();
				HANDLE hHeap = HeapCreate(HEAP_NO_SERIALIZE, PAGE_SIZE * 10, PAGE_SIZE * 100);
				printf("HeapCreate 测试完成\n\n");

				printf("HeapAlloc 测试...");
				getchar();
				int* pArr = (int*)HeapAlloc(hHeap, 0, sizeof(int) * 30);
				printf("HeapAlloc 测试完成\n\n");

				printf("HeapFree 测试...");
				getchar();
				HeapFree(hHeap, 0, pArr);
				printf("HeapFree 测试完成\n\n");

				printf("HeapDestroy 测试...");
				getchar();
				HeapDestroy(hHeap);
				printf("HeapDestroy 测试完成\n\n");
			}
			else if (op == 4)
			{
				getchar();
				HKEY hKey;
				size_t lRet;
				const char* keyData = "scalpelTestKey";
				printf("RegCreateKeyExA 测试...");
				getchar();
				lRet = RegCreateKeyExA(HKEY_CURRENT_USER, keyData, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
				if (lRet == ERROR_SUCCESS)
				{
					printf("RegCreateKeyExA 测试完成(创建键:HKEY_CURRENT_USER\\%s)\n\n", keyData);
				}
				else
				{
					printf("创建失败\n\n");
					break;
				}
				printf("RegSetValueExA 测试...");
				getchar();
				size_t iLen = strlen(keyData);
				const char* val = "scalpel";
				lRet = RegSetValueExA(hKey, "scalpelValue", 0, REG_SZ, (const BYTE*)val, sizeof(TCHAR) * iLen);
				if (lRet == ERROR_SUCCESS)
				{
					printf("RegSetValueExA 测试完成\n\n");
				}
				else
				{
					printf("设置失败\n\n");
					break;
				}

				printf("RegOpenKeyExA 测试...");
				getchar();
				lRet = RegOpenKeyExA(HKEY_CURRENT_USER, keyData, 0, KEY_ALL_ACCESS, &hKey);
				if (lRet == ERROR_SUCCESS) {
					printf("RegOpenKeyExA 测试完成\n\n");
				}
				else {
					printf("打开失败\n\n");
					break;
				}

				char lpData[128] = { 0 };
				LONG lpcbData;
				//RegQueryValueA(hKey, "", lpData ,&lpcbData);
				printf("RegDeleteValueA 测试...");
				getchar();
				lRet = RegDeleteValueA(hKey, "scalpelValue");
				if (lRet == ERROR_SUCCESS) {
					lRet = RegDeleteKeyA(HKEY_CURRENT_USER, keyData);
					printf("RegDeleteValueA 测试成功\n\n");
				}
				else {
					printf("删除失败\n\n");
					break;
				}

				printf("RegCloseKey 测试...");
				getchar();
				RegCloseKey(hKey);
				printf("RegCloseKey 测试成功\n\n");
			}
			else if (op == 5)
			{
				printf("\t\t1.发送信息\n");
				printf("\t\t2.接收信息\n");
				printf("\t\t3.简易分辨明密文\n");
				scanf_s("%d", &op);
				getchar();
				if (op == 1)
					sendData(NULL);
				else if (op == 2)
					recvData();
				else if (op == 3)
					sendFile(true);
			}
			break;
		}
		case 2://malicious
		{
			if (op == 1)
			{
				getchar();
				selfReplication();
				openAnotherFolder();
			}
			else if (op == 2)
			{
				getchar();
				modifyStartupRegistry();
			}
			else if (op == 3)
			{
				sendFile(false);
			}
			else if (op == 4)
			{
				getchar();
				printf("二次释放 测试\n");
				getchar();
				headRepeatedRelease();
				printf("二次释放 测试完成\n");
			}
			break;
		}
		default:
		{
			printf("bye!\n");
			flag = false;
			break;
		}
		}
	}
	return 0;
}
void headRepeatedRelease()
{

	HANDLE hHeap = HeapCreate(HEAP_NO_SERIALIZE, PAGE_SIZE * 10, PAGE_SIZE * 100);
	int* pArr = (int*)HeapAlloc(hHeap, 0, sizeof(int) * 30);
	for (int i = 0; i < 30; ++i)pArr[i] = i + 1;
	printf("first HeapFree!\n");
	HeapFree(hHeap, 0, pArr);
	printf("second HeapFree!\n");
	HeapFree(hHeap, 0, pArr);
	HeapDestroy(hHeap);
	return;
}
void selfReplication()
{
	char ExeFile[200];
	//得到当前文件路径名
	GetModuleFileNameA(NULL, ExeFile, 200);
	printf("自我复制 测试1...");
	getchar();
	//HMODULE hCurrent_Module = GetModuleHandle(NULL);
	//char selfName[_MAX_PATH + 1] = { 0 };
	
	//printf("name: %s\n", selfName);
	//return;
	CopyFileA(ExeFile, "testCode1.exe", false);
	printf("自我复制测试1 完成\n\n");

	printf("修改可执行文件测试...");
	getchar();
	HANDLE f1 = (HANDLE)CreateFileA("testCode1.exe", GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
	const char* text = "hello it's me, for replication";
	DWORD cbwriter = 0;
	WriteFile(f1, text, sizeof(text), &cbwriter, NULL);
	CloseHandle(f1);
	printf("修改可执行文件测试完成\n\n");


	printf("自我复制测试2...");
	getchar();
	HANDLE hSelf = (HANDLE)CreateFileA(ExeFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
	DWORD fileSize = GetFileSize(hSelf, NULL);
	DWORD dwBytesToRead = fileSize,dwBytesRead = 0;
	//ReadFile(hSelf, (LPVOID)buffer, strlen(testSrc), &cbData, NULL);
	char* tmpBuf = (char*)malloc(fileSize);
	ReadFile(hSelf, tmpBuf, dwBytesToRead, &dwBytesRead, NULL);
	HANDLE f2 = CreateFileA("testCode2.exe", GENERIC_WRITE, FILE_SHARE_READ, NULL,	CREATE_ALWAYS, NULL, NULL);
	WriteFile(f2, tmpBuf, dwBytesRead, &dwBytesToRead, NULL);
	CloseHandle(hSelf);
	CloseHandle(f2);
	printf("自我复制测试2 完成\n\n");
	return;
}
void modifyStartupRegistry()
{
	printf("启动项添加测试...");
	getchar();
	HKEY hKey = NULL;
	size_t lRet = RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, \
		KEY_READ, &hKey);
	const char* lpData = "";
	DWORD cbData = sizeof(lpData);
	lRet = RegSetKeyValueA(hKey, "startup","scalpelGUI.exe", REG_SZ, (const BYTE*)lpData, cbData);
	//lRet = RegSetValueA(hKey, "", REG_SZ, "scalpelGUI.exe", 15);
	if (lRet == ERROR_SUCCESS)
	{
		printf("启动项添加测试完成\n\n");
		RegDeleteKeyA(hKey, "startup");
	}
	else
	{
		printf("设置失败\n\n");
	}
	RegCloseKey(hKey);
}
void openAnotherFolder() {
	printf("操作多目录测试");
	getchar();
	HANDLE hOpenFile = (HANDLE)CreateFile(L"..\\testMultiFolder\\scalpelTest2.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	CloseHandle(hOpenFile);
	printf("操作多目录测试完成\n\n");
	return;
}
void recvData()
{
	printf("接收信息测试...");
	getchar();
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(1234);
	connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	Sleep(500);
	char szBuffer[MAXBYTE] = { 0 };
	recv(sock, szBuffer, MAXBYTE, NULL);
	printf("Message form server: %s\n", szBuffer);
	closesocket(sock);
	WSACleanup();
	printf("接收信息测试完成\n\n");
}
void sendData(const char *cont)
{
	if (!cont)
	{
		printf("发送信息测试...");
		getchar();
	}
	//初始化 DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//创建套接字
	SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	//绑定套接字
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
	sockAddr.sin_family = PF_INET;  //使用IPv4地址
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
	sockAddr.sin_port = htons(1234);  //端口
	bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	//进入监听状态
	listen(servSock, 20);
	//接收客户端请求
	SOCKADDR clntAddr;
	int nSize = sizeof(SOCKADDR);
	SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
	//向客户端发送数据
	char str[32] = "This is network test!";
	if (!cont)
	{
		send(clntSock, str, strlen(str) + sizeof(char), NULL);
	}
	else
	{
		send(clntSock, cont, strlen(cont) + sizeof(char), NULL);
	}
	//关闭套接字
	closesocket(clntSock);
	closesocket(servSock);
	//终止 DLL 的使用
	WSACleanup();
	if(!cont)
		printf("发送信息测试完成\n\n");
}
void sendFile(bool ifEncrypt)
{
	printf("发送文件测试...");
	getchar();
	const char* testSrc = "This is to disappear!";
	HANDLE f1 = CreateFileA("scalpelTest.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	DWORD cbData = 0;
	WriteFile(f1, testSrc, strlen(testSrc), &cbData, NULL);
	FlushFileBuffers(f1);
	CloseHandle(f1);
	char buffer[128] = { 0 };
	cbData = 0;
	HANDLE f2 = CreateFileA("scalpelTest.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
	DWORD fsize = GetFileSize(f2, NULL);
	ReadFile(f2, buffer, fsize, &cbData, NULL);
	CloseHandle(f2);
	if (ifEncrypt)
	{
		const byte* c = MD5::MD5(buffer).getDigest();
		sendData((const char*)c);
	}
	else
	{
		sendData(buffer);
	}
	printf("发送文件测试完成\n\n");
	return;
}