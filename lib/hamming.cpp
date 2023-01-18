#pragma once
#include <iostream>

#include "consts.cpp"

bool HammingBit(int i) {
    if (i == 1 - 1 || i == 1 - 1 + ham_bin_size_half ||
        i == 2 - 1 || i == 2 - 1 + ham_bin_size_half ||
        i == 4 - 1 || i == 4 - 1 + ham_bin_size_half ||
        i == 8 - 1 || i == 8 - 1 + ham_bin_size_half) {
        return 1;
    } else {
        return 0;
    }
}

int HammingCount(bool *v, int ham_ind) {
    int co = 0;  // counting bits for current hamming index
    for (int i = 0; i < ham_bin_size_half; i++) {
        if (((i + 1) / ham_ind) & 1) {
            co += v[i];
        }
    }
    return co;
}

bool *HammingCodeAddition(bool *char_bin) {
    bool *hamming_char_bin = new bool[ham_bin_size_full];

    // filling non-hamming positions in array with bits from binary one
    int cur_char_bin = 0;
    for (int i = 0; i < ham_bin_size_full; i++) {
        // checking if on hammond bit
        if (HammingBit(i)) {
            hamming_char_bin[i] = 0;
        } else {
            hamming_char_bin[i] = char_bin[cur_char_bin];
            cur_char_bin++;
        }
    }

    // filling hamming positions in haming array
    for (int half = 0; half < 2; half++) {
        for (int ham_ind = 1; ham_ind <= ham_bin_size_half; ham_ind *= 2) {
            int co = HammingCount(hamming_char_bin + (half * ham_bin_size_half) * sizeof(bool), ham_ind);
            hamming_char_bin[(half * ham_bin_size_half) + ham_ind - 1] = co % 2;
        }
    }

    return hamming_char_bin;
}

bool *FromUCharToHammingBinary(int cur) {
    bool *char_bin = new bool[ham_bin_size_half];
    for (int i = ham_bin_size_half - 1; i >= 0; i--) {
        char_bin[i] = cur % 2;
        cur /= 2;
    }
    bool *hamming_char_bin = HammingCodeAddition(char_bin);
    delete[] char_bin;
    return hamming_char_bin;
}

bool CorruptionCheck(bool v[]) {
    int wrong_bit = 0;
    bool was_wrong = 0;
    for (int i = 0; i < byte_size; i++) {
        if (HammingBit(i)) {
            int co = HammingCount(v, i + 1);
            if (co % 2 == 1) {
                wrong_bit += i;
                was_wrong = 1;
            }
        }
    }
    if (was_wrong) {
        if (wrong_bit < byte_size) {
            v[wrong_bit] = (1 + v[wrong_bit]) % 2;
        } else {
            return 0;
        }
    }
    return 1;
}

bool FromUCharToBinary(bool ans[], unsigned char c) {
    for (int i = byte_size - 1; i >= 0; i--) {
        ans[i] = c % 2;
        c /= 2;
    }
    return CorruptionCheck(ans);
}

bool SizeFromCharHammingToInt(std::vector<unsigned char> &uchar_hamming, long long &result) {
    result = 0;
    for (int i = 0; i < ham_size_of_size / byte_size; i++) {
        bool tmp[byte_size];
        if (!FromUCharToBinary(tmp, uchar_hamming[i])) {
            return 0;
        }
        for (int j = 0; j < byte_size; j++) {
            if (!HammingBit(j)) {
                result *= 2;
                result += tmp[j];
            }
        }
    }
    return 1;
}

bool NameFromCharHammingToString(std::vector<unsigned char> &uchar_hamming, std::string &result) {
    result = "";
    bool bool_file_name[ham_size_of_name];
    for (int i = 0; i < ham_size_of_name; i++) {
        bool_file_name[i] = 0;
    }
    int cur = 0;
    for (int i = 0; i < ham_size_of_name / byte_size; i++) {
        bool tmp[byte_size];
        if (!FromUCharToBinary(tmp, uchar_hamming[i])) {
            return 0;
        }
        for (int j = 0; j < byte_size; j++) {
            if (!HammingBit(j)) {
                bool_file_name[cur] = tmp[j];
                cur++;
            }
        }
    }
    for (int i = 0; i < ham_size_of_name / byte_size; i++) {
        unsigned char c = 0;
        for (int j = 0; j < byte_size; j++) {
            c *= 2;
            c += bool_file_name[(i * byte_size) + j];
        }
        result += c;
    }
    int i;
    std::string tmp = "";
    for (i = 0; i < result.size(); i++) {
        if (isprint(result[i])) {
            tmp += result[i];
        }
    }
    result = tmp;
    return 1;
}

bool InfoFrom2BytesHammingToChar(unsigned char a, unsigned char b, unsigned char &c) {
    bool binary_a[byte_size];
    bool binary_b[byte_size];
    if (!FromUCharToBinary(binary_a, a) || !FromUCharToBinary(binary_b, b)) {
        return 0;
    }
    c = 0;
    for (int i = 0; i < byte_size; i++) {
        if (!HammingBit(i)) {
            c *= 2;
            c += binary_a[i];
        }
    }

    for (int i = 0; i < byte_size; i++) {
        if (!HammingBit(i)) {
            c *= 2;
            c += binary_b[i];
        }
    }
    return 1;
}