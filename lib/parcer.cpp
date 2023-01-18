#pragma once

#include <algorithm>
#include <iostream>
#include <map>

#include "commands.cpp"

std::string ArgumentValue(char *argument) {
    int begining = 0;

    while (argument[begining] != '=') {
        if (begining > strlen(argument) - 1) {
            return "";
        }
        begining++;
    }
    begining++;

    while (isspace(argument[begining])) {
        begining++;
    }
    std::string tmp = argument;
    return tmp.substr(begining);
}

std::string ArgumentCrop(char *argument) {
    std::string ans = argument;
    int l;
    for (l = 0; l < ans.size(); l++) {
        if (ans[l] != '-') {
            break;
        }
    }
    int r = l;
    for (; r < ans.size(); r++) {
        if (ans[r] == '=') {
            r--;
            break;
        }
    }
    return ans.substr(l, r - l + 1);
}

void FlagInput(char *argument, std::vector<std::string> cur_flag) {
    std::string new_argument = ArgumentCrop(argument);
    std::map<std::string, int> m;
    m["c"] = 0;
    m["create"] = 0;
    m["f"] = 1;
    m["file"] = 1;
    m["l"] = 2;
    m["list"] = 2;
    m["x"] = 3;
    m["extract"] = 3;
    m["a"] = 4;
    m["append"] = 4;
    m["d"] = 5;
    m["delete"] = 5;
    m["A"] = 6;
    m["concatenate"] = 6;
    if (cur_flag.size() < 1) {
        std::cerr << "Not enough elements\n";
    }
    switch (m[new_argument]) {
        case 0: {  // -c, --create
            for (int i = 0; i < cur_flag.size(); i++) {
                Create(cur_flag[i]);
            }
            break;
        }
        case 1: {  // -f, --file=[ARHCNAME]
            break;
        }
        case 2: {  // -l, --list
            for (int i = 0; i < cur_flag.size(); i++) {
                List(cur_flag[i]);
            }
            break;
        }
        case 3: {  // -x, --extract
            std::vector<std::string> tmp;
            for (int i = 1; i < cur_flag.size(); i++) {
                tmp.push_back(cur_flag[i]);
            }
            Extract(cur_flag[0], tmp);
            break;
        }
        case 4: {  // -a, --append
            for (int i = 1; i < cur_flag.size(); i++) {
                Append(cur_flag[0], cur_flag[i]);
            }
            break;
        }
        case 5: {  // -d, --delete

            for (int i = 1; i < cur_flag.size(); i++) {
                Delete(cur_flag[0], cur_flag[i]);
            }
            break;
        }
        case 6: {  // -A, --concatenate
            if (cur_flag.size() < 3) {
                std::cerr << "Not enough elements\n";
            }
            Concatenate(cur_flag[0], cur_flag[1], cur_flag[2]);
            break;
        }
        default:
            break;
    }
}

void ReadFlags(int argc, char *argv[]) {
    std::vector<std::string> names;
    for (int i = argc - 1; i >= 1; i--) {
        if (argv[i][0] != '-') {
            names.push_back(argv[i]);
        } else {
            std::string argument = ArgumentValue(argv[i]);
            if (argument.size() != 0) {
                names.push_back(argument);
            }
            break;
        }
    }

    std::reverse(names.begin(), names.end());
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') {
            continue;
        }
        FlagInput(argv[i], names);
    }
}
