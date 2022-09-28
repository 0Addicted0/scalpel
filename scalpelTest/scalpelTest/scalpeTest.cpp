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
	printf("--------------------------------�������Ӧ���--------------------------------\n");
	printf("1.��������\n");
	printf("\t1.��Ϣ�����\n");
	printf("\t2.�ļ���������\n");
	printf("\t3.���ϲ�������\n");
	printf("\t4.ע����������\n");
	printf("\t5.����ͨ�Ų���\n");
	printf("2.�������\n");
	printf("\t1.�����ļ�����\n");
	printf("\t2.�����޸�ע���\n");
	printf("\t3.�ϴ��ļ�������\n");
	printf("\t4.�����ͷ�\n");
	printf("3.�˳�����\n");
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
				printf("MessageBoxA ����...\n");
				MessageBoxA(NULL, "I'm MessageBoxA", "I'm MessageBoxA's title", MB_OK);
				printf("MessageBoxA ���Խ���\n");
				printf("MessageBoxW ����...\n");
				MessageBoxW(NULL, L"I'm MessageBoxW", L"I'm MessageBoxW's title", MB_OK);
				printf("MessageBoxW ���Խ���\n");
			}
			else if (op == 2)
			{
				getchar();
				const char* testSrc = "scalpel writeFile test!";
				printf("CreateFileA ����...");
				getchar();
				HANDLE f1 = CreateFileA("scalpelTest.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
				printf("CreateFileA ���Խ���\n\n");
				DWORD cbData = 0;
				printf("WriteFile ����...");
				getchar();
				WriteFile(f1, testSrc, strlen(testSrc), &cbData, NULL);
				if (cbData)printf("д�������: %s\n", testSrc);
				printf("WriteFile ���Խ���\n\n");
				FlushFileBuffers(f1);
				CloseHandle(f1);

				char buffer[128] = { 0 };
				cbData = 0;
				printf("ReadFile ����...");
				getchar();
				HANDLE f2 = CreateFileA("scalpelTest.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
				DWORD fsize = GetFileSize(f2, NULL);
				if (ReadFile(f2, buffer, fsize, &cbData, NULL))
				{
					printf("��ȡ������: %s\n", buffer);
				}
				printf("ReadFile ���Խ���\n\n");
				printf("CloseHandle ����...");
				getchar();
				CloseHandle(f2);
				printf("CloseHandle ���Խ���\n\n");
			}
			else if (op == 3)
			{
				getchar();
				printf("HeapCreate ����...");
				getchar();
				HANDLE hHeap = HeapCreate(HEAP_NO_SERIALIZE, PAGE_SIZE * 10, PAGE_SIZE * 100);
				printf("HeapCreate �������\n\n");

				printf("HeapAlloc ����...");
				getchar();
				int* pArr = (int*)HeapAlloc(hHeap, 0, sizeof(int) * 30);
				printf("HeapAlloc �������\n\n");

				printf("HeapFree ����...");
				getchar();
				HeapFree(hHeap, 0, pArr);
				printf("HeapFree �������\n\n");

				printf("HeapDestroy ����...");
				getchar();
				HeapDestroy(hHeap);
				printf("HeapDestroy �������\n\n");
			}
			else if (op == 4)
			{
				getchar();
				HKEY hKey;
				size_t lRet;
				const char* keyData = "scalpelTestKey";
				printf("RegCreateKeyExA ����...");
				getchar();
				lRet = RegCreateKeyExA(HKEY_CURRENT_USER, keyData, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
				if (lRet == ERROR_SUCCESS)
				{
					printf("RegCreateKeyExA �������(������:HKEY_CURRENT_USER\\%s)\n\n", keyData);
				}
				else
				{
					printf("����ʧ��\n\n");
					break;
				}
				printf("RegSetValueExA ����...");
				getchar();
				size_t iLen = strlen(keyData);
				const char* val = "scalpel";
				lRet = RegSetValueExA(hKey, "scalpelValue", 0, REG_SZ, (const BYTE*)val, sizeof(TCHAR) * iLen);
				if (lRet == ERROR_SUCCESS)
				{
					printf("RegSetValueExA �������\n\n");
				}
				else
				{
					printf("����ʧ��\n\n");
					break;
				}

				printf("RegOpenKeyExA ����...");
				getchar();
				lRet = RegOpenKeyExA(HKEY_CURRENT_USER, keyData, 0, KEY_ALL_ACCESS, &hKey);
				if (lRet == ERROR_SUCCESS) {
					printf("RegOpenKeyExA �������\n\n");
				}
				else {
					printf("��ʧ��\n\n");
					break;
				}

				char lpData[128] = { 0 };
				LONG lpcbData;
				//RegQueryValueA(hKey, "", lpData ,&lpcbData);
				printf("RegDeleteValueA ����...");
				getchar();
				lRet = RegDeleteValueA(hKey, "scalpelValue");
				if (lRet == ERROR_SUCCESS) {
					lRet = RegDeleteKeyA(HKEY_CURRENT_USER, keyData);
					printf("RegDeleteValueA ���Գɹ�\n\n");
				}
				else {
					printf("ɾ��ʧ��\n\n");
					break;
				}

				printf("RegCloseKey ����...");
				getchar();
				RegCloseKey(hKey);
				printf("RegCloseKey ���Գɹ�\n\n");
			}
			else if (op == 5)
			{
				printf("\t\t1.������Ϣ\n");
				printf("\t\t2.������Ϣ\n");
				printf("\t\t3.���׷ֱ�������\n");
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
				printf("�����ͷ� ����\n");
				getchar();
				headRepeatedRelease();
				printf("�����ͷ� �������\n");
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
	//�õ���ǰ�ļ�·����
	GetModuleFileNameA(NULL, ExeFile, 200);
	printf("���Ҹ��� ����1...");
	getchar();
	//HMODULE hCurrent_Module = GetModuleHandle(NULL);
	//char selfName[_MAX_PATH + 1] = { 0 };
	
	//printf("name: %s\n", selfName);
	//return;
	CopyFileA(ExeFile, "testCode1.exe", false);
	printf("���Ҹ��Ʋ���1 ���\n\n");

	printf("�޸Ŀ�ִ���ļ�����...");
	getchar();
	HANDLE f1 = (HANDLE)CreateFileA("testCode1.exe", GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
	const char* text = "hello it's me, for replication";
	DWORD cbwriter = 0;
	WriteFile(f1, text, sizeof(text), &cbwriter, NULL);
	CloseHandle(f1);
	printf("�޸Ŀ�ִ���ļ��������\n\n");


	printf("���Ҹ��Ʋ���2...");
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
	printf("���Ҹ��Ʋ���2 ���\n\n");
	return;
}
void modifyStartupRegistry()
{
	printf("��������Ӳ���...");
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
		printf("��������Ӳ������\n\n");
		RegDeleteKeyA(hKey, "startup");
	}
	else
	{
		printf("����ʧ��\n\n");
	}
	RegCloseKey(hKey);
}
void openAnotherFolder() {
	printf("������Ŀ¼����");
	getchar();
	HANDLE hOpenFile = (HANDLE)CreateFile(L"..\\testMultiFolder\\scalpelTest2.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	CloseHandle(hOpenFile);
	printf("������Ŀ¼�������\n\n");
	return;
}
void recvData()
{
	printf("������Ϣ����...");
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
	printf("������Ϣ�������\n\n");
}
void sendData(const char *cont)
{
	if (!cont)
	{
		printf("������Ϣ����...");
		getchar();
	}
	//��ʼ�� DLL
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//�����׽���
	SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	//���׽���
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));  //ÿ���ֽڶ���0���
	sockAddr.sin_family = PF_INET;  //ʹ��IPv4��ַ
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //�����IP��ַ
	sockAddr.sin_port = htons(1234);  //�˿�
	bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	//�������״̬
	listen(servSock, 20);
	//���տͻ�������
	SOCKADDR clntAddr;
	int nSize = sizeof(SOCKADDR);
	SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
	//��ͻ��˷�������
	char str[32] = "This is network test!";
	if (!cont)
	{
		send(clntSock, str, strlen(str) + sizeof(char), NULL);
	}
	else
	{
		send(clntSock, cont, strlen(cont) + sizeof(char), NULL);
	}
	//�ر��׽���
	closesocket(clntSock);
	closesocket(servSock);
	//��ֹ DLL ��ʹ��
	WSACleanup();
	if(!cont)
		printf("������Ϣ�������\n\n");
}
void sendFile(bool ifEncrypt)
{
	printf("�����ļ�����...");
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
	printf("�����ļ��������\n\n");
	return;
}