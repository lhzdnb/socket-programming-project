//
// Created by Administrator on 2023/11/14.
//

#ifndef CODE_BUILD_ENCRYPTION_MAP_H
#define CODE_BUILD_ENCRYPTION_MAP_H
#include <unordered_map>
#include <string>
// Reference: https://blog.51cto.com/u_13875041/5877617
std::unordered_map<char, char> buildEncryptionMap() {
    std::unordered_map<char, char> encMap;
    for (char c = 'a'; c <= 'z'; ++c) {
        encMap[c] = (c - 'a' + 5) % 26 + 'a';
    }
    for (char c = 'A'; c <= 'Z'; ++c) {
        encMap[c] = (c - 'A' + 5) % 26 + 'A';
    }
    for (char c = '0'; c <= '9'; ++c) {
        encMap[c] = (c - '0' + 5) % 10 + '0';
    }
    
    return encMap;
}

std::string encrypt(const std::string& input, const std::unordered_map<char, char>& encMap) {
    std::string encrypted;
    for (char c : input) {
        auto it = encMap.find(c);
        if (it != encMap.end()) {
            encrypted += it->second;
        } else {
            encrypted += c;
        }
    }
    return encrypted;
}

#endif //CODE_BUILD_ENCRYPTION_MAP_H
