#include"clientclass.hpp"
#include <iostream>


int main() {	

	MyClientSocket clientsocket(8888, (char*)"127.0.0.1");


	system("pause");
	return 1;
}