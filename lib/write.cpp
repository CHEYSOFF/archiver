#pragma once
#include <cmath>
#include <iostream>
#include <string>

bool *HammingCodeAddition(bool *char_bin, int char_bin_size);

bool *FromUCharToHammingBinary(int cur) {
    const int char_bin_size = 8;  // 8 bits in a char
    bool *char_bin = new bool[char_bin_size];
    for (int i = char_bin_size - 1; i >= 0; i--) {
        char_bin[i] = cur % 2;
        cur /= 2;
    }
    // int cur_char_bin = 0;
    // for (int i = 0; i < 12; i++) {
    //     // checking if on hammond bit
    //     if(i == 1 - 1 || i == 2 - 1 || i == 4 - 1 || i == 8 - 1){
    //         std::cout << '_';
    //     }
    //     else{
    //         std::cout << char_bin[cur_char_bin];
    //         cur_char_bin++;
    //     }
    // }
    // std::cout << '\n';
    bool *hamming_char_bin = HammingCodeAddition(char_bin, char_bin_size);
    delete[] char_bin;
    return hamming_char_bin;
}

bool *HammingCodeAddition(bool *char_bin, int char_bin_size) {
    const int hamming_char_bin_size = 12;  // (8 in a char) + (4 hamming codes)
    bool *hamming_char_bin = new bool[hamming_char_bin_size];

    // filling non-hamming positions in array with bits from binary one
    int cur_char_bin = 0;
    for (int i = 0; i < hamming_char_bin_size; i++) {
        // checking if on hammond bit
        if (i == 1 - 1 || i == 2 - 1 || i == 4 - 1 || i == 8 - 1) {
            hamming_char_bin[i] = 0;
        } else {
            hamming_char_bin[i] = char_bin[cur_char_bin];
            cur_char_bin++;
        }
    }

    // filling hamming positions in haming array
    for (int ham_ind = 1; ham_ind <= hamming_char_bin_size; ham_ind *= 2) {
        int co = 0;  // counting bits dor current hamming index

        for (int i = ham_ind - 1; i < hamming_char_bin_size; i += 2 * ham_ind) {
            for (int j = i; j < std::min(i + ham_ind, hamming_char_bin_size); j++) {
                if (hamming_char_bin[j]) {
                    co++;
                }
            }
        }
        hamming_char_bin[ham_ind - 1] = co % 2;
    }

    return hamming_char_bin;
}

bool WriteSizeToFile(std::string out_path, long long file_size) {
    const int hamming_char_bin_size = 12;  // (8 in a char) + (4 hamming codes)
    const int size_of_size = 64;           // (12 hamming_char_bin_size) * (5 bits) + 4 extra
    // to make up to even bytes
    bool size_array[size_of_size];

    size_array[0] = 0;
    size_array[1] = 0;
    size_array[2] = 0;
    size_array[3] = 0;

    for (int i = size_of_size - hamming_char_bin_size; i >= 0; i -= hamming_char_bin_size) {
        bool *hamming_char_bin = FromUCharToHammingBinary(file_size % 256);
        file_size /= 256;
        for (int j = i; j < i + hamming_char_bin_size; j++) {
            size_array[j] = hamming_char_bin[j - i];
            std::cout << size_array[j];
        }
        std::cout << '\n';
    }

    std::fstream out;
    out.open(out_path, std::ios::out | std::ios::binary | std::ios::app);
    
    int byte_size = 8;
    for (int i = 0; i < size_of_size; i += byte_size) {
        char byte = 0;
        for (int j = i; j < i + byte_size; j++) {
            byte *= 2;
            byte += size_array[j];
        }
        out << byte;
    }

    out.close();
}

// bool WriteToFile(std::string out_path, std::string text) {
//     std::fstream out;
//     out.open(out_path, std::ios::out | std::ios::binary | std::ios::app);
//     const int hamming_char_bin_size = 12;  // (8 in a char) + (4 hamming codes)
//     for (int i = 0; i < text.size(); i++) {
//         const int hamming_char_bin_size = 12;  // (8 in a char) + (4 hamming codes)
//         bool *hamming_char_bin = FromCharToHammingBinary(text[i]);

//         for(int i = 0; i < hamming_char_bin_size; i++){
//             out.write(hamming_char_bin[i]);
//         }
//     }
//     out.close();
// }