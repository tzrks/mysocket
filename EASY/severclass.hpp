#ifndef _EasyTcpServer_hpp_
#define _EasyTcpServer_hpp_

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




class MySocketSever {
public:
	SOCKET _csock = INVALID_SOCKET;
	MySocketSever(int port, char *severip) {
		WORD ver = MAKEWORD(2, 2);//����socket2.0�汾
		WSADATA dat;
		WSAStartup(ver, &dat);//����
		//----------------
		//1����һ��socket           ,�ļ�����TCP   
		SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		//2�����ڽ��ܿͻ������ӵ�����˿�
		sockaddr_in _sin = {};
		_sin.sin_family = AF_INET;
		_sin.sin_port = htons(port);//host to net unsigned short
		_sin.sin_addr.S_un.S_addr = inet_addr(severip);//INADDR_ANY :������IP�� ����
		if (SOCKET_ERROR == bind(_sock, (sockaddr*)&_sin, sizeof(_sin))) {
			printf("���󣬰�����˿�ʧ��...\n");
		}
		else { printf("������˿ڳɹ�...\n"); }
		//3 listen ��������˿�
		if (SOCKET_ERROR == listen(_sock, 5)) { printf("���󣬼�������˿�ʧ��...\n"); }
		else { printf("��������˿ڳɹ�...\n"); }	//4 accept ����
		sockaddr_in clientAddr = {};//�ͻ��˵ĵ�ַ��Ϣ
		int nAddrlen = sizeof(sockaddr_in);
		int nums = 0;
		char _recvbuf[128] = {};
		_csock = accept(_sock, (sockaddr*)&clientAddr, &nAddrlen);//�ͻ���SOCKET	
		while (1) {		//�ظ����ɶ�ν��ܿͻ���	

			if (INVALID_SOCKET == _csock) { printf("���󣬽��ܵ���Ч�ͻ���socket...\n"); }
			else {


				int nlen = recv(_csock, _recvbuf, 128, 0);//n5 ���տͻ��˵�����
				if (0 == strcmp(_recvbuf, "exit")) { printf("�ͻ������˳����������"); break; }
				//n6 ��������
				if (0 == strcmp(_recvbuf, "getName")) {
					char msgbuf[] = "Сǿ";
					send(_csock, msgbuf, strlen(msgbuf) + 1, 0);
				}
				else if (0 == strcmp(_recvbuf, "getage")) {
					char msgbuf[] = "80";
					send(_csock, msgbuf, strlen(msgbuf) + 1, 0);
				}
				else {
					char msgbuf[] = "δ֪�������������";
					send(_csock, msgbuf, strlen(msgbuf) + 1, 0);
				}
			}
		}

	}
	~MySocketSever() {
		closesocket(_csock);
		WSACleanup();//
	}
private:


};




#endif // !_EasyTcpServer_hpp_