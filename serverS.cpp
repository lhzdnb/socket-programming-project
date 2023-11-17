//
// Created by Administrator on 2023/11/16.
//
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
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char* argv[]) {
    unordered_map<string, string> bookMap;
    string bookCode, number;
    
    bool admin = false;
    
    // =============== Step 1 - Set up a UDP socket ===============
    int udpSocket;
    int udp_port = 41469;
    udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSocket == -1) {
        cout << "socket() failed with error: " << errno << endl;
        return 0;
    }
    else {
//        cout << "socket() is OK!" << endl;
    }
    
    ifstream file;
    file.open("science.txt", ios::in);
    if (!file.is_open()) {
        cout << "Error opening science.txt" << endl;
        return 1;
    }
    
    string line;
    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        istringstream iss(line);
        string code, num;
        if (getline(iss, code, ',') && getline(iss, num)) {
            // Delete the possible space in the beginning of the number
            num.erase(0, num.find_first_not_of(" \n\r\t"));
            bookMap[code] = num;
        }
    }
    
    file.close();
    
    // =============== Step 2 - Bind the UDP socket ===============
    
    sockaddr_in udpService;
    udpService.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &udpService.sin_addr);
    udpService.sin_port = htons(udp_port);
    if (bind(udpSocket, (struct sockaddr*)&udpService, sizeof(udpService)) == -1) {
        cout << "bind() failed with error: " << errno << endl;
        close(udpSocket);
        return 0;
    }
    else {
        cout << "Server S is up and running using UDP on port " << udp_port << "." << endl;
    }
    
    sockaddr_in clientService;
    socklen_t clientService_length = sizeof(clientService);
    clientService.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &clientService.sin_addr);
    clientService.sin_port = htons(45469);
    
    // =============== Step 3 - Receive data from Main Server ===============
    
    while (true) {
        char recvBuffer[200];
        int recvBytes = recvfrom(udpSocket, recvBuffer, sizeof(recvBuffer), 0, (struct sockaddr*)&clientService, &clientService_length);
        
        if (recvBytes < 0) {
            cout << "recvfrom() failed with error: " << errno << endl;
            close(udpSocket);
            return 0;
        }
        else {
            bookCode = recvBuffer;
            if (recvBuffer[recvBytes - 2] == 'A') {
                admin = true;
                cout << "Server S received an inventory status request for code " << bookCode << "." << endl;
            }
            else {
                cout << "Server S received " << bookCode << " code from the Main Server." << endl;
            }
        }
        
        // =============== Step 4 - Send data to Main Server ===============
        
        char sendBuffer[200];
        if (bookMap.find(bookCode) != bookMap.end()) {
            number = bookMap[bookCode];
            if (stoi(number) > 0 && !admin) {
                strcpy(sendBuffer, "The requested book is available.");
                
                // Update the number of books
                string new_number = to_string(stoi(number) - 1);
                bookMap[bookCode] = new_number;
                ofstream outFile;
                outFile.open("science.txt", ios::out);
                if (!outFile.is_open()) {
                    cerr << "Unable to open file science.txt for writing" << endl;
                    return 1;
                }
                
                for (const auto& pair : bookMap) {
                    outFile << pair.first << ", " << pair.second << "\n";
                }
                
                outFile.close();
            }
                // Admin request, just send the number of books if it is not negative
            else if (stoi(number) >= 0 && admin) {
                strcpy(sendBuffer, number.c_str());
            }
            else {
                strcpy(sendBuffer, "The requested book is not available.");
            }
        }
        else {
            strcpy(sendBuffer, "Not able to find the book.");
        }
        int sendBytes = sendto(udpSocket, sendBuffer, sizeof(sendBuffer), 0, (struct sockaddr*)&clientService, sizeof(clientService));
        if (sendBytes < 0) {
            cout << "sendto() failed with error: " << errno << endl;
            close(udpSocket);
            return 0;
        }
        else if (admin) {
            cout << "Server S finished sending the inventory status to the Main Server using UDP on port " << udp_port << "." << endl;
        }
        else {
            cout << "Server S finished sending the availability status of code " << bookCode << " to the Main Server using UDP on port " << udp_port << "." << endl;
        }
    }
    
    close(udpSocket);
    return 0;
}
