#pragma once

const int byte_size = 8;
const int char_size = 256;
const int char_size_half = char_size / 2;

const int size_of_size = 5 * 8;                 // (5 bits) * (byte_size)
const int ham_size_of_size = size_of_size * 2;  // size_of_size * (2 hammings extra)

const int max_name_length = 16;
const int size_of_name = max_name_length * byte_size;
const int ham_size_of_name = size_of_name * 2;  // size_of_name * (2 hammings extra)

const int ham_bin_size_full = 16;  // 2 * ((4 in a char) + (4 hamming codes)) ||| per byte
const int ham_bin_size_half = ham_bin_size_full / 2;

/*
bytes without hamming
5    +   16  =   21
size     name
bytes with hamming
5    *   2      =   10
16   *   2      =   32
                    ||
                    42
*/

/*
FILE STRUCT:
    File size (10) (125gb max)
    File name (32)
    File info (File size)
*/
