#pragma once
#include "../archive/read.cpp"

#include <direct.h>

#include <cstdio>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

#include "../consts.cpp"
#include "../hamming.cpp"

std::string GetFileName(std::string in_path);

// iteration_type:
// 0 - names
// 1 - unpack
// 2 - delete file
// 3 - merge archives

bool ArchiveIteration(std::string &archive_path, uint32_t iteration_type, std::set<std::string> &names, std::string &file_path, std::vector<std::string> extract_list);

// UNPACK
bool ArchiveIteration(std::string &archive_path, uint32_t iteration_type, std::vector<std::string> extract_list) {
    std::set<std::string> names_tmp;
    std::string file_path_tmp;
    return ArchiveIteration(archive_path, iteration_type, names_tmp, file_path_tmp, extract_list);
}

// FILE LIST
bool ArchiveIteration(std::string &archive_path, uint32_t iteration_type, std::set<std::string> &names) {
    std::string file_path_tmp;
    std::vector<std::string> extract_list_tmp;
    return ArchiveIteration(archive_path, iteration_type, names, file_path_tmp, extract_list_tmp);
}

// DELETE FILE
bool ArchiveIteration(std::string &archive_path, uint32_t iteration_type, std::string &file_path) {
    std::set<std::string> names_tmp;
    std::vector<std::string> extract_list_tmp;
    return ArchiveIteration(archive_path, iteration_type, names_tmp, file_path, extract_list_tmp);
}

// MERGE ARCHIVES
bool ArchiveIteration(std::string &archive_path, uint32_t iteration_type, std::string in_archive_path, std::set<std::string> &names) {
    std::vector<std::string> extract_list_tmp;
    return ArchiveIteration(archive_path, iteration_type, names, in_archive_path, extract_list_tmp);
}

