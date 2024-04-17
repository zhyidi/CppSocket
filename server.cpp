#include <iostream>
#include <winsock2.h>
#include <thread>
#include <cstring>
using namespace std;
#pragma comment(lib, "ws2_32.lib")

void receiveMsgFromclient(SOCKET communicationSocket)
{
    // 循环接收数据
    char revData[255]; // 保存接收数据
    while (true)
    {
        // 接收数据
        int ret = recv(communicationSocket, revData, 255, 0);
        if (ret > 0)
        {
            revData[ret] = 0x00;
            cout << "客户端：";
            cout << revData << "\n";
        }
        // 发送数据
        string data;
        cout << "please input data to client:";
        cin >> data;
        const char *sendData;
        sendData = data.c_str(); // string 转 const char *
        send(communicationSocket, sendData, strlen(sendData), 0);
    }
    closesocket(communicationSocket);
}

int main(int argc, char *argv[])
{
    // 初始化WSA
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(sockVersion, &wsaData) != 0)
    {
        return 0;
    }

    // 创建套接字
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
    {
        cout << "Create socket error !";
        return 0;
    }

    // 绑定IP和端口
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(8080);
    sin.sin_addr.S_un.S_addr = INADDR_ANY; // 本机
    if (bind(serverSocket, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        cout << "Bind error !";
    }

    // 开始监听
    if (listen(serverSocket, 5) == SOCKET_ERROR)
    {
        cout << "Listen error !";
        return 0;
    }
    cout << "服务器开始监听...\n";

    while (true) // 循环接受服务
    {
        SOCKET communicationSocket;
        sockaddr_in remoteAddr; // 远程ip
        int nAddrlen = sizeof(remoteAddr);
        communicationSocket = accept(serverSocket, (SOCKADDR *)&remoteAddr, &nAddrlen);
        if (communicationSocket == INVALID_SOCKET)
        {
            cout << "Accept error !";
            continue;
        }
        cout << "接受到一个连接: " << inet_ntoa(remoteAddr.sin_addr) << "\n";
        thread mes_th(receiveMsgFromclient, communicationSocket); // 每次连接开一个通信线程
        mes_th.detach();
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}