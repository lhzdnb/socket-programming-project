//
// Created by Hao Liang on 2023/11/15.
//

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>

using namespace std;

int main(int argc, char* argv[]) {
    cout << "\n====== Step 1 - Set up DLL ======\n" << endl;

    SOCKET serverSocket, acceptSocket;
    int port = 45469;
    WSADATA wsaData;
    int wsaErr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaErr = WSAStartup(wVersionRequested, &wsaData);
    if (wsaErr != 0) {
        cout << "WSAStartup failed with error: " << wsaErr << endl;
        return 0;
    }
    else {
        cout << "WSAStartup() is OK!" << endl;
        cout << "The status: " << wsaData.szSystemStatus << endl;
    }

    cout << "\n====== Step 2 - Set up a Server Socket ======\n" << endl;
    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        cout << "socket() failed with error: " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else {
        cout << "socket() is OK!" << endl;
    }

    cout << "\n====== Step 3 - Bind the Server Socket ======\n" << endl;
    sockaddr_in service;
    service.sin_family = AF_INET;
    InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
    service.sin_port = htons(port);
    if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        cout << "bind() failed with error: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    else {
        cout << "\nbind() is OK!\n" << endl;
    }

    cout << "\n====== Step 4 - Listen on the Server Socket ======\n" << endl;
    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        cout << "listen() failed with error: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    else {
        cout << "listen() is OK! Waiting for connections!" << endl;
    }

    cout << "\n====== Step 5 - Accept a Client Socket ======\n" << endl;

    acceptSocket = accept(serverSocket, NULL, NULL);
    if (acceptSocket == INVALID_SOCKET) {
        cout << "accept() failed with error: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    cout << "Accepted connection" << endl;
    system("pause");
    WSACleanup();


    return 0;
}
