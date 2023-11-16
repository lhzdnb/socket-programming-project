//
// Created by Administrator on 2023/11/15.
//
// ========================== linux version ==========================

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
#include "build_encryption_map.h"

using namespace std;

int main(int argc, char* argv[]) {
    string username;
    string password;
    string bookCode;
    std::unordered_map<char, char> encryptionMap = buildEncryptionMap();
    cout << "Client is up and running." << endl;
    bool isAuthorized = false;
    
    int clientSocket;
    sockaddr_in localAddress;
    int localPortNumber = 0;
    int serverMport = 45469;
    
    // =============== Step 1 - Set up a Client Socket ===============
    
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        cout << "socket() failed with error: " << errno << endl;
        return 0;
    } else {
//        cout << "socket() is OK!" << endl;
    }
    
    // =============== Step 2 - Connect to the Server ===============
    
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &clientService.sin_addr);
    clientService.sin_port = htons(serverMport);
    if (connect(clientSocket, (struct sockaddr*)&clientService, sizeof(clientService)) == -1) {
        cout << "connect() failed with error: " << errno << endl;
        close(clientSocket);
        return 0;
    } else {
//        cout << "connect() is OK!" << endl;
        socklen_t addressLength = sizeof(localAddress);
        if (getsockname(clientSocket, (struct sockaddr*)&localAddress, &addressLength) == -1) {
            cout << "getsockname() failed with error: " << errno << endl;
        } else {
            localPortNumber = ntohs(localAddress.sin_port);
//            cout << "Local port assigned: " << localPortNumber << endl;
        }
    }
    
    // =============== Step 3 - Chat to the server ===============
    
    while (true) {
        char sendBuffer[200];
        
        if (!isAuthorized) {
            cout << "Please enter the username: ";
            cin >> username;
            cout << "Please enter the password: ";
            cin >> password;
            
            string encryptedUsername = encrypt(username, encryptionMap);
            string encryptedPassword = encrypt(password, encryptionMap);
            string message = encryptedUsername + " " + encryptedPassword;
            strncpy(sendBuffer, message.c_str(), sizeof(sendBuffer));
            
            int byteSent = send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);
            if (byteSent > 0) {
                cout << username << " sent an authentication request to the Main Server." << endl;
            } else {
                cout << "send() failed with error: " << errno << endl;
                close(clientSocket);
                return 0;
            }
        } else {
            cout << "Please enter book code to query: ";
            cin >> bookCode;
            strncpy(sendBuffer, bookCode.c_str(), sizeof(sendBuffer));
            int byteSent = send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);
            if (byteSent > 0) {
                cout << username << " sent the request to the Main Server." << endl;
            } else {
                cout << "send() failed with error: " << errno << endl;
                close(clientSocket);
                return 0;
            }
        }
        
        char receiveBuffer[200];
        int byteRecv = recv(clientSocket, receiveBuffer, sizeof(receiveBuffer), 0);
        if (byteRecv > 0) {
            cout << "Response received from the Main Server on TCP port: " << localPortNumber << "." << endl;
            if (strcmp(receiveBuffer, "Authorize") == 0) {
                isAuthorized = true;
                cout << username << " received the result of the authentication from the Main Server using TCP over port " << localPortNumber << ". Authentication is successful." << endl;
            }
            else if (strcmp(receiveBuffer, "Unauthorized") == 0) {
                cout << username << " received the result of the authentication from the Main Server using TCP over port " << localPortNumber << ". Authentication failed: Password does not match." << endl;
            }
            else if (strcmp(receiveBuffer, "NoUser") == 0) {
                cout << username << " received the result of the authentication from the Main Server using TCP over port " << localPortNumber << ". Authentication failed: Username not found." << endl;
            }
            else if (strcmp(receiveBuffer, "NoServer") == 0 || strcmp(receiveBuffer, "NoRecord") == 0) {
                cout << "Not able to find the book-code " << bookCode << " in the system\n" << endl;
                cout << "\n---- Start a new query ----" << endl;
            }
            else if (strcmp(receiveBuffer, "Available") == 0) {
                cout << "The requested book " << bookCode << " is available in the library.\n" << endl;
                cout << "\n---- Start a new query ----" << endl;
            }
            else if (strcmp(receiveBuffer, "Unavailable") == 0) {
                cout << "The requested book " << bookCode << " is NOT available in the library.\n" << endl;
                cout << "\n---- Start a new query ----" << endl;
            }
        } else {
            cout << "recv() failed with error: " << errno << endl;
            close(clientSocket);
            return 0;
        }
    }
    
    close(clientSocket);
    return 0;
}
