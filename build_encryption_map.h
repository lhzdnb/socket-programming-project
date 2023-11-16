//
// Created by Administrator on 2023/11/14.
//

#ifndef CODE_BUILD_ENCRYPTION_MAP_H
#define CODE_BUILD_ENCRYPTION_MAP_H
#include <unordered_map>
#include <string>

std::unordered_map<char, char> buildEncryptionMap() {
    std::unordered_map<char, char> encMap;
    
    // 加密小写字母
    for (char c = 'a'; c <= 'z'; ++c) {
        encMap[c] = (c - 'a' + 5) % 26 + 'a';
    }
    
    // 加密大写字母
    for (char c = 'A'; c <= 'Z'; ++c) {
        encMap[c] = (c - 'A' + 5) % 26 + 'A';
    }
    
    // 加密数字
    for (char c = '0'; c <= '9'; ++c) {
        encMap[c] = (c - '0' + 5) % 10 + '0';
    }
    
    // 特殊字符不变
    return encMap;
}

std::string encrypt(const std::string& input, const std::unordered_map<char, char>& encMap) {
    std::string encrypted;
    for (char c : input) {
        auto it = encMap.find(c);
        if (it != encMap.end()) {
            encrypted += it->second;
        } else {
            encrypted += c; // 特殊字符保持不变
        }
    }
    return encrypted;
}

#endif //CODE_BUILD_ENCRYPTION_MAP_H
