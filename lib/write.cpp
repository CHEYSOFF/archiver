#pragma once
#include <iostream>
#include <string>
#include <cmath>

bool *HammingCodeAddition(bool *char_bin, int char_bin_size);

bool *FromCharToHammingBinary(char c) {
    const int char_bin_size = 8;  // 8 bits in a char
    bool *char_bin = new bool[char_bin_size];
    int cur = int(c);
    for (int i = char_bin_size - 1; i >= 0; i--) {
        char_bin[i] = cur % 2;
        cur /= 2;
    }

    bool *hamming_char_bin = HammingCodeAddition(char_bin, char_bin_size);
    int cur_char_bin = 0;
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
        if(i == 1 - 1 || i == 2 - 1 || i == 4 - 1 || i == 8 - 1){ 
            hamming_char_bin[i] = 0;
        }
        else{
            hamming_char_bin[i] = char_bin[cur_char_bin];
            cur_char_bin++;
        }
    }

    // filling hamming positions in haming array
    for (int ham_ind = 1; ham_ind <= hamming_char_bin_size; ham_ind *= 2) {
        int co = 0; // counting bits dor current hamming index

        for (int i = ham_ind - 1; i < hamming_char_bin_size; i += 2 * ham_ind) {

            for (int j = i; j < std::min(i + ham_ind, hamming_char_bin_size); j++){
                if(hamming_char_bin[j]){
                    co++;
                }

            }
        }
        hamming_char_bin[ham_ind - 1] = co % 2;

    }

    return hamming_char_bin;
}

bool WriteToFile(std::string in_path, std::string text) {
    const int hamming_char_bin_size = 12;  // (8 in a char) + (4 hamming codes)
    for (int i = 0; i < text.size(); i++) {
        bool *char_bin = FromCharToHammingBinary(text[i]);
        // bool *hamming_char_bin[hamming_char_bin_size] = HammingCodeAddition(char_bin);
        // delete[] char_bin;
    }
}