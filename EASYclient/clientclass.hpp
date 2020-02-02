#ifndef _EasyTcpClient_hpp_
#define _EasyTcpClient_hpp_

#ifdef _WIN32
#define FD_SETSIZE      2506
#define WIN32_LEAN_AND_MEAN
#include<windows.h>
#include<WinSock2.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")
#else
#include<unistd.h> //uni std
#include<arpa/inet.h>
#include<string.h>

#define SOCKET int
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)
#endif

class MyClientSocket {

public :
	SOCKET _csock = INVALID_SOCKET;
	MyClientSocket(int port, char *severip) {
		WORD ver = MAKEWORD(2, 2);//����socket2.0�汾
		WSADATA dat;
		WSAStartup(ver, &dat);//����
		SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);
		if (INVALID_SOCKET == _sock) { printf("���󣬽���socketʧ��...\n"); }
		else { printf("����socket�ɹ�...\n"); }
		//2���ӷ����� connect
		sockaddr_in _sin = {};
		_sin.sin_family = AF_INET;
		_sin.sin_port = htons(port);//host to net unsigned short �˿ں�Ҫ���������һ��
		_sin.sin_addr.S_un.S_addr = inet_addr(severip);//��������ַ
		int ret = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
		while (INVALID_SOCKET == ret) {
			printf("�����������ʧ��,����������Ƿ�����...\n");
			ret = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
			Sleep(1000);
		}
		 printf("����������ӳɹ�...\n");
		//
		while (1)
		{
			//n3 ���� ��������
			char cmdbuf[128] = {};
			printf("���������getName,getage,exit\n");
			scanf("%s", cmdbuf);
			//n4 ��������
			if (0 == strcmp(cmdbuf, "exit"))
			{
				char cmdbuf[128] = { "exit" };
				send(_sock, cmdbuf, strlen(cmdbuf) + 1, 0);
				break;//�˳�
			}
			else
			{//n5 ����
				send(_sock, cmdbuf, strlen(cmdbuf) + 1, 0);
			}
			//3 ���շ�������Ϣ recv
			char recvbuf[128] = {};
			int nlen = recv(_sock, recvbuf, 128, 0);
			if (nlen > 0) {
				printf("���յ����ݣ�%s\n", recvbuf);
			}	
	}

};
	~MyClientSocket() {
		closesocket(_csock);
		WSACleanup();//
	}	
};


#endif // !_EasyTcpClient_hpp_