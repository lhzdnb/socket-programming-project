//
// Created by Hao Liang on 2023/11/15.
//

//#include <iostream>
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <tchar.h>
//
//using namespace std;
//
//int main(int argc, char* argv[]) {
//    cout << "\n====== Step 1 - Set up DLL ======\n" << endl;
//
//    SOCKET serverSocket, acceptSocket;
//    int port = 45469;
//    WSADATA wsaData;
//    int wsaErr;
//    WORD wVersionRequested = MAKEWORD(2, 2);
//    wsaErr = WSAStartup(wVersionRequested, &wsaData);
//    if (wsaErr != 0) {
//        cout << "WSAStartup failed with error: " << wsaErr << endl;
//        return 0;
//    }
//    else {
//        cout << "WSAStartup() is OK!" << endl;
//        cout << "The status: " << wsaData.szSystemStatus << endl;
//    }
//
//    cout << "\n====== Step 2 - Set up a Server Socket ======\n" << endl;
//    serverSocket = INVALID_SOCKET;
//    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//    if (serverSocket == INVALID_SOCKET) {
//        cout << "socket() failed with error: " << WSAGetLastError() << endl;
//        WSACleanup();
//        return 0;
//    }
//    else {
//        cout << "socket() is OK!" << endl;
//    }
//
//    cout << "\n====== Step 3 - Bind the Server Socket ======\n" << endl;
//    sockaddr_in service;
//    service.sin_family = AF_INET;
//    InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
//    service.sin_port = htons(port);
//    if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
//        cout << "bind() failed with error: " << WSAGetLastError() << endl;
//        closesocket(serverSocket);
//        WSACleanup();
//        return 0;
//    }
//    else {
//        cout << "\nbind() is OK!\n" << endl;
//    }
//
//    cout << "\n====== Step 4 - Listen on the Server Socket ======\n" << endl;
//    if (listen(serverSocket, 1) == SOCKET_ERROR) {
//        cout << "listen() failed with error: " << WSAGetLastError() << endl;
//        closesocket(serverSocket);
//        WSACleanup();
//        return 0;
//    }
//    else {
//        cout << "listen() is OK! Waiting for connections!" << endl;
//    }
//
//    cout << "\n====== Step 5 - Accept a Client Socket ======\n" << endl;
//
//    acceptSocket = accept(serverSocket, NULL, NULL);
//    if (acceptSocket == INVALID_SOCKET) {
//        cout << "accept() failed with error: " << WSAGetLastError() << endl;
//        closesocket(serverSocket);
//        WSACleanup();
//        return -1;
//    }
//
//    cout << "Accepted connection" << endl;
//
//    cout << "\n====== Step 6 - Chat to the client ======\n" << endl;
//
//    char buffer[200];
//    int byteReceived = recv(acceptSocket, buffer, 200, 0);
//    if (byteReceived > 0) {
//        cout << "Message received successfully!" << endl;
//        cout << "Message: " << buffer << endl;
//    }
//    else {
//        cout << "recv() failed with error: " << WSAGetLastError() << endl;
//        closesocket(serverSocket);
//        closesocket(acceptSocket);
//        WSACleanup();
//        return 0;
//    }
//
//    char confirmation[200] = "Message received successfully!";
//    send(acceptSocket, confirmation, 200, 0);
//    int byteSent = send(acceptSocket, confirmation, 200, 0);
//    if (byteSent > 0) {
//        cout << "Confirmation sent successfully!" << endl;
//    }
//    else {
//        cout << "send() failed with error: " << WSAGetLastError() << endl;
//        closesocket(serverSocket);
//        closesocket(acceptSocket);
//        WSACleanup();
//        return 0;
//    }
//
//    cout << "\n====== Step 7 - Close the Server Socket ======\n" << endl;
//    system("pause");
//    closesocket(acceptSocket);
//    closesocket(serverSocket);
//    WSACleanup();
//
//    return 0;
//}

#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

