#include <iostream>

#include "../lib/read.cpp"
#include "../lib/write.cpp"

int main() {
    bool *hamming_char_bin = FromCharToHammingBinary(72);\
    
    for (int i = 0; i < 12; i++) {
        std::cout << hamming_char_bin[i];
    }
}
