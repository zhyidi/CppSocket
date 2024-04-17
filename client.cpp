#include <iostream>
#include <winsock2.h>
#include <thread>
#include <cstring>
using namespace std;
#pragma comment(lib, "ws2_32.lib")

void receiveMsgFromserver(SOCKET sclient)
{
	while (true)
	{
		string data;
		cout << "please input data to server:";
		cin >> data;
		const char *sendData;
		sendData = data.c_str(); // string 转 const char *
		send(sclient, sendData, strlen(sendData), 0);
		//循环接收数据
		char revData[255]; // 保存接收数据
		//接收数据
		int ret = recv(sclient, revData, 255, 0);
		if (ret > 0)
		{
			revData[ret] = 0x00;
			cout << "服务端：";
			cout << revData << "\n";
		}
	}
}

int main()
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}

	SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET)
	{
		cout << "invalid socket!";
		return 0;
	}

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(8080);
	serAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		cout << "connect error !";
		closesocket(sclient);
		return 0;
	}

	thread mes_th(receiveMsgFromserver, sclient);
	mes_th.join();

	closesocket(sclient);
	WSACleanup();
	return 0;
}