int main(int argc, char* argv[]) {
    unordered_map<string, int> portNumbers;
    portNumbers["S"] = 41469;
    portNumbers["L"] = 42469;
    portNumbers["H"] = 43469;
    
    bool isAuthorized = false;
    
    cout <<"\nMain Server is up and running\n" << endl;
    
    unordered_map<string, string> members;
    
    ifstream file;
    file.open("../input_files/member.txt", ios::in);
    if (!file.is_open()) {
        cout << "Error opening member.txt" << endl;
        return 1;
    }
    
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string username, password;
        if (getline(iss, username, ',') && getline(iss, password)) {
            password.erase(0, password.find_first_not_of(" \n\r\t"));
            members[username] = password;
        }
    }
    
    file.close();
    
    cout << "Main Server loaded member data" << endl;
    
    int TCP_Socket;
    int TCP_Port = 45469;
    TCP_Socket = socket(AF_INET, SOCK_STREAM, 0);
    if (TCP_Socket == -1) {
        cout << "TCP socket() failed with error: " << errno << endl;
        return 0;
    }
    else {
        cout << "TCP socket() is OK!" << endl;
    }
    
    int UDP_Socket;
    int UDP_Port = 44469;
    UDP_Socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (UDP_Socket == -1) {
        cout << "UDP socket() failed with error: " << errno << endl;
        return 0;
    }
    else {
        cout << "UDP socket() is OK!" << endl;
    }
    
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &clientService.sin_addr);
    clientService.sin_port = htons(TCP_Port);
    if (bind(TCP_Socket, (struct sockaddr*)&clientService, sizeof(clientService)) == -1) {
        cout << "TCP bind() failed with error: " << errno << endl;
        close(TCP_Socket);
        return 0;
    }
    else {
        cout << "\nTCP bind() is OK!\n" << endl;
    }
    
    sockaddr_in backendService;
    backendService.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &backendService.sin_addr);
    backendService.sin_port = htons(UDP_Port);
    if (bind(UDP_Socket, (struct sockaddr*)&backendService, sizeof(backendService)) == -1) {
        cout << "UDP bind() failed with error: " << errno << endl;
        close(UDP_Socket);
        return 0;
    }
    else {
        cout << "\nUDP bind() is OK!\n" << endl;
    }
    
    if (listen(TCP_Socket, 1) == -1) {
        cout << "listen() failed with error: " << errno << endl;
        close(TCP_Socket);
        return 0;
    }
    else {
        cout << "listen() is OK! Waiting for connections!" << endl;
    }
    
    while (true) {
        int acceptTCP_Socket = accept(TCP_Socket, NULL, NULL);
        if (acceptTCP_Socket == -1) {
            cout << "accept() failed with error: " << errno << endl;
            close(TCP_Socket);
            return -1;
        }
        else {
            cout << "Accepted connection" << endl;
        }
        
        char TCP_receive_buffer[200];
        int responseType = 0;
        
        int TCP_byteReceived = recv(acceptTCP_Socket, TCP_receive_buffer, 200, 0);
        if (TCP_byteReceived > 0) {
            if (!isAuthorized) {
                string username, password;
                string bufferStr(TCP_receive_buffer);
                istringstream iss(bufferStr);
                iss >> username >> password;
                cout << "Main Server received the username and password from the client using TCP over port" << TCP_Port << endl;
                
                if (members.find(username) != members.end()) {
                    if (members[username] == password) {
                        isAuthorized = true;
                        cout << "Password " << password << " matches the username. Send a reply to the client" << endl;
                        responseType = 1;
                    }
                    else {
                        cout << "Main Server failed to authenticate " << username << endl;
                        responseType = 2;
                    }
                }
                else {
                    cout << "Main Server failed to authenticate " << username << endl;
                    responseType = 3;
                }
            } else {
                cout << "Main Server received the request from the client using TCP over port " << TCP_Port << endl;
                string target(1, TCP_receive_buffer[0]);
                int targetPort = portNumbers[target];
                
                if (portNumbers.find(target) == portNumbers.end()) {
                    cout << "Did not found " << TCP_receive_buffer << " in the book code list" << endl;
                    responseType = 4;
                }
                else {
                    cout << "Found " << TCP_receive_buffer << " located at Server " << target << ". Send to Server " << target << endl;
                    sockaddr_in targetServer;
                    targetServer.sin_family = AF_INET;
                    inet_pton(AF_INET, "127.0.0.1", &targetServer.sin_addr);
                    targetServer.sin_port = htons(portNumbers[target]);
                    
                    int byteSent = sendto(UDP_Socket, TCP_receive_buffer, 200, 0, (struct sockaddr*)&targetServer, sizeof(targetServer));
                    if (byteSent > 0) {
                        char UDP_receive_buffer[200];
                        int byteRecv = recvfrom(UDP_Socket, UDP_receive_buffer, 200, 0, NULL, NULL);
                        if (byteRecv > 0) {
                            cout << "Main Server received the result from Server " << target << ". The book status result using UDP over port " << UDP_Port << ":" << endl;
                            if (strcmp(UDP_receive_buffer, "Available") == 0) {
                                responseType = 5;
                            }
                            else {
                                responseType = 6;
                            }
                        }
                        else {
                            cout << "recvfrom() backend server failed with error: " << errno << endl;
                            return 0;
                        }
                    }
                    else {
                        cout << "sendto() backend server failed with error: " << errno << endl;
                        return 0;
                    }
                }
            }
            
            char TCP_send_buffer[200];
            switch (responseType) {
                case 1:
                    strcpy(TCP_send_buffer, "Authorize");
                    break;
                case 2:
                    strcpy(TCP_send_buffer, "Unauthorized");
                    break;
                case 3:
                    strcpy(TCP_send_buffer, "NoUser");
                    break;
                case 4:
                    strcpy(TCP_send_buffer, "NoServer");
                    break;
                case 5:
                    strcpy(TCP_send_buffer, "Available");
                    break;
                case 6:
                    strcpy(TCP_send_buffer, "Not Available");
                    break;
                default:
                    break;
            }
            
            int TCP_byteSent = send(acceptTCP_Socket, TCP_send_buffer, 200, 0);
            if (TCP_byteSent < 0) {
                cout << "send() failed with error: " << errno << endl;
                close(TCP_Socket);
                return 0;
            }
        }
        close(acceptTCP_Socket);
    }
    
    close(TCP_Socket);
    close(UDP_Socket);
    
    return 0;
}
