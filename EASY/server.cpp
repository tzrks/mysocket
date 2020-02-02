#include"severclass.hpp"
#include <iostream>
using namespace std;


int main() {	
	MySocketSever msocket(8888, (char*)"127.0.0.1");


	system("pause");
	return 1;
}