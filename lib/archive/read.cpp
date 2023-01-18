#pragma once
#include "../unarchive/read.cpp"

#include <fstream>
#include <iostream>
#include <set>
#include <string>

#include "write.cpp"

int GetFileSize(std::string in_path) {
    long long max_size = 1'000'000'000'000 / 8;
    // max file_size is 1'000'000'000'000 bits = 125'000'000'000 mb = 125 gb
    // => 40 bits = 5 bytes for file size
    std::fstream in;
    in.open(in_path, std::ifstream::binary | std::ifstream::in);
    unsigned char c;
    long long char_co = 0;
    while (in.good()) {
        c = in.get();
        char_co++;
        if (char_co > max_size) {
            return -1;
        }
    }
    return char_co;
}

std::string GetFileName(std::string in_path) {
    int begining = 0;
    for (int i = 0; i < in_path.size(); i++) {
        if (in_path[i] == '\\' || in_path[i] == '/') {
            begining = i + 1;
        }
    }
    return in_path.substr(begining);
}

bool AddFile(std::string in_path, std::string out_path) {
    in_path = "../" + in_path;
    std::string file_name = GetFileName(in_path);

    // CHECKING IF FILE ALREADY IN ARCHIVE
    std::set<std::string> archive_file_names;
    FileList(out_path, archive_file_names);
    if (archive_file_names.find(file_name) != archive_file_names.end()) {
        DeleteFile(out_path, file_name);
    }
    //

    if (file_name.size() > max_name_length) {
        std::cerr << "File name is too large(max length is" << max_name_length << " symbols)\n";
        return 0;
    }
    long long file_size = GetFileSize(in_path);
    if (file_size == -1) {
        std::cerr << file_name << " is too large(max size is 125 gb)\n";
        return 0;
    }
    if (!WriteInfoToFile(out_path, file_size, file_name, in_path)) {
        std::cerr << "Wrong path of the file\n";
        return 0;
    }

    return 1;
}