// THE FUNCTION
bool ArchiveIteration(std::string &archive_path, uint32_t iteration_type, std::set<std::string> &names, std::string &file_path, std::vector<std::string> extract_list) {
    std::string second_archive_path = "../" + file_path + ".haf";

    bool end_of_file = 0;
    std::fstream tmp;

    std::string full_archive_path = "../" + archive_path + ".haf";
    std::string full_archive_path_tmp = full_archive_path + ".tmp";
    std::string full_folder_path = "../" + archive_path;

    // TMP OPENING
    if (iteration_type == 2) {
        tmp.open(full_archive_path_tmp, std::ifstream::out | std::ifstream::binary);
    }
    if (iteration_type == 3) {
        tmp.open(full_archive_path, std::ifstream::out | std::ifstream::binary | std::ifstream::app);
    }
    //

    if (iteration_type > 3) {
        std::cerr << "Wrong iteration type\n";
        return 0;
    }

    if (iteration_type == 1) {
        mkdir(full_folder_path.c_str());
    }

    // IN OPENING
    std::fstream in;
    if (iteration_type == 3) {
        in.open(second_archive_path, std::ifstream::in | std::ifstream::binary);
    } else {
        in.open(full_archive_path, std::ifstream::in | std::ifstream::binary);
    }
    //

    while (true) {
        if (!in.good()) {
            break;
        }
        unsigned char cc = in.get();
        if (cc == 255) {
            break;
        }

        // READING FILE SIZE
        std::vector<unsigned char> char_file_size(ham_size_of_size / byte_size);
        char_file_size[0] = cc;
        for (int i = 1; i < ham_size_of_size / byte_size; i++) {
            if (!in.good()) {
                std::cerr << "Corruption in file size(not enough bytes)\n";
                return 0;
            }
            char_file_size[i] = in.get();
        }
        long long num_file_size;
        if (!SizeFromCharHammingToInt(char_file_size, num_file_size)) {
            std::cerr << "Corruption in file size(wrong bytes)\n";
            return 0;
        }
        //

        // READING FILE NAME
        std::vector<unsigned char> char_file_name(ham_size_of_name / byte_size);
        for (int i = 0; i < ham_size_of_name / byte_size; i++) {
            if (!in.good()) {
                std::cerr << "Corruption in file name(not enough bytes)\n";
                return 0;
            }
            char_file_name[i] = in.get();
        }
        std::string string_file_name;
        if (!NameFromCharHammingToString(char_file_name, string_file_name)) {
            std::cerr << "Corruption in file name(wrong bytes)\n";
            return 0;
        }
        //

        // CHECKING IF WE NEED TO DELETE THIS FILE(if needed)
        bool should_write = 1;
        if (iteration_type == 2 || iteration_type == 3) {
            std::string file_name = GetFileName(file_path);
            switch (iteration_type) {
                case 2: {
                    if (file_name == string_file_name) {
                        should_write = 0;
                    }
                    break;
                }
                case 3: {
                    if (names.find(file_name) != names.end()) {
                        should_write = 0;
                    }
                    break;
                }
            }

            if (should_write) {
                // writing file size to tmp
                for (int i = 0; i < ham_size_of_size / byte_size; i++) {
                    tmp << char_file_size[i];
                }

                // writing file name to tmp
                for (int i = 0; i < ham_size_of_name / byte_size; i++) {
                    tmp << char_file_name[i];
                }
            }
        }
        //

        // CHECKING IF WE NEED TO EXTRACT THIS FILE(if needed)
        bool should_extract = 0;
        if (iteration_type == 1) {
            std::string file_name = GetFileName(string_file_name);

            for (int i = 0; i < extract_list.size(); i++) {
                if (extract_list[i] == file_name) {
                    should_extract = 1;
                }
            }
            if (extract_list.size() == 0) {
                should_extract = 1;
            }
        }
        //

        // NAME OUTPUT(if needed)
        if (iteration_type == 0) {
            names.insert(string_file_name);
        }
        //

        // READING FILE INFO AND WRITING IT
        std::string file_path = full_folder_path + '/' + string_file_name;
        std::fstream out;
        if (iteration_type == 1 && should_extract) {
            out.open(file_path, std::ifstream::out | std::ifstream::binary);
        }
        if (iteration_type == 3) {
            out.open(full_archive_path, std::ifstream::out | std::ifstream::binary | std::ifstream::app);
        }

        for (long long i = 0; i < num_file_size; i++) {
            if (!in.good()) {
                std::cerr << "Corruption in file info(not enough bytes)\n";
                return 0;
            }
            unsigned char a = in.get();
            if (!in.good()) {
                std::cerr << "Corruption in file info(not enough bytes)\n";
                return 0;
            }
            unsigned char b = in.get();
            unsigned char c;

            if (!InfoFrom2BytesHammingToChar(a, b, c)) {
                std::cerr << "Corruption in file info(wrong bytes)\n";
                return 0;
            }
            // WRITING FILE INFO(if needed)
            if (iteration_type == 1 && should_extract) {
                out << c;
                if (c == 255) {
                    continue;
                }
                
            }
            if ((iteration_type == 2 && should_write) || (iteration_type == 3 && should_write)) {
                tmp << a;
                tmp << b;
            }
        }

        if (iteration_type == 1 && should_extract) {
            out.close();
        }

        //
    }
    in.close();
    // GETTING RID OF TMP(if needed)
    if (iteration_type == 2) {
        tmp.close();
        std::remove((full_archive_path).c_str());
        if (std::rename((full_archive_path_tmp).c_str(), (full_archive_path).c_str())) {
            std::cerr << "Error renaming";
            return 1;
        }
    }
    //
    return 1;
}

bool FileList(std::string &archive_path, std::set<std::string> &names) {
    return ArchiveIteration(archive_path, 0, names);
}

bool ReadArchive(std::string &archive_path, std::vector<std::string> names) {
    return ArchiveIteration(archive_path, 1, names);
}

bool DeleteFile(std::string &archive_path, std::string file_path) {
    return ArchiveIteration(archive_path, 2, file_path);
}

bool MergeArchives(std::string &out_archive_path, std::string in_archive_path) {
    std::set<std::string> names;
    FileList(out_archive_path, names);
    return (ArchiveIteration(out_archive_path, 3, in_archive_path, names));
}
