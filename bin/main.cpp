#include <iostream>

#include "../lib/read.cpp"
#include "../lib/write.cpp"

//  file struct:
//  BEFORE HAMMING
//  name    bytes   bits
//  size    5       40     
//  name    21      168
//  AFTER HAMMING
//  name    bytes   bits
//  size    8       64      
//  name    32      256 

int main() {
    // bool *hamming_char_bin = FromCharToHammingBinary(72);
    
    // for (int i = 0; i < 12; i++) {
    //     std::cout << hamming_char_bin[i];
    // }
    WriteSizeToFile("asba.txt", 1);
}
