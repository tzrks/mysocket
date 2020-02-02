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
		WORD ver = MAKEWORD(2, 2);//创建socket2.0版本
		WSADATA dat;
		WSAStartup(ver, &dat);//启动
		SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);
		if (INVALID_SOCKET == _sock) { printf("错误，建立socket失败...\n"); }
		else { printf("建立socket成功...\n"); }
		//2连接服务器 connect
		sockaddr_in _sin = {};
		_sin.sin_family = AF_INET;
		_sin.sin_port = htons(port);//host to net unsigned short 端口号要与服务器端一至
		_sin.sin_addr.S_un.S_addr = inet_addr(severip);//服务器地址
		int ret = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
		while (INVALID_SOCKET == ret) {
			printf("与服务器连接失败,请检查服务器是否启动...\n");
			ret = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
			Sleep(1000);
		}
		 printf("与服务器连接成功...\n");
		//
		while (1)
		{
			//n3 输入 请求命令
			char cmdbuf[128] = {};
			printf("请输入命令：getName,getage,exit\n");
			scanf("%s", cmdbuf);
			//n4 处理请求
			if (0 == strcmp(cmdbuf, "exit"))
			{
				char cmdbuf[128] = { "exit" };
				send(_sock, cmdbuf, strlen(cmdbuf) + 1, 0);
				break;//退出
			}
			else
			{//n5 发送
				send(_sock, cmdbuf, strlen(cmdbuf) + 1, 0);
			}
			//3 接收服务器信息 recv
			char recvbuf[128] = {};
			int nlen = recv(_sock, recvbuf, 128, 0);
			if (nlen > 0) {
				printf("接收到数据：%s\n", recvbuf);
			}	
	}

};
	~MyClientSocket() {
		closesocket(_csock);
		WSACleanup();//
	}	
};


#endif // !_EasyTcpClient_hpp_