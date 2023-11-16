////////
//////// Created by Administrator on 2023/11/15.
////////
//
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
//    cout << "=============== Server ===============" << endl;
//
//    int serverSocket, acceptSocket;
//    int port = 45469;
//
//    cout << "\n====== Step 1 - Set up a Server Socket ======\n" << endl;
//    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
//    if (serverSocket == -1) {
//        cout << "socket() failed" << endl;
//        return 0;
//    }
//    else {
//        cout << "socket() is OK!" << endl;
//    }
//
//    cout << "\n====== Step 2 - Bind the Server Socket ======\n" << endl;
//    sockaddr_in service;
//    memset(&service, 0, sizeof(service));
//    service.sin_family = AF_INET;
//    inet_pton(AF_INET, "127.0.0.1", &service.sin_addr);
//    service.sin_port = htons(port);
//
//    if (bind(serverSocket, (struct sockaddr*)&service, sizeof(service)) == -1) {
//        cout << "bind() failed" << endl;
//        close(serverSocket);
//        return 0;
//    }
//    else {
//        cout << "bind() is OK!" << endl;
//    }
//
//    cout << "\n====== Step 3 - Listen on the Server Socket ======\n" << endl;
//    if (listen(serverSocket, 1) == -1) {
//        cout << "listen() failed" << endl;
//        close(serverSocket);
//        return 0;
//    }
//    else {
//        cout << "listen() is OK! Waiting for connections!" << endl;
//    }
//
//    cout << "\n====== Step 4 - Accept a Client Socket ======\n" << endl;
//    acceptSocket = accept(serverSocket, NULL, NULL);
//    if (acceptSocket == -1) {
//        cout << "accept() failed" << endl;
//        close(serverSocket);
//        return -1;
//    }
//    cout << "Accepted connection" << endl;
//
//    cout << "\n====== Step 5 - Chat with the Client ======\n" << endl;
//    char buffer[200];
//    int byteReceived = recv(acceptSocket, buffer, 200, 0);
//    if (byteReceived > 0) {
//        cout << "Message received successfully!" << endl;
//        cout << "Message: " << buffer << endl;
//    }
//    else {
//        cout << "recv() failed" << endl;
//        close(serverSocket);
//        return 0;
//    }
//
//    char confirmation[200] = "Message received successfully!";
//    int byteSent = send(acceptSocket, confirmation, strlen(confirmation) + 1, 0);
//    if (byteSent > 0) {
//        cout << "Confirmation sent successfully!" << endl;
//    }
//    else {
//        cout << "send() failed" << endl;
//        close(serverSocket);
//        return 0;
//    }
//
//    cout << "\n====== Step 6 - Close the Server Socket ======\n" << endl;
//    close(serverSocket);
//    close(acceptSocket);
//
//    return 0;
//}
