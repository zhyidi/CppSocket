#include <iostream>
#include <winsock2.h>
#include <thread>
#include <cstring>
using namespace std;
#pragma comment(lib, "ws2_32.lib")

void receiveMsgFromclient(SOCKET communicationSocket)
{
    // ѭ����������
    char revData[255]; // �����������
    while (true)
    {
        // ��������
        int ret = recv(communicationSocket, revData, 255, 0);
        if (ret > 0)
        {
            revData[ret] = 0x00;
            cout << "�ͻ��ˣ�";
            cout << revData << "\n";
        }
        // ��������
        string data;
        cout << "please input data to client:";
        cin >> data;
        const char *sendData;
        sendData = data.c_str(); // string ת const char *
        send(communicationSocket, sendData, strlen(sendData), 0);
    }
    closesocket(communicationSocket);
}

int main(int argc, char *argv[])
{
    // ��ʼ��WSA
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(sockVersion, &wsaData) != 0)
    {
        return 0;
    }

    // �����׽���
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
    {
        cout << "Create socket error !";
        return 0;
    }

    // ��IP�Ͷ˿�
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(8080);
    sin.sin_addr.S_un.S_addr = INADDR_ANY; // ����
    if (bind(serverSocket, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        cout << "Bind error !";
    }

    // ��ʼ����
    if (listen(serverSocket, 5) == SOCKET_ERROR)
    {
        cout << "Listen error !";
        return 0;
    }
    cout << "��������ʼ����...\n";

    while (true) // ѭ�����ܷ���
    {
        SOCKET communicationSocket;
        sockaddr_in remoteAddr; // Զ��ip
        int nAddrlen = sizeof(remoteAddr);
        communicationSocket = accept(serverSocket, (SOCKADDR *)&remoteAddr, &nAddrlen);
        if (communicationSocket == INVALID_SOCKET)
        {
            cout << "Accept error !";
            continue;
        }
        cout << "���ܵ�һ������: " << inet_ntoa(remoteAddr.sin_addr) << "\n";
        thread mes_th(receiveMsgFromclient, communicationSocket); // ÿ�����ӿ�һ��ͨ���߳�
        mes_th.detach();
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}