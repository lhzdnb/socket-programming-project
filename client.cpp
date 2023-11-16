//
// Created by Administrator on 2023/11/15.
//
// ========================== windows version ==========================
#include <iostream>
using namespace std;
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>

int main(int argc, char* argv[]) {
    cout << "=============== Client ===============" << endl;
    cout << "\n====== Step 1 - Set up DLL ======\n" << endl;

    SOCKET clientSocket;
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

    cout << "\n====== Step 2 - Set up a Client Socket ======\n" << endl;

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

    cout << "\n====== Step 3 - Connect to the Server ======\n" << endl;
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);
    clientService.sin_port = htons(port);
    if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
        cout << "connect() failed with error: " << WSAGetLastError() << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 0;
    }
    else {
        cout << "connect() is OK!" << endl;
    }

    cout << "\n====== Step 4 - Chat to the server ======\n" << endl;

    char buffer[200];
    cout << "Please enter your message to send to the server: ";
    cin.getline(buffer, 200);
    int byteSent = send(clientSocket, buffer, 200, 0);

    if (byteSent > 0) {
        cout << "Message sent successfully!" << endl;
    }
    else {
        cout << "send() failed with error: " << WSAGetLastError() << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 0;
    }

    char receiveBuffer[200];
    int byteRecv = recv(clientSocket, receiveBuffer, 200, 0);
    if (byteRecv > 0) {
        cout << "Message received: " << receiveBuffer << endl;
    }
    else {
        cout << "recv() failed with error: " << WSAGetLastError() << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 0;
    }

    cout << "\n====== Step 5 - Close the Client Socket ======\n" << endl;
    system("pause");
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}

// ========================== linux version ==========================

//#include <iostream>
//#include <cstring>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <unistd.h>
//
//using namespace std;
//
//int main(int argc, char* argv[]) {
//    cout << "=============== Client ===============" << endl;
//
//    int clientSocket;
//    int port = 45469;
//
//    cout << "\n====== Step 1 - Create a Client Socket ======\n" << endl;
//
//    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
//    if (clientSocket == -1) {
//        cout << "socket() failed" << endl;
//        return 0;
//    }
//    else {
//        cout << "socket() is OK!" << endl;
//    }
//
//    cout << "\n====== Step 2 - Connect to the Server ======\n" << endl;
//    sockaddr_in clientService;
//    memset(&clientService, 0, sizeof(clientService));
//    clientService.sin_family = AF_INET;
//    inet_pton(AF_INET, "127.0.0.1", &clientService.sin_addr);
//    clientService.sin_port = htons(port);
//
//    if (connect(clientSocket, (struct sockaddr*)&clientService, sizeof(clientService)) == -1) {
//        cout << "connect() failed" << endl;
//        close(clientSocket);
//        return 0;
//    }
//    else {
//        cout << "connect() is OK!" << endl;
//    }
//
//    cout << "\n====== Step 3 - Chat with the Server ======\n" << endl;
//
//    char buffer[200];
//    cout << "Please enter your message to send to the server: ";
//    cin.getline(buffer, 200);
//    int byteSent = send(clientSocket, buffer, strlen(buffer) + 1, 0);
//
//    if (byteSent > 0) {
//        cout << "Message sent successfully!" << endl;
//    }
//    else {
//        cout << "send() failed" << endl;
//        close(clientSocket);
//        return 0;
//    }
//
//    char receiveBuffer[200];
//    int byteRecv = recv(clientSocket, receiveBuffer, 200, 0);
//    if (byteRecv > 0) {
//        cout << "Message received: " << receiveBuffer << endl;
//    }
//    else {
//        cout << "recv() failed" << endl;
//        close(clientSocket);
//        return 0;
//    }
//
//    cout << "\n====== Step 4 - Close the Client Socket ======\n" << endl;
//    close(clientSocket);
//
//    return 0;
//}
