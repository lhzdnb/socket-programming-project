//
// Created by Administrator on 2023/11/15.
//
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // 提供 close() 函数
#include <cstring> // 提供 memset 函数

using namespace std;

int main(int argc, char* argv[]) {
    cout << "\n====== Step 1 - Create a Server Socket ======\n" << endl;
    
    int serverSocket, acceptSocket;
    int port = 45469;
    
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        cout << "socket() failed" << endl;
        return 0;
    } else {
        cout << "socket() is OK!" << endl;
    }
    
    cout << "\n====== Step 2 - Bind the Server Socket ======\n" << endl;
    sockaddr_in service;
    memset(&service, 0, sizeof(service)); // 清零
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("127.0.0.1");
    service.sin_port = htons(port);
    
    if (bind(serverSocket, (struct sockaddr *)&service, sizeof(service)) == -1) {
        cout << "bind() failed" << endl;
        close(serverSocket);
        return 0;
    } else {
        cout << "bind() is OK!" << endl;
    }
    
    cout << "\n====== Step 3 - Listen on the Server Socket ======\n" << endl;
    if (listen(serverSocket, 1) == -1) {
        cout << "listen() failed" << endl;
        close(serverSocket);
        return 0;
    } else {
        cout << "listen() is OK! Waiting for connections!" << endl;
    }
    
    cout << "\n====== Step 4 - Accept a Client Socket ======\n" << endl;
    
    acceptSocket = accept(serverSocket, NULL, NULL);
    if (acceptSocket == -1) {
        cout << "accept() failed" << endl;
        close(serverSocket);
        return -1;
    }
    
    cout << "Accepted connection" << endl;
    close(acceptSocket);
    close(serverSocket);
    
    return 0;
}
