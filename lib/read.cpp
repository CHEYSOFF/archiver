#pragma once
#include <fstream>
#include <iostream>
#include <string>

#include "write.cpp"

int GetFileSize(std::string in_path) {
    long long max_size = 1'000'000'000'000 / 8;
    // max file_size is 1'000'000'000'000 bits = 125'000'000'000 mb = 125 gb
    // => 40 bits = 5 bytes for file size
    std::fstream in;
    in.open(in_path, std::ios::in);
    char c;
    long long char_co = 0;
    while (in.get(c)) {
        char_co++;
        if (char_co > max_size) {
            return -1;
        }
    }
    return char_co;
}

std::string GetFileName(std::string in_path){
    int begining = 0;
    for(int i = 0; i < in_path.size(); i++){
        if(in_path[i] == '\\' || in_path[i] == '/'){
            begining = i + 1;
        }
    }
    return in_path.substr(begining);
}

// bytes without hamming
// 5    +   21  =   26
// size     name
// bytes with hamming 
// 5    *   1.5     =   8
// 21   *   1.5     =   32

bool AddFile(std::string in_path, std::string out_path) {
    std::string file_name = GetFileName(in_path);
    int max_name_length = 21;
    if(file_name.size() > max_name_length){

        std::cerr << "File name is too large(max length is 27 symbols)\n";
        return 0;
    }
    long long file_size = GetFileSize(in_path);  // Добавить название и его размер
    if(file_size == -1){
        std::cerr << file_name << " is too large(max size is 125 gb)\n";
        return 0;
    }
    
    WriteSizeToFile(out_path, file_size);
}
