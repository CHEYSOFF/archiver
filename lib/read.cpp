#pragma once
#include <iostream>
#include <string>
#include <fstream>

#include "write.cpp"

int GetFileSize(std::string in_path){
    std::ifstream txt_file(in_path, std::ios::in);
    char c;
    int char_co = 0;
    while(txt_file.get(c)){
        char_co++;
    }
    return char_co;
}

bool FileRead(std::string in_path, std::string out_path){
    int file_size = GetFileSize(in_path); // Добавить название и его размер
    WriteToFile(out_path, std::to_string(file_size));
}