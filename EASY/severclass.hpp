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
		WORD ver = MAKEWORD(2, 2);//创建socket2.0版本
		WSADATA dat;
		WSAStartup(ver, &dat);//启动
		//----------------
		//1建立一个socket           ,文件流，TCP   
		SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		//2绑定用于接受客户端连接的网络端口
		sockaddr_in _sin = {};
		_sin.sin_family = AF_INET;
		_sin.sin_port = htons(port);//host to net unsigned short
		_sin.sin_addr.S_un.S_addr = inet_addr(severip);//INADDR_ANY :本所有IP都 可以
		if (SOCKET_ERROR == bind(_sock, (sockaddr*)&_sin, sizeof(_sin))) {
			printf("错误，绑定网络端口失败...\n");
		}
		else { printf("绑定网络端口成功...\n"); }
		//3 listen 监听网络端口
		if (SOCKET_ERROR == listen(_sock, 5)) { printf("错误，监听网络端口失败...\n"); }
		else { printf("监听网络端口成功...\n"); }	//4 accept 接受
		sockaddr_in clientAddr = {};//客户端的地址信息
		int nAddrlen = sizeof(sockaddr_in);
		int nums = 0;
		char _recvbuf[128] = {};
		_csock = accept(_sock, (sockaddr*)&clientAddr, &nAddrlen);//客户端SOCKET	
		while (1) {		//重复，可多次接受客户端	

			if (INVALID_SOCKET == _csock) { printf("错误，接受到无效客户端socket...\n"); }
			else {


				int nlen = recv(_csock, _recvbuf, 128, 0);//n5 接收客户端的数据
				if (0 == strcmp(_recvbuf, "exit")) { printf("客户端已退出，任务结束"); break; }
				//n6 处理请求
				if (0 == strcmp(_recvbuf, "getName")) {
					char msgbuf[] = "小强";
					send(_csock, msgbuf, strlen(msgbuf) + 1, 0);
				}
				else if (0 == strcmp(_recvbuf, "getage")) {
					char msgbuf[] = "80";
					send(_csock, msgbuf, strlen(msgbuf) + 1, 0);
				}
				else {
					char msgbuf[] = "未知命令，请重新输入";
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