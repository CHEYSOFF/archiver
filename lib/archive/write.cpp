#pragma once
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../consts.cpp"
#include "../hamming.cpp"

void GetSizeArray(long long file_size, unsigned char ham_char_size_array[]) {
    bool size_array[ham_size_of_size];

    // size from long long to unsigned char
    unsigned char char_size_array[size_of_size / byte_size];
    for (int i = size_of_size / byte_size - 1; i >= 0; i--) {
        char_size_array[i] = (unsigned char)(file_size % 256);
        file_size /= 256;
    }

    // size from unsigned char to bool ham
    for (int i = 0; i < size_of_size / byte_size; i++) {
        bool *tmp = FromUCharToHammingBinary(char_size_array[i]);
        for (int j = 0; j < ham_bin_size_full; j++) {
            size_array[i * ham_bin_size_full + j] = tmp[j];
        }
        delete tmp;
    }

    // size from bool ham to unsigned char ham
    for (int i = 0; i < ham_size_of_size; i += byte_size) {
        unsigned char c = 0;
        for (int j = i; j < i + byte_size; j++) {
            c *= 2;
            c += size_array[j];
        }
        ham_char_size_array[i / byte_size] = c;
    }
}

void GetNameArray(std::string name_of_file, unsigned char ham_char_name_array[]) {
    bool name_array[ham_size_of_name];
    for (int i = 0; i < ham_size_of_name; i++) {
        name_array[i] = 0;
    }

    for (int i = 0; i < name_of_file.size(); i++) {
        bool *hamming_char_bin = FromUCharToHammingBinary(name_of_file[i]);
        for (int j = 0; j < ham_bin_size_full; j++) {
            name_array[(i * ham_bin_size_full) + j] = hamming_char_bin[j];
        }
    }

    for (int i = 0; i < ham_size_of_name; i += byte_size) {
        unsigned char c = 0;
        for (int j = i; j < i + byte_size; j++) {
            c *= 2;
            c += name_array[j];
        }
        ham_char_name_array[i / byte_size] = c;
    }
}

bool WriteInfoToFile(std::string out_path, long long file_size, std::string name_of_file, std::string in_path) {
    out_path = "../" + out_path + ".haf";

    unsigned char size_array[ham_size_of_size / byte_size];
    GetSizeArray(file_size, size_array);

    unsigned char name_array[ham_size_of_name / byte_size];
    GetNameArray(name_of_file, name_array);

    std::fstream out;
    out.open(out_path, std::ifstream::out | std::ifstream::binary | std::ifstream::app);
    for (int i = 0; i < ham_size_of_size / byte_size; i++) {
        out << size_array[i];
    }

    for (int i = 0; i < ham_size_of_name / byte_size; i++) {
        out << name_array[i];
    }

    std::fstream in;
    in.open(in_path, std::ifstream::in | std::ifstream::binary);

    for (int i = 0; i < file_size; i++) {
        unsigned char c;
        if (!in.good()) {
            return 0;
        }
        c = in.get();
        bool *hamming_file_info_byte = FromUCharToHammingBinary(c);
        unsigned char first_byte = 0;
        unsigned char second_byte = 0;
        for (int j = 0; j < ham_bin_size_half; j++) {
            first_byte *= 2;
            first_byte += hamming_file_info_byte[j];
        }
        for (int j = ham_bin_size_half; j < ham_bin_size_full; j++) {
            second_byte *= 2;
            second_byte += hamming_file_info_byte[j];
        }
        out << first_byte << second_byte;
    }
    out.close();
    return 1;
}

void CreateFile(std::string path) {
    std::ofstream a_file("../" + path + ".haf");
}
