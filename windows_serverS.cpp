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
   // =============== Step 1 - Set up DLL ===============
    unordered_map<string, string> bookMap;
    string bookCode, number;
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
        cout << "WSAStartup() is OK!" << endl;
        cout << "The status: " << wsaData.szSystemStatus << endl;
    }
    
    // =============== Step 2 - Set up a UDP socket ===============
    SOCKET udpSocket;
    int udp_port = 41469;
    udpSocket = INVALID_SOCKET;
    udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET) {
        cout << "socket() failed with error: " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else {
        cout << "socket() is OK!" << endl;
    }
    
    ifstream file;
    file.open("../input_files/science.txt", ios::in);
    if (!file.is_open()) {
        cout << "Error opening member.txt" << endl;
        return 1;
    }
    
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string code, num;
        if (getline(iss, code, ',') && getline(iss, num)) {
            // Delete the possible space in the beginning of the number
            num.erase(0, num.find_first_not_of(" \n\r\t"));
            bookMap[code] = num;
        }
    }
    
    file.close();
    
    // =============== Step 3 - Bind the UDP socket ===============
    sockaddr_in udpService;
    udpService.sin_family = AF_INET;
    InetPton(AF_INET, _T("127.0.0.1"), &udpService.sin_addr.s_addr);
    udpService.sin_port = htons(udp_port);
    if (bind(udpSocket, (SOCKADDR*)&udpService, sizeof(udpService)) == SOCKET_ERROR) {
        cout << "bind() failed with error: " << WSAGetLastError() << endl;
        closesocket(udpSocket);
        WSACleanup();
        return 0;
    }
    else {
        cout << "Server S is up and running using UDP in port" << udp_port << "." << endl;
    }
    
    // =============== Step 4 - Receive data from Main Server ===============
    while (true) {
        sockaddr_in clientService;
        int clientService_length = sizeof(clientService);
        clientService.sin_family = AF_INET;
        InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);
        clientService.sin_port = htons(45469);
        char recvBuffer[1024];
        int recvBytes = recvfrom(udpSocket, recvBuffer, sizeof(recvBuffer), 0, (SOCKADDR*)&clientService, &clientService_length);
        
        if (recvBytes < 0) {
            cout << "recvfrom() failed with error: " << WSAGetLastError() << endl;
            closesocket(udpSocket);
            WSACleanup();
            return 0;
        }
        else {
            bookCode = recvBuffer;
            cout << "Server S received " << bookCode << " code from the Main Server." << endl;
        }
        
        // =============== Step 5 - Send data to Main Server ===============
        
        char sendBuffer[200];
        if (bookMap.find(bookCode) != bookMap.end()) {
            number = bookMap[bookCode];
            if (stoi(number) > 0) {
                strcpy_s(sendBuffer, "The requested book is available.");
                
                // Update the number of books
                string new_number = to_string(stoi(number) - 1);
                bookMap[bookCode] = new_number;
                ofstream outFile;
                outFile.open("../input_files/science.txt", ios::out);
                if (!outFile.is_open()) {
                    cerr << "无法打开文件science.txt进行写入" << endl;
                    return 1;
                }
                
                for (const auto& pair : bookMap) {
                    outFile << pair.first << ", " << pair.second << "\n";
                }
                
                outFile.close();
            }
            else {
                strcpy_s(sendBuffer, "The requested book is not available.");
            }
        }
        else {
            strcpy_s(sendBuffer, "Not able to find the book");
        }
        int sendBytes = sendto(udpSocket, sendBuffer, sizeof(sendBuffer), 0, (SOCKADDR*)&clientService, sizeof(clientService));
        if (sendBytes < 0) {
            cout << "sendto() failed with error: " << WSAGetLastError() << endl;
            closesocket(udpSocket);
            WSACleanup();
            return 0;
        }
        else {
            cout << "Server S finished sending the availability status of code " << bookCode << " to the Main Server using UDP on port " << udp_port << "." << endl;
        }
    }
    
}