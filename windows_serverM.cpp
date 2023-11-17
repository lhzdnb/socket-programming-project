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
using namespace std;

int main(int argc, char* argv[]) {
    
    unordered_map<string, int> portNumbers;
    portNumbers["S"] = 41469;
    portNumbers["L"] = 42469;
    portNumbers["H"] = 43469;
    
    bool isAuthorized = false;
    bool isAdmin = false;
    char UDP_receive_buffer[200] = {0};
    char bookCode[200] = {0};
    
    // ================== Step 0: Print out the message and load member data ==================
    
    cout <<"\nMain Server is up and running\n" << endl;
    
    unordered_map<string, string> members;
    
    ifstream file;
    file.open("member.txt", ios::in);
    if (!file.is_open()) {
        cout << "Error opening member.txt" << endl;
        return 1;
    }
    
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string username, password;
        if (getline(iss, username, ',') && getline(iss, password)) {
            // Delete the possible space in the beginning of the password
            password.erase(0, password.find_first_not_of(" \n\r\t"));
            
            members[username] = password;
        }
    }
    
    file.close();
    
    cout << "Main Server loaded member list." << endl;
    
    // ================== Step 1: Set up DLL ==================
    
    // Initialize Winsock
    WSADATA wsaData;
    int wsaErr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaErr = WSAStartup(wVersionRequested, &wsaData);
    if (wsaErr != 0) {
        cout << "WSAStartup failed with error: " << wsaErr << endl;
        return 0;
    }
    else {
//        cout << "WSAStartup() is OK!" << endl;
//        cout << "The status: " << wsaData.szSystemStatus << endl;
    }
    
    // ================== Step 2: Set up the TCP and UDP Socket ==================
    
    // TCP socket
    SOCKET TCP_Socket;
    int TCP_Port = 45469;
    TCP_Socket = INVALID_SOCKET;
    TCP_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (TCP_Socket == INVALID_SOCKET) {
        cout << "TCP socket() failed with error: " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else {
//        cout << "TCP socket() is OK!" << endl;
    }
    
    // UDP socket
    SOCKET UDP_Socket;
    int UDP_Port = 44469;
    UDP_Socket = INVALID_SOCKET;
    UDP_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (UDP_Socket == INVALID_SOCKET) {
        cout << "UDP socket() failed with error: " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else {
//        cout << "UDP socket() is OK!" << endl;
    }
    
    // ================== Step 3: Bind the TCP and UDP Socket ==================
    
    // TCP bind
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);
    clientService.sin_port = htons(TCP_Port);
    if (bind(TCP_Socket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
        cout << "TCP bind() failed with error: " << WSAGetLastError() << endl;
        closesocket(TCP_Socket);
        WSACleanup();
        return 0;
    }
    else {
//        cout << "\nTCP bind() is OK!\n" << endl;
    }
    
    // UDP bind
    sockaddr_in backendService;
    backendService.sin_family = AF_INET;
    InetPton(AF_INET, _T("127.0.0.1"), &backendService.sin_addr.s_addr);
    backendService.sin_port = htons(UDP_Port);
    if (bind(UDP_Socket, (SOCKADDR*)&backendService, sizeof(backendService)) == SOCKET_ERROR) {
        cout << "UDP bind() failed with error: " << WSAGetLastError() << endl;
        closesocket(UDP_Socket);
        WSACleanup();
        return 0;
    }
    else {
//        cout << "\nUDP bind() is OK!\n" << endl;
    }
    
    // ================== Step 4: Listen on the TCP Socket ==================
    if (listen(TCP_Socket, 1) == SOCKET_ERROR) {
        cout << "listen() failed with error: " << WSAGetLastError() << endl;
        closesocket(TCP_Socket);
        WSACleanup();
        return 0;
    }
    else {
//        cout << "listen() is OK! Waiting for connections!" << endl;
    }
    
    
    // ================== Step 5: Accept a Client Socket ==================
    SOCKET acceptTCP_Socket = accept(TCP_Socket, NULL, NULL);
    if (acceptTCP_Socket == INVALID_SOCKET) {
        cout << "accept() failed with error: " << WSAGetLastError() << endl;
        closesocket(TCP_Socket);
        WSACleanup();
        return -1;
    }
    else {
//        cout << "Accepted connection" << endl;
    }
    
    while (true) {
        // ================== Step 6: Chat with the client ==================
        char TCP_receive_buffer[200] = {0};
        int responseType = 0;
        // 1: authorize success, 2: password not match, 3: username not exist
        // 4: request not match 5: available, 6: not available 7: not found
        // 8. not found admin 9. available admin
        
        int TCP_byteReceived = recv(acceptTCP_Socket, TCP_receive_buffer, 200, 0);
        if (TCP_byteReceived > 0) {
            // ================== Step 7: Authorize ==================
            if (!isAuthorized) {
                string username, password;
                string bufferStr(TCP_receive_buffer);
                istringstream iss(bufferStr);
                iss >> username >> password;
                cout << "Main Server received the username and password from the client using TCP over port " << TCP_Port
                   << "."  << endl;
                // If the username exists in the member.txt
                if (members.find(username) != members.end()) {
                    // If the password matches the username
                    if (members[username] == password) {
                        isAuthorized = true;
                        
                        // Admin request
                        if (username == "firns" && password == members["firns"]) {
                            isAdmin = true;
                        }
                        cout << "Password " << password << " matches the username. Send a reply to the client." << endl;
                        responseType = 1;
                    }
                        // If the password does not match the username
                    else {
                        cout << "Password " << password << " does not match the username. Send a reply to the client." << endl;
                        responseType = 2;
                    }
                }
                    // If the username does not exist in the member.txt
                else {
                    cout << username << " is not registered. Send a reply to the client." << endl;
                    responseType = 3;
                }
            } else {
                strcpy(bookCode, TCP_receive_buffer);
                // ================== Step 8: Send the request to the backend server ==================
                cout << "Main Server received the book request from the client using TCP over port " << TCP_Port << "." << endl;
                string target(1, TCP_receive_buffer[0]);
                
                if (portNumbers.find(target) == portNumbers.end()) {
                    cout << "Did not found " << TCP_receive_buffer << " in the book code list." << endl;
                    responseType = 4;
                }
                else {
                    int targetPort = portNumbers[target];
                    cout << "Found " << TCP_receive_buffer << " located at Server " << target << ". Send to Server " << target << "." << endl;
                    sockaddr_in targetServer;
                    targetServer.sin_family = AF_INET;
                    InetPton(AF_INET, _T("127.0.0.1"), &targetServer.sin_addr.s_addr);
                    targetServer.sin_port = htons(targetPort);
                    
                    // Admin request to backend server
                    if (isAdmin) {
                        TCP_receive_buffer[TCP_byteReceived - 2] = 'A';
                        TCP_receive_buffer[TCP_byteReceived + 2] = '\0';
                    }
                    
                    int byteSent = sendto(UDP_Socket, TCP_receive_buffer, 200, 0, (SOCKADDR*)&targetServer,
                                          sizeof(targetServer));
                    
                    if (byteSent > 0) {
                        // ================== Step 9: Receive the response from the backend server ==================
                        int byteRecv = recvfrom(UDP_Socket, UDP_receive_buffer, 200, 0, NULL, NULL);
                        if (byteRecv > 0) {
                            cout << "Main Server received from Server " << target << " the book status result using UDP over port " << UDP_Port << "." << endl;
                            if (strcmp(UDP_receive_buffer, "The requested book is available.") == 0 && !isAdmin) {
                                cout << "The requested book " << bookCode << " is available." << endl;
                                responseType = 5;
                            }
                            else if (strcmp(UDP_receive_buffer, "The requested book is not available.") == 0) {
                                cout << "The requested book " << bookCode << " is NOT available." << endl;
                                responseType = 6;
                            }
                            
                            else if (strcmp(UDP_receive_buffer, "Not able to find the book.") == 0 && !isAdmin) {
                                cout << "Not able to find the book " << bookCode << " in the system." << endl;
                                responseType = 7;
                            }
                            else if (strcmp(UDP_receive_buffer, "Not able to find the book.") == 0 && isAdmin) {
                                cout << "Not able to find the book " << bookCode << " in the system." << endl;
                                responseType = 8;
                            }
                            else if (isAdmin) {
                                responseType = 9;
                            }
                        }
                        else {
                            cout << "recvfrom() backend server failed with error: " << WSAGetLastError() << endl;
                            WSACleanup();
                            return 0;
                        }
                    }
                    else {
                        cout << "sendto() backend server failed with error: " << WSAGetLastError() << endl;
                        WSACleanup();
                        return 0;
                    }
                }
                
            }
            
            // ================== Step 10: Send the response to the client ==================
            char TCP_send_buffer[200] = {0};
            switch (responseType) {
                case 1:
                    strcpy_s(TCP_send_buffer, "Authorize");
                    break;
                case 2:
                    strcpy_s(TCP_send_buffer, "Unauthorized");
                    break;
                case 3:
                    strcpy_s(TCP_send_buffer, "NoUser");
                    break;
                case 4:
                    strcpy_s(TCP_send_buffer, "NoServer");
                    break;
                case 5:
                    strcpy_s(TCP_send_buffer, "Available");
                    break;
                case 6:
                    strcpy_s(TCP_send_buffer, "Unavailable");
                    break;
                case 7:
                    strcpy_s(TCP_send_buffer, "NoRecord");
                    break;
                case 8:
                    strcpy_s(TCP_send_buffer, "NoRecord");
                    break;
                case 9:
                    strcpy_s(TCP_send_buffer, UDP_receive_buffer);
                    break;
                default:
                    break;
            }
            int TCP_byteSent = send(acceptTCP_Socket, TCP_send_buffer, 200, 0);
            if (TCP_byteSent > 0) {
                if (isAuthorized && responseType >= 4 && responseType <= 8) {
                    cout << "Main Server sent the book result to the client." << endl;
                }
                else if (responseType == 9) {
                    cout << "Number of books " << bookCode << " available is: " << TCP_send_buffer[0] << "." << endl;
                    cout << "Main Server sent the book result to the client." << endl;
                }
            }
            else {
                cout << "send() failed with error: " << WSAGetLastError() << endl;
                closesocket(TCP_Socket);
                WSACleanup();
                return 0;
            }
        }
    }
}