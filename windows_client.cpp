//
// Created by Administrator on 2023/11/15.
//
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include "build_encryption_map.h"
using namespace std;

int main(int argc, char* argv[]) {
    std::unordered_map<char, char> encryptionMap = buildEncryptionMap();
    cout << "Client is up and running" << endl;
    bool isAuthorized = false;
    // =============== Step 1 - Set up DLL ===============

    SOCKET clientSocket;
    sockaddr_in localAddress;
    int localPortNumber = 0;
    int serverMport = 45469;
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
    
    // =============== Step 2 - Set up a Client Socket ===============

    clientSocket = INVALID_SOCKET;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        cout << "socket() failed with error: " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else {
        cout << "socket() is OK!" << endl;
    }

    // =============== Step 3 - Connect to the Server ===============
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);
    clientService.sin_port = htons(serverMport);
    if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
        cout << "connect() failed with error: " << WSAGetLastError() << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 0;
    }
    else {
        cout << "connect() is OK!" << endl;
        int addressLength = sizeof(localAddress);
        if (getsockname(clientSocket, (struct sockaddr*)&localAddress, &addressLength) == SOCKET_ERROR) {
            cout << "getsockname() failed with error: " << WSAGetLastError() << endl;
        } else {
            // 打印分配给客户端套接字的本地端口号
//            cout << "Local port assigned: " << ntohs(localAddress.sin_port) << endl;
            localPortNumber = ntohs(localAddress.sin_port);
        }
    }
    
    // =============== Step 4 - Chat to the server ===============
    
    while(true) {
        char sendBuffer[200];
        string username;
        string password;
        string bookCode;
        if (!isAuthorized) {
            cout << "Please enter the username: ";
            cin >> username;
            cout << "Please enter the password: ";
            cin >> password;
            
            string encryptedUsername = encrypt(username, encryptionMap);
            string encryptedPassword = encrypt(password, encryptionMap);
            string message = encryptedUsername + " " + encryptedPassword;
            strcpy_s(sendBuffer, message.c_str());

            int byteSent = send(clientSocket, sendBuffer, 200, 0);
            if (byteSent > 0) {
                cout << username << " sent an authentication request to the Main Server." << endl;
            }
            else {
                cout << "send() failed with error: " << WSAGetLastError() << endl;
                closesocket(clientSocket);
                WSACleanup();
                return 0;
            }
        }
        else {
            cout << "Please enter book code to query: ";
            cin >> bookCode;
            strcpy_s(sendBuffer, bookCode.c_str());
            int byteSent = send(clientSocket, sendBuffer, 200, 0);
            if (byteSent > 0) {
                cout << username << "sent the request to the Main Server." << endl;
            }
            else {
                cout << "send() failed with error: " << WSAGetLastError() << endl;
                closesocket(clientSocket);
                WSACleanup();
                return 0;
            }
        }

        char receiveBuffer[200];
        int byteRecv = recv(clientSocket, receiveBuffer, 200, 0);
        if (byteRecv > 0) {
            cout << "Response received from the Main Server on TCP port: " << localPortNumber << endl;
            if (strcmp(receiveBuffer, "Authorize") == 0) {
                isAuthorized = true;
                cout << username << "received the result of the authentication from the Main Server using TCP over port " << localPortNumber << "Authentication is successful" << endl;
            }
            else if (strcmp(receiveBuffer, "Unauthorized") == 0) {
                cout << username << "received the result of the authentication from the Main Server using TCP over port " << localPortNumber << "Authentication failed: Username not found" << endl;
            }
            else if (strcmp(receiveBuffer, "NoUser") == 0) {
                cout << username << "received the result of the authentication from the Main Server using TCP over port " << localPortNumber << "Authentication failed: Password does not match" << endl;
            }
            else if (strcmp(receiveBuffer, "NoServer") == 0) {
                cout << "Not able to find the book-code " << bookCode << " in the system\n" << endl;
                cout << "---- Start a new query ----" << endl;
            }
            else if (strcmp(receiveBuffer, "Available") == 0) {
                cout << "The requested book " << bookCode << " is available in the library." << endl;
                cout << "---- Start a new query ----" << endl;
            }
            else if (strcmp(receiveBuffer, "Unavailable") == 0) {
                cout << "The requested book " << bookCode << " is NOT available in the library." << endl;
                cout << "---- Start a new query ----" << endl;
            }
        }
        else {
            cout << "recv() failed with error: " << WSAGetLastError() << endl;
            closesocket(clientSocket);
            WSACleanup();
            return 0;
        }
    }
    
    // =============== Step 5 - Close the Client Socket ===============
    closesocket(clientSocket);
    WSACleanup();
    return 0;
    
}